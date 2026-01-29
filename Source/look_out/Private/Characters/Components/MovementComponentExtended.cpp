// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/MovementComponentExtended.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/BaseCharacter.h"
#include "Characters/Components/GrabbingComponent.h"

// Sets default values for this component's properties
UMovementComponentExtended::UMovementComponentExtended()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMovementComponentExtended::BeginPlay()
{
	Super::BeginPlay();

	// ...
	OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
}


// Called every frame
void UMovementComponentExtended::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMovementComponentExtended::MoveForward(float AxisValue)
{
	const FRotator ControlRotation = OwnerCharacter->GetControlRotation();

	const FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	OwnerCharacter->AddMovementInput(ForwardDirection, AxisValue);
}

void UMovementComponentExtended::MoveRight(float AxisValue)
{
	const FRotator ControlRotation = OwnerCharacter->GetControlRotation();

	const FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	OwnerCharacter->AddMovementInput(RightDirection, AxisValue);
}

void UMovementComponentExtended::ToggleSprint(bool State)
{
	if (State && CanSprint)
	{
		ChangeWalkSpeed(SprintSpeed);
		IsSprinting = true;
	}
	else if (!State)
	{
		ChangeWalkSpeed(WalkSpeed);
		IsSprinting = false;
	}
}

void UMovementComponentExtended::JumpAction()
{
	OwnerCharacter->Jump();
}

void UMovementComponentExtended::ChangeWalkSpeed(float _WalkSpeed)
{
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = _WalkSpeed;
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeedCrouched = _WalkSpeed / 2;
}


void UMovementComponentExtended::Look(float InputAxisX, float InputAxisY, bool bHoldingRMB)
{
	bool HaveToRotateItem = OwnerCharacter->GrabbingComponent->IsGrabbingObject && bHoldingRMB && OwnerCharacter->GrabbingComponent->GrabbedObjectType().
		bIsNotHeavy;

	if (!HaveToRotateItem)
	{
		OwnerCharacter->AddControllerYawInput(InputAxisX);
		OwnerCharacter->AddControllerPitchInput(InputAxisY);
	}
	else
	{
		OwnerCharacter->GrabbingComponent->LightweightObjectRotation(InputAxisX, InputAxisY);
	}
}

void UMovementComponentExtended::DoCrouch(bool State)
{
	if (State)
	{
		if (OwnerCharacter->CanCrouch())
		{
			OwnerCharacter->Crouch();
		}
		else OwnerCharacter->UnCrouch();
	}
	else
	{
		OwnerCharacter->UnCrouch();
	}
}
