// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/GrabbingComponent.h"

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
void UGrabbingComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ProcessGrabbing(DeltaTime);
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
		FRelatedForwardVectorResult CameraPoints = OwnerCharacter->GetForwardVectorRelatedToCamera(GrabDistance);

		FHitResult HitResult;

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(OwnerCharacter);

		const bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			CameraPoints.StartVector,
			CameraPoints.EndVector,
			ECC_Visibility,
			QueryParams
		);

		if (bHit)
		{
			AActor* HitActor = HitResult.GetActor();
			UPrimitiveComponent* HitActorComponent = HitResult.GetComponent();

			if (HitActor->ActorHasTag("lightweight"))
			{
				OwnerCharacter->PhysicsHandle->GrabComponentAtLocation(HitActorComponent, "None",
				                                                       HitActorComponent->GetComponentLocation());
				GrabRotation = HitActorComponent->GetComponentRotation();
				IsGrabbingObject = true;
			}
			else
			{
				if (HitActor->ActorHasTag("heavyweight"))
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
		FWeightCheckResult Result = GrabbedObjectType();
		if (Result.bIsNotHeavy)
		{
			OwnerCharacter->PhysicsHandle->ReleaseComponent();
		}
		else if (Result.bIsHeavy)
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
	FWeightCheckResult Result = GrabbedObjectType();
	if (Result.bIsNotHeavy)
	{
		FRelatedForwardVectorResult VectorResult = OwnerCharacter->GetForwardVectorRelatedToCamera(GrabDistance);
		OwnerCharacter->PhysicsHandle->SetTargetLocation(VectorResult.EndVector);
		UPrimitiveComponent* GrabbedComponent = OwnerCharacter->PhysicsHandle->GetGrabbedComponent();
		FRotator NewRotation = FMath::RInterpTo(
			GrabbedComponent->GetComponentRotation(), GrabRotation, DeltaSeconds,
			10.0);
		GrabbedComponent->SetWorldRotation(NewRotation);
	}
	else if (Result.bIsHeavy)
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

		FVector Component2Location = Component2->GetComponentLocation();
		FVector Component1Location = Component1->GetComponentLocation();

		float DistanceVectorLength = FVector::Dist(Component1Location, Component2Location);
		float MaxDistance = GrabDistance * 1.5;

		if (DistanceVectorLength >= MaxDistance)
		{
			FVector Difference = Component1Location - Component2Location;
			Difference.Normalize();
			float ObjectScale = Component1->GetComponentScale().X;

			FVector FinalLaunchVector = DistanceVectorLength * Difference * ObjectScale * 0.1;
			OwnerCharacter->LaunchCharacter(FinalLaunchVector, false, false);
		}
	}
}
