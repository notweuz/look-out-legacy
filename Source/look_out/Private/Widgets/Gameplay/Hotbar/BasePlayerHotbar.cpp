// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Widgets/Gameplay/Hotbar/BasePlayerHotbar.h"

void UBasePlayerHotbar::NativeConstruct()
{
	Super::NativeConstruct();
	Slots = { Slot1, Slot2, Slot3, Slot4, Slot5, Slot6, Slot7, Slot8, Slot9, Slot10 };
}

void UBasePlayerHotbar::SetActiveHotbarSlot(int OldIndex, int NewIndex)
{
	if (Slots.IsValidIndex(OldIndex)) Slots[OldIndex]->SetActive(false);
	if (Slots.IsValidIndex(NewIndex)) Slots[NewIndex]->SetActive(true);
}

void UBasePlayerHotbar::UpdateHotbarSlots_Implementation(TArray<FItem>& ItemList)
{
	for (int I = 0; I < 10; I++)
	{
		Slots[I]->SetSlotImage(ItemList.IsValidIndex(I) ? ItemList[I].ItemIcon.Get() : nullptr);
		Slots[I]->InventorySlotIndex = I;
	}
}
