// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Characters/Components/PlayerInputsComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Characters/BaseCharacter.h"
#include "Characters/Components/GrabbingComponent.h"
#include "Characters/Components/MovementComponentExtended.h"
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

	if (const APlayerController* PC = Cast<APlayerController>(OwnerCharacter->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
				PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC_BaseMapping, 0);
		}
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
		EIC->BindAction(IA_Throw, ETriggerEvent::Completed, this, &UPlayerInputsComponent::Input_ThrowTriggered);
		EIC->BindAction(IA_Interact, ETriggerEvent::Completed, this, &UPlayerInputsComponent::Input_InteractTriggered);
		EIC->BindAction(IA_Inventory, ETriggerEvent::Completed, this,
		                &UPlayerInputsComponent::Input_InventoryTriggered);
		EIC->BindAction(IA_RMB, ETriggerEvent::Completed, this, &UPlayerInputsComponent::Input_RMBTriggered);
		EIC->BindAction(IA_LMB, ETriggerEvent::Completed, this, &UPlayerInputsComponent::Input_LMBTriggered);
	}
}

bool UPlayerInputsComponent::IsActionHeld(const UInputAction* Action) const
{
	if (!OwnerCharacter) return false;

	const APlayerController* PC = Cast<APlayerController>(OwnerCharacter->GetController());
	if (!PC) return false;

	const UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	if (!Subsystem) return false;

	return Subsystem->GetPlayerInput()->GetActionValue(Action).Get<bool>();
}

void UPlayerInputsComponent::Input_MoveForward(const FInputActionValue& Value)
{
	OwnerCharacter->MovementComponentExtended->MoveForward(Value.Get<float>());
}

void UPlayerInputsComponent::Input_MoveSideways(const FInputActionValue& Value)
{
	OwnerCharacter->MovementComponentExtended->MoveRight(Value.Get<float>());
}

void UPlayerInputsComponent::Input_Look(const FInputActionValue& Value)
{
	const FVector2D Axis = Value.Get<FVector2D>();
	OwnerCharacter->MovementComponentExtended->Look(Axis.X, Axis.Y);
}

void UPlayerInputsComponent::Input_JumpStarted()
{
	OwnerCharacter->MovementComponentExtended->JumpAction();
}

void UPlayerInputsComponent::Input_CrouchStarted()
{
	OwnerCharacter->MovementComponentExtended->DoCrouch(true);
}

void UPlayerInputsComponent::Input_CrouchEnded()
{
	OwnerCharacter->MovementComponentExtended->DoCrouch(false);
}

void UPlayerInputsComponent::Input_SprintStarted()
{
	OwnerCharacter->MovementComponentExtended->ToggleSprint(true);
}

void UPlayerInputsComponent::Input_SprintEnded()
{
	OwnerCharacter->MovementComponentExtended->ToggleSprint(false);
}

void UPlayerInputsComponent::Input_ThrowTriggered()
{
	OwnerCharacter->GrabbingComponent->ThrowObject();
}

void UPlayerInputsComponent::Input_InteractTriggered()
{
	OwnerCharacter->PlayerInteractionComponent->Interact();
}

void UPlayerInputsComponent::Input_InventoryTriggered()
{
	OwnerCharacter->PlayerUIComponent->ToggleInventoryWindow(nullptr);
}

void UPlayerInputsComponent::Input_RMBTriggered()
{
	OwnerCharacter->InventoryComponent->CollectItem();
}

void UPlayerInputsComponent::Input_LMBTriggered()
{
	OwnerCharacter->GrabbingComponent->ToggleGrab(!OwnerCharacter->GrabbingComponent->IsGrabbingObject);
}

void UPlayerInputsComponent::Input_Scroll(const FInputActionValue& Value)
{
	const float Delta = Value.Get<float>();
	if (OwnerCharacter->GrabbingComponent->IsGrabbingObject)
	{
		OwnerCharacter->GrabbingComponent->ChangeGrabDistance(Delta);
	} else
	{
		OwnerCharacter->InventoryComponent->ScrollActiveItem(Delta);
	}
}
