// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/MovementComponentExtended.h"

#include "Characters/BaseCharacter.h"
#include "Characters/Components/GrabbingComponent.h"
#include "Characters/Components/PlayerInputsComponent.h"
#include "Core/AdvancedGameUserSettings.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UMovementComponentExtended::UMovementComponentExtended()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UMovementComponentExtended::BeginPlay()
{
	Super::BeginPlay();

	// ...
	OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
}

bool UMovementComponentExtended::HasMovementOwner() const
{
	return OwnerCharacter != nullptr;
}

bool UMovementComponentExtended::IsDraggingHeavyObject() const
{
	return OwnerCharacter &&
		OwnerCharacter->GrabbingComponent &&
		OwnerCharacter->GrabbingComponent->IsGrabbingObject &&
		OwnerCharacter->GrabbingComponent->GetGrabbedObjectType() == Heavyweight;
}

UCharacterMovementComponent* UMovementComponentExtended::GetCharacterMovementComponent() const
{
	return OwnerCharacter ? OwnerCharacter->GetCharacterMovement() : nullptr;
}

float UMovementComponentExtended::GetMouseSensitivity() const
{
	if (const UAdvancedGameUserSettings* Settings = UAdvancedGameUserSettings::GetAdvancedGameUserSettings())
	{
		return Settings->MouseSensitivity;
	}

	return 1.0f;
}

void UMovementComponentExtended::MoveForward(const float AxisValue)
{
	if (!HasMovementOwner())
	{
		return;
	}

	const FRotator ControlRotation = OwnerCharacter->GetControlRotation();

	const FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	OwnerCharacter->AddMovementInput(ForwardDirection, AxisValue);
}

void UMovementComponentExtended::MoveRight(const float AxisValue)
{
	if (!HasMovementOwner())
	{
		return;
	}

	const FRotator ControlRotation = OwnerCharacter->GetControlRotation();

	const FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	OwnerCharacter->AddMovementInput(RightDirection, AxisValue);
}

void UMovementComponentExtended::Look(float InputAxisX, float InputAxisY)
{
	if (!HasMovementOwner() || !OwnerCharacter->PlayerInputsComponent || !OwnerCharacter->GrabbingComponent)
	{
		return;
	}

	const bool bRotateObject = OwnerCharacter->PlayerInputsComponent->IsActionHeld(
		OwnerCharacter->PlayerInputsComponent->IA_Rotate);
	if (const bool HaveToRotateItem = OwnerCharacter->GrabbingComponent->IsGrabbingObject && bRotateObject &&
		OwnerCharacter->GrabbingComponent->GetGrabbedObjectType() == Lightweight; !HaveToRotateItem)
	{
		float MouseSensitivity = GetMouseSensitivity();
		if (IsDraggingHeavyObject())
		{
			MouseSensitivity *= HeavyDragLookSensitivityMultiplier;
		}

		InputAxisX *= MouseSensitivity;
		InputAxisY *= MouseSensitivity;
		OwnerCharacter->AddControllerYawInput(InputAxisX);
		OwnerCharacter->AddControllerPitchInput(InputAxisY);
	}
	else
	{
		OwnerCharacter->GrabbingComponent->RotateLightweightObject(InputAxisX, InputAxisY);
	}
}

void UMovementComponentExtended::JumpAction()
{
	if (HasMovementOwner())
	{
		OwnerCharacter->Jump();
	}
}

void UMovementComponentExtended::DoCrouch(const bool State)
{
	if (!HasMovementOwner())
	{
		return;
	}

	if (State)
	{
		if (OwnerCharacter->CanCrouch())
		{
			OwnerCharacter->Crouch();
		}
		else
		{
			OwnerCharacter->UnCrouch();
		}
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
	if (UCharacterMovementComponent* CharacterMovement = GetCharacterMovementComponent())
	{
		CharacterMovement->MaxWalkSpeed = _WalkSpeed;
		CharacterMovement->MaxWalkSpeedCrouched = _WalkSpeed / 2.0f;
	}
}
