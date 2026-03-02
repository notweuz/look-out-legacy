// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Widgets/Gameplay/Hotbar/BasePlayerHotbar.h"

void UBasePlayerHotbar::NativeConstruct()
{
	Super::NativeConstruct();

	Slots.Empty();
	for (int32 i = 1; i <= 10; i++)
	{
		FName SlotName = *FString::Printf(TEXT("SlotTexture%d"), i);
		if (UWidget* Found = GetWidgetFromName(SlotName))
		{
			Slots.Add(Cast<UBasePlayerInventorySlot>(Found));
		}
	}
}

void UBasePlayerHotbar::UpdateHotbarSlots_Implementation(TArray<FItem>& ItemList)
{
	for (int32 i = 0; i < Slots.Num(); i++)
	{
		if (Slots[i])
		{
			Slots[i]->SetSlotImage(ItemList.IsValidIndex(i) ? ItemList[i].ItemIcon.LoadSynchronous() : nullptr);
		}
	}
}
