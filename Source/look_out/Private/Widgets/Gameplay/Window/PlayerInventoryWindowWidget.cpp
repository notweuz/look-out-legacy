// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Widgets/Gameplay/Window/PlayerInventoryWindowWidget.h"

void UPlayerInventoryWindowWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlayerInventoryWindowWidget::UpdateInventorySlots(TArray<FItem>& ItemList)
{
	ClearInventorySlots();
	
	for (const FItem& Item : ItemList)
	{
		if (!InventorySlotClass) continue;

		UBasePlayerInventorySlot* NewSlot = CreateWidget<UBasePlayerInventorySlot>(this, InventorySlotClass);
		if (!NewSlot) continue;

		NewSlot->SetSlotImage(Item.ItemIcon.Get());
		InventorySlotsBox->AddChild(NewSlot);
		InventorySlotWidgets.Add(NewSlot);
	}
}

void UPlayerInventoryWindowWidget::ClearInventorySlots()
{
	for (UBasePlayerInventorySlot* Slot : InventorySlotWidgets)
	{
		if (Slot)
		{
			Slot->RemoveFromParent();
		}
	}
	InventorySlotWidgets.Empty();
}


