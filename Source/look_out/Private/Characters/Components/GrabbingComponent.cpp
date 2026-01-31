// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/GrabbingComponent.h"

#include "Camera/CameraComponent.h"
#include "Characters/BaseCharacter.h"
#include "Characters/Components/MovementComponentExtended.h"
#include "Components/CapsuleComponent.h"
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

FWeightCheckResult UGrabbingComponent::GrabbedObjectType()
{
	FWeightCheckResult Result;

	const UPrimitiveComponent* GrabbedComponent = nullptr;

	if (OwnerCharacter->PhysicsHandle && OwnerCharacter->PhysicsHandle->GetGrabbedComponent())
	{
		GrabbedComponent = OwnerCharacter->PhysicsHandle->GetGrabbedComponent();
	}

	if (HeavyObject != nullptr)
	{
		Result.bIsHeavy = true;
	}
	if (GrabbedComponent)
	{
		Result.bIsNotHeavy = true;
	}

	return Result;
}

void UGrabbingComponent::ToggleGrabComponent(bool State)
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
			const AActor* HitActor = HitResult.GetActor();
			UPrimitiveComponent* HitActorComponent = HitResult.GetComponent();

			if (HitActor->ActorHasTag("grabbable"))
			{
				OwnerCharacter->PhysicsHandle->GrabComponentAtLocation(HitActorComponent, "None",
				                                                       HitActorComponent->GetComponentLocation());
				GrabRotation = HitActorComponent->GetComponentRotation();
				IsGrabbingObject = true;
			}
			else
			{
				if (HitActor->ActorHasTag("draggable"))
				{
					HeavyObject = HitActorComponent;
					OwnerCharacter->PhysicsConstraint->SetConstrainedComponents(
						HitActorComponent, "None", OwnerCharacter->GetCapsuleComponent(), "None");
					OwnerCharacter->MovementComponentExtended->CanSprint = false;
					OwnerCharacter->MovementComponentExtended->ToggleSprint(false);
					OwnerCharacter->MovementComponentExtended->ChangeWalkSpeed(
						OwnerCharacter->MovementComponentExtended->DragSpeed);
					IsGrabbingObject = true;
				}
			}
		}
	}
	else
	{
		auto [bIsHeavy, bIsNotHeavy] = GrabbedObjectType();
		if (bIsNotHeavy)
		{
			OwnerCharacter->PhysicsHandle->ReleaseComponent();
		}
		else if (bIsHeavy)
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

void UGrabbingComponent::ProcessGrabbing(float DeltaSeconds)
{
	auto [bIsHeavy, bIsNotHeavy] = GrabbedObjectType();
	if (bIsNotHeavy)
	{
		auto [StartVector, EndVector] = OwnerCharacter->GetForwardVectorRelatedToCamera(GrabDistance);
		OwnerCharacter->PhysicsHandle->SetTargetLocation(EndVector);
		UPrimitiveComponent* GrabbedComponent = OwnerCharacter->PhysicsHandle->GetGrabbedComponent();
		const FRotator NewRotation = FMath::RInterpTo(
			GrabbedComponent->GetComponentRotation(), GrabRotation, DeltaSeconds,
			10.0);
		GrabbedComponent->SetWorldRotation(NewRotation);
	}
	else if (bIsHeavy)
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
	auto [bIsHeavy, bIsNotHeavy] = GrabbedObjectType();
	if (bIsNotHeavy)
	{
		UPrimitiveComponent* GrabbedComponent = OwnerCharacter->PhysicsHandle->GetGrabbedComponent();
		const FVector ForwardVector = OwnerCharacter->Camera->GetForwardVector();

		const float Force = FMath::Clamp(Strength / GrabbedComponent->GetMass(), 500, Strength);
		const FVector ForceVector = ForwardVector * Force;
		
		ToggleGrabComponent(false);
		GrabbedComponent->SetAllPhysicsLinearVelocity(ForceVector);
	}
}
