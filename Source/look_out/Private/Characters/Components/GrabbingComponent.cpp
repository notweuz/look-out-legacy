// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/GrabbingComponent.h"

#include "Characters/BaseCharacter.h"
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
void UGrabbingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
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

	UPrimitiveComponent* GrabbedComponent = nullptr;

	if (OwnerCharacter->PhysicsHandle && OwnerCharacter->PhysicsHandle->GetGrabbedComponent())
	{
		GrabbedComponent = OwnerCharacter->PhysicsHandle->GetGrabbedComponent();
	}

	if (GrabbedComponent && HeavyObject)
	{
		bool IsObjectHeavy = GrabbedComponent == HeavyObject;

		if (IsObjectHeavy)
		{
			Result.bIsHeavy = true;
			Result.bIsNotHeavy = false;
		}
		else
		{
			Result.bIsHeavy = false;
			Result.bIsNotHeavy = true;
		}
	}
	else
	{
		Result.bIsHeavy = false;
		Result.bIsNotHeavy = false;
	}

	return Result;
}