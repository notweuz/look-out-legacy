// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Widgets/Gameplay/BasePlayerUI.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/NamedSlot.h"
#include "Libraries/UIHelpers.h"

void UBasePlayerUI::UpdateTempItemSpriteImage()
{
	ABaseCharacter* Player = UUIHelpers::GetBasePlayerFromWidget(this);
	if (!Player) return;
	
	if (!Player->InventoryComponent->TempItem.ItemIcon.IsNull()) 
	{
		TempItemSpriteImage->SetBrushFromTexture(Player->InventoryComponent->TempItem.ItemIcon.Get());
		TempItemSpriteImage->SetVisibility(ESlateVisibility::HitTestInvisible);
	} else
	{
		TempItemSpriteImage->SetBrushFromTexture(nullptr);
		TempItemSpriteImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UBasePlayerUI::UpdateEntireInventory()
{
	UpdateTempItemSpriteImage();
	Hotbar->UpdateHotbarSlots();
	for (const UNamedSlot* Slot : { SoloInventorySlot, InventorySlot1, InventorySlot2 })
	{
		if (!Slot) continue;

		UWidget* Child = Slot->GetContent();
		if (!Child) continue;

		if (const UBasePlayerWindowWidget* Window = Cast<UBasePlayerWindowWidget>(Child))
		{
			if (UPlayerInventoryWindowWidget* InvWidget = Cast<UPlayerInventoryWindowWidget>(
				Window->BodySlot->GetContent()))
			{
				InvWidget->UpdateInventorySlots();
			}
		}
	}
}

void UBasePlayerUI::NativeConstruct()
{
	Super::NativeConstruct();
	UpdateTempItemSpriteImage();
}

void UBasePlayerUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!TempItemSpriteImage || !FloatingWidgetsCanvasPanel) return;
	if (!TempItemSpriteImage->GetBrush().GetResourceObject()) return;

	const FVector2D MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

	if (UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(TempItemSpriteImage->Slot))
	{
		Slot->SetAlignment(FVector2D(0.5f, 0.5f));
		Slot->SetPosition(MousePos);
	}
}