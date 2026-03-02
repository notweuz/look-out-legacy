// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Widgets/Gameplay/Hotbar/BasePlayerHotbar.h"

void UBasePlayerHotbar::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBasePlayerHotbar::UpdateHotbarSlots_Implementation(TArray<FItem>& ItemList)
{
	// some shit code bcz why not
	Hotbar = ItemList;
	Slot1->SetSlotImage(ItemList.IsValidIndex(0) ? ItemList[0].ItemIcon.Get() : nullptr);
	Slot2->SetSlotImage(ItemList.IsValidIndex(1) ? ItemList[1].ItemIcon.Get() : nullptr);
	Slot3->SetSlotImage(ItemList.IsValidIndex(2) ? ItemList[2].ItemIcon.Get() : nullptr);
	Slot4->SetSlotImage(ItemList.IsValidIndex(3) ? ItemList[3].ItemIcon.Get() : nullptr);
	Slot5->SetSlotImage(ItemList.IsValidIndex(4) ? ItemList[4].ItemIcon.Get() : nullptr);
	Slot6->SetSlotImage(ItemList.IsValidIndex(5) ? ItemList[5].ItemIcon.Get() : nullptr);
	Slot7->SetSlotImage(ItemList.IsValidIndex(6) ? ItemList[6].ItemIcon.Get() : nullptr);
	Slot8->SetSlotImage(ItemList.IsValidIndex(7) ? ItemList[7].ItemIcon.Get() : nullptr);
	Slot9->SetSlotImage(ItemList.IsValidIndex(8) ? ItemList[8].ItemIcon.Get() : nullptr);
	Slot10->SetSlotImage(ItemList.IsValidIndex(9) ? ItemList[9].ItemIcon.Get() : nullptr);
	// have a nice day
}
