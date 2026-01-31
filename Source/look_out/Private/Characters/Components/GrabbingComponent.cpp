// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/GrabbingComponent.h"

#include "Camera/CameraComponent.h"
#include "Characters/BaseCharacter.h"
#include "Characters/Components/MovementComponentExtended.h"
#include "Components/CapsuleComponent.h"
#include "Interfaces/Grabbable.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

// Sets default values for this component's properties
UGrabbingComponent::UGrabbingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabbingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
}


// Called every frame
void UGrabbingComponent::TickComponent(const float DeltaTime, const ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGrabbingComponent::LightweightObjectRotation(float InputAxisX, float InputAxisY)
{
	InputAxisX *= -1;
	InputAxisY *= -1;

	GrabRotation.Roll = GrabRotation.Roll - InputAxisY;
	GrabRotation.Yaw = GrabRotation.Yaw - InputAxisX;
}

EGrabbableObjectType UGrabbingComponent::GrabbedObjectType()
{
	EGrabbableObjectType Result = None;

	const UPrimitiveComponent* GrabbedComponent = nullptr;

	if (OwnerCharacter->PhysicsHandle && OwnerCharacter->PhysicsHandle->GetGrabbedComponent())
	{
		GrabbedComponent = OwnerCharacter->PhysicsHandle->GetGrabbedComponent();
	}

	if (HeavyObject != nullptr)
	{
		Result = Heavyweight;
	}
	if (GrabbedComponent)
	{
		Result = Lightweight;
	}

	return Result;
}

void UGrabbingComponent::ToggleGrabComponent(const bool State)
{
	if (State)
	{
		const auto [StartVector, EndVector] = OwnerCharacter->GetForwardVectorRelatedToCamera(GrabDistance);

		FHitResult HitResult;

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(OwnerCharacter);

		const bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			StartVector,
			EndVector,
			ECC_Visibility,
			QueryParams
		);

		if (bHit)
		{
			AActor* HitActor = HitResult.GetActor();
			UPrimitiveComponent* HitActorComponent = HitResult.GetComponent();

			if (HitActor && HitActor->GetClass()->ImplementsInterface(UGrabbable::StaticClass()))
			{
				if (IGrabbable* GrabbableInterface = Cast<IGrabbable>(HitActor))
				{
					if (const EGrabbableObjectType Type = GrabbableInterface->GetGrabbableType(); Type == Lightweight)
					{
						OwnerCharacter->PhysicsHandle->GrabComponentAtLocation(
							HitActorComponent, 
							NAME_None,
							HitActorComponent->GetComponentLocation()
						);
						GrabRotation = HitActorComponent->GetComponentRotation();
						IsGrabbingObject = true;
					}
					else if (Type == Heavyweight)
					{
						HeavyObject = HitActorComponent;
						OwnerCharacter->PhysicsConstraint->SetConstrainedComponents(
							HitActorComponent, NAME_None,
							OwnerCharacter->GetCapsuleComponent(), NAME_None
						);
						OwnerCharacter->MovementComponentExtended->CanSprint = false;
						OwnerCharacter->MovementComponentExtended->ToggleSprint(false);
						OwnerCharacter->MovementComponentExtended->ChangeWalkSpeed(
							OwnerCharacter->MovementComponentExtended->DragSpeed
						);
						IsGrabbingObject = true;
					}
				}
			}
		}
	}
	else
	{
		if (const EGrabbableObjectType ObjectType = GrabbedObjectType(); ObjectType == Lightweight)
		{
			OwnerCharacter->PhysicsHandle->ReleaseComponent();
		}
		else if (ObjectType == Heavyweight)
		{
			HeavyObject = nullptr;
			OwnerCharacter->PhysicsConstraint->BreakConstraint();
			OwnerCharacter->MovementComponentExtended->CanSprint = true;
			OwnerCharacter->MovementComponentExtended->ChangeWalkSpeed(
				OwnerCharacter->MovementComponentExtended->WalkSpeed);
		}
		IsGrabbingObject = false;
	}
}

void UGrabbingComponent::ProcessGrabbing(const float DeltaSeconds)
{
	if (const EGrabbableObjectType ObjectType = GrabbedObjectType(); ObjectType == Lightweight)
	{
		auto [StartVector, EndVector] = OwnerCharacter->GetForwardVectorRelatedToCamera(GrabDistance);
		OwnerCharacter->PhysicsHandle->SetTargetLocation(EndVector);
		UPrimitiveComponent* GrabbedComponent = OwnerCharacter->PhysicsHandle->GetGrabbedComponent();
		const FRotator NewRotation = FMath::RInterpTo(
			GrabbedComponent->GetComponentRotation(), GrabRotation, DeltaSeconds,
			10.0);
		GrabbedComponent->SetWorldRotation(NewRotation);
	}
	else if (ObjectType == Heavyweight)
	{
		UPrimitiveComponent* Component1 = nullptr;
		FName BoneName1;
		UPrimitiveComponent* Component2 = nullptr;
		FName BoneName2;

		OwnerCharacter->PhysicsConstraint->GetConstrainedComponents(
			Component1,
			BoneName1,
			Component2,
			BoneName2
		);

		const FVector Component2Location = Component2->GetComponentLocation();
		const FVector Component1Location = Component1->GetComponentLocation();

		const float DistanceVectorLength = FVector::Dist(Component1Location, Component2Location);

		if (const float MaxDistance = GrabDistance * 1.5; DistanceVectorLength >= MaxDistance)
		{
			FVector Difference = Component1Location - Component2Location;
			Difference.Normalize();
			const float ObjectScale = Component1->GetComponentScale().X;

			const FVector FinalLaunchVector = DistanceVectorLength * Difference * ObjectScale * 0.1;
			OwnerCharacter->LaunchCharacter(FinalLaunchVector, false, false);
		}
	}
}

void UGrabbingComponent::ThrowObject()
{
	if (const EGrabbableObjectType ObjectType = GrabbedObjectType(); ObjectType == Lightweight)
	{
		UPrimitiveComponent* GrabbedComponent = OwnerCharacter->PhysicsHandle->GetGrabbedComponent();
		const FVector ForwardVector = OwnerCharacter->Camera->GetForwardVector();

		const float Force = FMath::Clamp(Strength / GrabbedComponent->GetMass(), 500, Strength);
		const FVector ForceVector = ForwardVector * Force;
		
		ToggleGrabComponent(false);
		GrabbedComponent->SetAllPhysicsLinearVelocity(ForceVector);
	}
}