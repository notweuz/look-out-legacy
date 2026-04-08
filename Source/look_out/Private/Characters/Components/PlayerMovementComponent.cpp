// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/PlayerMovementComponent.h"

#include "Characters/BaseCharacter.h"
#include "Characters/Components/PlayerGrabComponent.h"
#include "Characters/Components/PlayerInputsComponent.h"
#include "Core/AdvancedGameUserSettings.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UPlayerMovementComponent::UPlayerMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UPlayerMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
}

bool UPlayerMovementComponent::HasMovementOwner() const
{
	return OwnerCharacter != nullptr;
}

bool UPlayerMovementComponent::IsDraggingHeavyObject() const
{
	return OwnerCharacter &&
		OwnerCharacter->PlayerGrabComponent &&
		OwnerCharacter->PlayerGrabComponent->IsGrabbingObject &&
		OwnerCharacter->PlayerGrabComponent->GetGrabbedObjectType() == Heavyweight;
}

UCharacterMovementComponent* UPlayerMovementComponent::GetCharacterMovementComponent() const
{
	return OwnerCharacter ? OwnerCharacter->GetCharacterMovement() : nullptr;
}

float UPlayerMovementComponent::GetMouseSensitivity() const
{
	if (const UAdvancedGameUserSettings* Settings = UAdvancedGameUserSettings::GetAdvancedGameUserSettings())
	{
		return Settings->MouseSensitivity;
	}

	return 1.0f;
}

void UPlayerMovementComponent::MoveForward(const float AxisValue)
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

void UPlayerMovementComponent::MoveRight(const float AxisValue)
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

void UPlayerMovementComponent::Look(float InputAxisX, float InputAxisY)
{
	if (!HasMovementOwner() || !OwnerCharacter->PlayerInputsComponent || !OwnerCharacter->PlayerGrabComponent)
	{
		return;
	}

	const bool bRotateObject = OwnerCharacter->PlayerInputsComponent->IsActionHeld(
		OwnerCharacter->PlayerInputsComponent->IA_Rotate);
	if (const bool HaveToRotateItem = OwnerCharacter->PlayerGrabComponent->IsGrabbingObject && bRotateObject &&
		OwnerCharacter->PlayerGrabComponent->GetGrabbedObjectType() == Lightweight; !HaveToRotateItem)
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
		OwnerCharacter->PlayerGrabComponent->RotateLightweightObject(InputAxisX, InputAxisY);
	}
}

void UPlayerMovementComponent::JumpAction()
{
	if (HasMovementOwner())
	{
		OwnerCharacter->Jump();
	}
}

void UPlayerMovementComponent::DoCrouch(const bool State)
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
void UPlayerMovementComponent::TickComponent(const float DeltaTime, const ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UPlayerMovementComponent::ToggleSprint(const bool State)
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

void UPlayerMovementComponent::ChangeWalkSpeed(const float _WalkSpeed)
{
	if (UCharacterMovementComponent* CharacterMovement = GetCharacterMovementComponent())
	{
		CharacterMovement->MaxWalkSpeed = _WalkSpeed;
		CharacterMovement->MaxWalkSpeedCrouched = _WalkSpeed / 2.0f;
	}
}
