// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Widgets/Gameplay/Hotbar/BasePlayerHotbar.h"

void UBasePlayerHotbar::NativeConstruct()
{
	Super::NativeConstruct();
	Hotbar.SetNum(10);
}

void UBasePlayerHotbar::UpdateHotbarSlots_Implementation(TArray<FItem>& ItemList)
{
	Hotbar.SetNum(ItemList.Num());
	
	// ik this is looks like shitcode, but I don't want to do it
	if (SlotTexture1) SlotTexture1->SetSlotImage(ItemList.IsValidIndex(0) ? ItemList[0].ItemIcon.LoadSynchronous() : nullptr);
	if (SlotTexture2) SlotTexture2->SetSlotImage(ItemList.IsValidIndex(1) ? ItemList[1].ItemIcon.LoadSynchronous() : nullptr);
	if (SlotTexture3) SlotTexture3->SetSlotImage(ItemList.IsValidIndex(2) ? ItemList[2].ItemIcon.LoadSynchronous() : nullptr);
	if (SlotTexture4) SlotTexture4->SetSlotImage(ItemList.IsValidIndex(3) ? ItemList[3].ItemIcon.LoadSynchronous() : nullptr);
	if (SlotTexture5) SlotTexture5->SetSlotImage(ItemList.IsValidIndex(4) ? ItemList[4].ItemIcon.LoadSynchronous() : nullptr);
	if (SlotTexture6) SlotTexture6->SetSlotImage(ItemList.IsValidIndex(5) ? ItemList[5].ItemIcon.LoadSynchronous() : nullptr);
	if (SlotTexture7) SlotTexture7->SetSlotImage(ItemList.IsValidIndex(6) ? ItemList[6].ItemIcon.LoadSynchronous() : nullptr);
	if (SlotTexture8) SlotTexture8->SetSlotImage(ItemList.IsValidIndex(7) ? ItemList[7].ItemIcon.LoadSynchronous() : nullptr);
	if (SlotTexture9) SlotTexture9->SetSlotImage(ItemList.IsValidIndex(8) ? ItemList[8].ItemIcon.LoadSynchronous() : nullptr);
	if (SlotTexture10) SlotTexture10->SetSlotImage(ItemList.IsValidIndex(9) ? ItemList[9].ItemIcon.LoadSynchronous() : nullptr);
	// have a nice day
}
