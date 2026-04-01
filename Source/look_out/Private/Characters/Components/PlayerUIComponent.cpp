// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Characters/Components/PlayerUIComponent.h"

#include "Characters/BaseCharacter.h"
#include "Components/NamedSlot.h"
#include "Misc/LogCategories.h"

// Sets default values for this component's properties
UPlayerUIComponent::UPlayerUIComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UPlayerUIComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
	if (!OwnerCharacter)
	{
		return;
	}

	if (APlayerController* PC = Cast<APlayerController>(OwnerCharacter->GetController()); PC && PlayerUIClass)
	{
		PlayerUI = CreateWidget<UBasePlayerUI>(PC, PlayerUIClass);
		if (PlayerUI)
		{
			PlayerUI->AddToViewport();
			UE_LOG(LogUI, Warning, TEXT("Player UI created"));
		}
		else
		{
			UE_LOG(LogUI, Warning, TEXT("Failed to setup player UI"));
		}
	}
}


// Called every frame
void UPlayerUIComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerUIComponent::CloseInventoryWindows()
{
	if (SoloInventoryWindow)
	{
		SoloInventoryWindow->RemoveFromParent();
		SoloInventoryWindow = nullptr;
	}

	if (AdditionalInventoryWindow)
	{
		AdditionalInventoryWindow->RemoveFromParent();
		AdditionalInventoryWindow = nullptr;
	}

	OpenedAdditionalInventory = nullptr;
	bIsInventoryWindowOpen = false;
}

void UPlayerUIComponent::ApplyInventoryInputMode(const bool bInventoryOpened) const
{
	if (!OwnerCharacter)
	{
		return;
	}

	APlayerController* PC = Cast<APlayerController>(OwnerCharacter->GetController());
	if (!PC)
	{
		return;
	}

	PC->SetIgnoreLookInput(bInventoryOpened);

	if (bInventoryOpened)
	{
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		PC->SetInputMode(InputMode);
	}
	else
	{
		PC->SetInputMode(FInputModeGameOnly());
	}

	PC->SetShowMouseCursor(bInventoryOpened);

	if (PlayerUI)
	{
		PlayerUI->SetInterfaceOpenState(bInventoryOpened);
	}
}

UBasePlayerWindowWidget* UPlayerUIComponent::CreateInventoryWindow(
	UStorageComponent* StorageToDisplay,
	UNamedSlot* TargetSlot,
	const FText& WindowTitle,
	const EInventorySlotType SlotType,
	const FVector2D& InitialOffset
) const
{
	if (!OwnerCharacter || !TargetSlot || !PlayerWindowClass || !InventoryWindowClass || !StorageToDisplay)
	{
		return nullptr;
	}

	APlayerController* PC = Cast<APlayerController>(OwnerCharacter->GetController());
	if (!PC)
	{
		return nullptr;
	}

	UBasePlayerWindowWidget* Window = CreateWidget<UBasePlayerWindowWidget>(PC, PlayerWindowClass);
	UPlayerInventoryWindowWidget* InventoryWidget = CreateWidget<UPlayerInventoryWindowWidget>(PC, InventoryWindowClass);
	if (!Window || !InventoryWidget)
	{
		return nullptr;
	}

	if (!Window->TitleTextBlock || !Window->CloseButton || !Window->BodySlot)
	{
		return nullptr;
	}

	Window->TitleTextBlock->SetText(WindowTitle);
	Window->CloseButton->SetVisibility(ESlateVisibility::Hidden);
	Window->SetRenderTranslation(InitialOffset);
	InventoryWidget->SetupInventoryWindow(StorageToDisplay, SlotType);
	Window->BodySlot->AddChild(InventoryWidget);
	TargetSlot->AddChild(Window);

	return Window;
}

void UPlayerUIComponent::ToggleInventoryWindow(UStorageComponent* AdditionalInventory)
{
	if (!PlayerWindowClass || !InventoryWindowClass || !OwnerCharacter || !PlayerUI)
	{
		return;
	}

	UInventoryComponent* OwnerInventory = OwnerCharacter->FindComponentByClass<UInventoryComponent>();
	if (!OwnerInventory)
	{
		return;
	}

	const bool bRequestedStorageView = AdditionalInventory != nullptr;
	const bool bSameLayoutAlreadyOpen = bIsInventoryWindowOpen &&
		OpenedAdditionalInventory == AdditionalInventory;

	if (bSameLayoutAlreadyOpen || (bIsInventoryWindowOpen && !bRequestedStorageView))
	{
		CloseInventoryWindows();
		ApplyInventoryInputMode(false);
		return;
	}

	CloseInventoryWindows();

	if (bRequestedStorageView)
	{
		SoloInventoryWindow = CreateInventoryWindow(
			OwnerInventory,
			PlayerUI->InventorySlot1,
			FText::FromString(TEXT("Inventory")),
			EInventorySlotType::PlayerInventory,
			FVector2D(-80.0f, 0.0f)
		);

		AdditionalInventoryWindow = CreateInventoryWindow(
			AdditionalInventory,
			PlayerUI->InventorySlot2,
			FText::FromString(TEXT("Container")),
			EInventorySlotType::ExternalInventory,
			FVector2D(80.0f, 0.0f)
		);

		OpenedAdditionalInventory = AdditionalInventory;
	}
	else
	{
		SoloInventoryWindow = CreateInventoryWindow(
			OwnerInventory,
			PlayerUI->SoloInventorySlot,
			FText::FromString(TEXT("Inventory")),
			EInventorySlotType::PlayerInventory
		);
	}

	bIsInventoryWindowOpen = SoloInventoryWindow != nullptr || AdditionalInventoryWindow != nullptr;
	ApplyInventoryInputMode(bIsInventoryWindowOpen);
}
