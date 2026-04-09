// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Characters/Components/PlayerInputsComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Characters/BaseCharacter.h"
#include "Characters/Components/PlayerGrabComponent.h"
#include "Characters/Components/PlayerInventoryComponent.h"
#include "Characters/Components/PlayerMovementComponent.h"
#include "Characters/Components/PlayerSideInteractionComponent.h"

// Sets default values for this component's properties
UPlayerInputsComponent::UPlayerInputsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UPlayerInputsComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
	if (!OwnerCharacter)
	{
		return;
	}

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = GetInputSubsystem(); Subsystem && IMC_BaseMapping)
	{
		Subsystem->AddMappingContext(IMC_BaseMapping, 0);
	}
}


// Called every frame
void UPlayerInputsComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerInputsComponent::SetupPlayerInput(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EIC->BindAction(IA_MoveForward, ETriggerEvent::Triggered, this, &UPlayerInputsComponent::Input_MoveForward);
		EIC->BindAction(IA_MoveSideways, ETriggerEvent::Triggered, this, &UPlayerInputsComponent::Input_MoveSideways);
		EIC->BindAction(IA_Look, ETriggerEvent::Triggered, this, &UPlayerInputsComponent::Input_Look);
		EIC->BindAction(IA_Jump, ETriggerEvent::Started, this, &UPlayerInputsComponent::Input_JumpStarted);
		EIC->BindAction(IA_Crouch, ETriggerEvent::Started, this, &UPlayerInputsComponent::Input_CrouchStarted);
		EIC->BindAction(IA_Crouch, ETriggerEvent::Completed, this, &UPlayerInputsComponent::Input_CrouchEnded);
		EIC->BindAction(IA_Sprint, ETriggerEvent::Started, this, &UPlayerInputsComponent::Input_SprintStarted);
		EIC->BindAction(IA_Sprint, ETriggerEvent::Completed, this, &UPlayerInputsComponent::Input_SprintEnded);
		EIC->BindAction(IA_Throw, ETriggerEvent::Started, this, &UPlayerInputsComponent::Input_ThrowTriggered);
		EIC->BindAction(IA_Interact, ETriggerEvent::Started, this, &UPlayerInputsComponent::Input_InteractTriggered);
		EIC->BindAction(IA_Inventory, ETriggerEvent::Started, this,
		                &UPlayerInputsComponent::Input_InventoryTriggered);
		EIC->BindAction(IA_RMB, ETriggerEvent::Started, this, &UPlayerInputsComponent::Input_RMBTriggered);
		EIC->BindAction(IA_LMB, ETriggerEvent::Started, this, &UPlayerInputsComponent::Input_LMBTriggered);
		EIC->BindAction(IA_Scroll, ETriggerEvent::Triggered, this, &UPlayerInputsComponent::Input_Scroll);
	}
}

APlayerController* UPlayerInputsComponent::GetPlayerController() const
{
	return OwnerCharacter ? Cast<APlayerController>(OwnerCharacter->GetController()) : nullptr;
}

UEnhancedInputLocalPlayerSubsystem* UPlayerInputsComponent::GetInputSubsystem() const
{
	const APlayerController* PC = GetPlayerController();
	if (!PC)
	{
		return nullptr;
	}

	return ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
}

bool UPlayerInputsComponent::CanProcessGameplayInput() const
{
	return OwnerCharacter;
}

bool UPlayerInputsComponent::IsActionHeld(const UInputAction* Action) const
{
	if (!Action)
	{
		return false;
	}

	const UEnhancedInputLocalPlayerSubsystem* Subsystem = GetInputSubsystem();
	if (!Subsystem || !Subsystem->GetPlayerInput())
	{
		return false;
	}

	return Subsystem->GetPlayerInput()->GetActionValue(Action).Get<bool>();
}

bool UPlayerInputsComponent::IsButtonHeld(const FKey Key) const
{
	const UEnhancedInputLocalPlayerSubsystem* Subsystem = GetInputSubsystem();
	if (!Subsystem)
	{
		return false;
	}

	const UPlayerInput* PlayerInput = Subsystem->GetPlayerInput();
	if (!PlayerInput) return false;

	const FKeyState* KeyState = PlayerInput->GetKeyState(Key);
	if (!KeyState) return false;

	return KeyState->bDown;
}

void UPlayerInputsComponent::Input_MoveForward(const FInputActionValue& Value)
{
	if (OwnerCharacter && OwnerCharacter->PlayerMovementController)
	{
		OwnerCharacter->PlayerMovementController->MoveForward(Value.Get<float>());
	}
}

void UPlayerInputsComponent::Input_MoveSideways(const FInputActionValue& Value)
{
	if (OwnerCharacter && OwnerCharacter->PlayerMovementController)
	{
		OwnerCharacter->PlayerMovementController->MoveRight(Value.Get<float>());
	}
}

void UPlayerInputsComponent::Input_Look(const FInputActionValue& Value)
{
	if (!OwnerCharacter || !OwnerCharacter->PlayerMovementController)
	{
		return;
	}

	const FVector2D Axis = Value.Get<FVector2D>();
	OwnerCharacter->PlayerMovementController->Look(Axis.X, Axis.Y);
}

void UPlayerInputsComponent::Input_JumpStarted()
{
	if (!CanProcessGameplayInput() || !OwnerCharacter->PlayerMovementController) return;
	OwnerCharacter->PlayerMovementController->JumpAction();
}

void UPlayerInputsComponent::Input_CrouchStarted()
{
	if (!CanProcessGameplayInput() || !OwnerCharacter->PlayerMovementController) return;
	OwnerCharacter->PlayerMovementController->DoCrouch(true);
}

void UPlayerInputsComponent::Input_CrouchEnded()
{
	if (!CanProcessGameplayInput() || !OwnerCharacter->PlayerMovementController) return;
	OwnerCharacter->PlayerMovementController->DoCrouch(false);
}

void UPlayerInputsComponent::Input_SprintStarted()
{
	if (!CanProcessGameplayInput() || !OwnerCharacter->PlayerMovementController) return;
	OwnerCharacter->PlayerMovementController->ToggleSprint(true);
}

void UPlayerInputsComponent::Input_SprintEnded()
{
	if (!CanProcessGameplayInput() || !OwnerCharacter->PlayerMovementController) return;
	OwnerCharacter->PlayerMovementController->ToggleSprint(false);
}

void UPlayerInputsComponent::Input_ThrowTriggered()
{
	if (!CanProcessGameplayInput() || !OwnerCharacter->PlayerGrabComponent) return;
	OwnerCharacter->PlayerGrabComponent->ThrowObject();
}

void UPlayerInputsComponent::Input_InteractTriggered()
{
	if (!CanProcessGameplayInput() || !OwnerCharacter->PlayerInteractionComponent) return;
	OwnerCharacter->PlayerInteractionComponent->Interact();
}

void UPlayerInputsComponent::Input_InventoryTriggered()
{
	
}

void UPlayerInputsComponent::Input_RMBTriggered()
{
	if (!CanProcessGameplayInput()) return;
	OwnerCharacter->PlayerInventoryComponent->Collect();
}

void UPlayerInputsComponent::Input_LMBTriggered()
{
	if (!CanProcessGameplayInput() || !OwnerCharacter || !OwnerCharacter->PlayerGrabComponent) return;
	OwnerCharacter->PlayerGrabComponent->ToggleGrab(!OwnerCharacter->PlayerGrabComponent->IsGrabbingObject);
}

void UPlayerInputsComponent::Input_Scroll(const FInputActionValue& Value)
{
	if (!CanProcessGameplayInput() || !OwnerCharacter || !OwnerCharacter->PlayerGrabComponent) return;

	const float Delta = Value.Get<float>();
	if (OwnerCharacter->PlayerGrabComponent->IsGrabbingObject)
	{
		OwnerCharacter->PlayerGrabComponent->ChangeGrabDistance(Delta);
	}
}
