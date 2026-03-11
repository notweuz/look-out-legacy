// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Widgets/Gameplay/Window/PlayerInventoryWindowWidget.h"

#include "Libraries/UIHelpers.h"
#include "Characters/BaseCharacter.h"
#include "Characters/Components/InventoryComponent.h"
#include "InputCoreTypes.h"

void UPlayerInventoryWindowWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

FReply UPlayerInventoryWindowWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry,
                                                             const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		if (const ABaseCharacter* Player = UUIHelpers::GetBasePlayerFromWidget(this); Player && Player->InventoryComponent)
		{
			Player->InventoryComponent->OnInventoryWindowClicked();
		}

		return FReply::Handled();
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
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
		NewSlot->InventorySlotType = PlayerInventory;
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
