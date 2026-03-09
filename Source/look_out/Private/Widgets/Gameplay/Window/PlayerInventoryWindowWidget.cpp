// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Widgets/Gameplay/Window/PlayerInventoryWindowWidget.h"

#include "Libraries/UIHelpers.h"

void UPlayerInventoryWindowWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlayerInventoryWindowWidget::UpdateInventorySlots()
{
	ClearInventorySlots();

	const ABaseCharacter* Player = UUIHelpers::GetBasePlayerFromWidget(this);
	if (!Player) return;
	
	TArray<FItem> ItemList = Player->InventoryComponent->Storage;
	for (int32 I = 0; I < ItemList.Num(); I++)
	{
		if (!InventorySlotClass) continue;

		UBasePlayerInventorySlot* NewSlot = CreateWidget<UBasePlayerInventorySlot>(this, InventorySlotClass);
		if (!NewSlot) continue;

		NewSlot->SetSlotImage(ItemList[I].ItemIcon.Get());
		NewSlot->InventorySlotIndex = I;
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
