// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Characters/Components/PlayerUIComponent.h"

#include "Characters/BaseCharacter.h"
#include "Components/NamedSlot.h"

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

	if (APlayerController* PC = Cast<APlayerController>(OwnerCharacter->GetController()); PC && PlayerUIClass)
	{
		PlayerUI = CreateWidget<UBasePlayerUI>(PC, PlayerUIClass);
		if (PlayerUI)
		{
			PlayerUI->AddToViewport();
			UE_LOG(LogTemp, Warning, TEXT("Player UI created"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to setup player UI"));
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

void UPlayerUIComponent::ToggleInventoryWindow(UInventoryComponent* AdditionalInventory)
{
	if (!PlayerWindowClass || !InventoryWindowClass || !OwnerCharacter || !PlayerUI)
	{
		return;
	}

	APlayerController* PC = Cast<APlayerController>(OwnerCharacter->GetController());
	if (!PC)
	{
		return;
	}

	if (bIsInventoryWindowOpen)
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

		bIsInventoryWindowOpen = false;
		PC->SetIgnoreLookInput(false);
		PC->SetInputMode(FInputModeGameOnly());
		PC->SetShowMouseCursor(false);
		OwnerCharacter->PlayerUIComponent->PlayerUI->bIsAnyInterfaceOpened = false;
		return;
	}

	UInventoryComponent* OwnerInventory = OwnerCharacter->FindComponentByClass<UInventoryComponent>();
	if (!OwnerInventory)
	{
		return;
	}

	if (AdditionalInventory)
	{
		UBasePlayerWindowWidget* Window1 = CreateWidget<UBasePlayerWindowWidget>(PC, PlayerWindowClass);
		if (UPlayerInventoryWindowWidget* InvWidget1 = CreateWidget<
			UPlayerInventoryWindowWidget>(PC, InventoryWindowClass); Window1 && InvWidget1 && PlayerUI->InventorySlot1)
		{
			Window1->TitleTextBlock->SetText(FText::FromString(TEXT("Inventory")));
			InvWidget1->UpdateInventorySlots();
			Window1->CloseButton->SetVisibility(ESlateVisibility::Hidden);
			Window1->BodySlot->AddChild(InvWidget1);
			PlayerUI->InventorySlot1->AddChild(Window1);
			SoloInventoryWindow = Window1;
		}

		UBasePlayerWindowWidget* Window2 = CreateWidget<UBasePlayerWindowWidget>(PC, PlayerWindowClass);
		if (UPlayerInventoryWindowWidget* InvWidget2 = CreateWidget<
			UPlayerInventoryWindowWidget>(PC, InventoryWindowClass); Window2 && InvWidget2 && PlayerUI->InventorySlot2)
		{
			Window2->TitleTextBlock->SetText(FText::FromString(TEXT("Container")));
			InvWidget2->UpdateInventorySlots();
			Window2->BodySlot->AddChild(InvWidget2);
			Window2->CloseButton->SetVisibility(ESlateVisibility::Hidden);
			PlayerUI->InventorySlot2->AddChild(Window2);
			AdditionalInventoryWindow = Window2;
		}
	}
	else
	{
		UBasePlayerWindowWidget* Window = CreateWidget<UBasePlayerWindowWidget>(PC, PlayerWindowClass);
		UPlayerInventoryWindowWidget* InvWidget = CreateWidget<UPlayerInventoryWindowWidget>(PC, InventoryWindowClass);
		if (Window && InvWidget && PlayerUI->SoloInventorySlot)
		{
			Window->TitleTextBlock->SetText(FText::FromString(TEXT("Inventory")));
			InvWidget->UpdateInventorySlots();
			Window->CloseButton->SetVisibility(ESlateVisibility::Hidden);
			Window->BodySlot->AddChild(InvWidget);
			PlayerUI->SoloInventorySlot->AddChild(Window);
			SoloInventoryWindow = Window;
		}
	}

	PC->SetIgnoreLookInput(true);
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	PC->SetInputMode(InputMode);
	PC->SetShowMouseCursor(true);
	OwnerCharacter->PlayerUIComponent->PlayerUI->bIsAnyInterfaceOpened = true;
	bIsInventoryWindowOpen = true;
}
