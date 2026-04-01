// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Widgets/Gameplay/Window/PlayerInventoryWindowWidget.h"

#include "Libraries/UIHelpers.h"
#include "Characters/BaseCharacter.h"
#include "Core/Components/StorageComponent.h"
#include "Characters/Components/InventoryComponent.h"
#include "InputCoreTypes.h"

void UPlayerInventoryWindowWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlayerInventoryWindowWidget::SetupInventoryWindow(UStorageComponent* InStorage, const EInventorySlotType InSlotType)
{
	LinkedStorage = InStorage;
	WindowSlotType = InSlotType;
	UpdateInventorySlots();
}

FReply UPlayerInventoryWindowWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry,
                                                             const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		if (const ABaseCharacter* Player = UUIHelpers::GetBasePlayerFromWidget(this); Player && Player->InventoryComponent)
		{
			Player->InventoryComponent->OnInventoryWindowClickedFor(LinkedStorage);
		}

		return FReply::Handled();
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UPlayerInventoryWindowWidget::UpdateInventorySlots()
{
	ClearInventorySlots();

	if (!LinkedStorage || !InventorySlotsBox || !InventorySlotClass)
	{
		return;
	}
	
	const TArray<FItem>& ItemList = LinkedStorage->Storage;
	for (int32 I = 0; I < ItemList.Num(); I++)
	{
		UBasePlayerInventorySlot* NewSlot = CreateWidget<UBasePlayerInventorySlot>(this, InventorySlotClass);
		if (!NewSlot) continue;

		NewSlot->SetSlotImage(ItemList[I].ItemIcon.LoadSynchronous());
		NewSlot->InventorySlotIndex = I;
		NewSlot->InventorySlotType = WindowSlotType;
		NewSlot->LinkedStorage = LinkedStorage;
		InventorySlotsBox->AddChild(NewSlot);
		InventorySlotWidgets.Add(NewSlot);
	}
}

void UPlayerInventoryWindowWidget::ClearInventorySlots()
{
	if (InventorySlotsBox)
	{
		InventorySlotsBox->ClearChildren();
	}

	for (UBasePlayerInventorySlot* Slot : InventorySlotWidgets)
	{
		if (Slot)
		{
			Slot->RemoveFromParent();
		}
	}
	InventorySlotWidgets.Empty();
}
