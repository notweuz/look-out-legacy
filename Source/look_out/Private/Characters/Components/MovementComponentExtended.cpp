// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/MovementComponentExtended.h"

#include "Characters/BaseCharacter.h"
#include "Characters/Components/GrabbingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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


void UMovementComponentExtended::MoveForward(const float AxisValue)
{
	const FRotator ControlRotation = OwnerCharacter->GetControlRotation();

	const FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	OwnerCharacter->AddMovementInput(ForwardDirection, AxisValue);
}

void UMovementComponentExtended::MoveRight(const float AxisValue)
{
	const FRotator ControlRotation = OwnerCharacter->GetControlRotation();

	const FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	OwnerCharacter->AddMovementInput(RightDirection, AxisValue);
}

void UMovementComponentExtended::Look(const float InputAxisX, const float InputAxisY, const bool bHoldingRMB)
{
	const bool HaveToRotateItem = OwnerCharacter->GrabbingComponent->IsGrabbingObject && bHoldingRMB && OwnerCharacter->GrabbingComponent->GrabbedObjectType() == Lightweight;

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

void UMovementComponentExtended::JumpAction()
{
	OwnerCharacter->Jump();
}

void UMovementComponentExtended::DoCrouch(const bool State)
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

// Called every frame
void UMovementComponentExtended::TickComponent(const float DeltaTime, const ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UMovementComponentExtended::ToggleSprint(const bool State)
{
	if (State && CanSprint)
	{
		ChangeWalkSpeed(SprintSpeed);
		IsSprinting = true;
	}
	else if (!State && CanSprint)
	{
		ChangeWalkSpeed(WalkSpeed);
		IsSprinting = false;
	}
}

void UMovementComponentExtended::ChangeWalkSpeed(const float _WalkSpeed)
{
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = _WalkSpeed;
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeedCrouched = _WalkSpeed / 2;
}
