// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Widgets/Gameplay/BasePlayerUI.h"

#include "Components/CanvasPanelSlot.h"
#include "Libraries/UIHelpers.h"

void UBasePlayerUI::UpdateTempItemSpriteImage()
{
	ABaseCharacter* Player = UUIHelpers::GetBasePlayerFromWidget(this);
	if (!Player) return;
	
	if (!Player->InventoryComponent->TempItem.ItemIcon.IsNull()) 
	{
		TempItemSpriteImage->SetBrushFromTexture(Player->InventoryComponent->TempItem.ItemIcon.Get());
		TempItemSpriteImage->SetVisibility(ESlateVisibility::Visible);
	} else
	{
		TempItemSpriteImage->SetBrushFromTexture(nullptr);
		TempItemSpriteImage->SetVisibility(ESlateVisibility::Hidden);
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

	const APlayerController* PC = GetOwningPlayer();
	if (!PC || !TempItemSpriteImage || !FloatingWidgetsCanvasPanel) return;

	float MouseX, MouseY;
	PC->GetMousePosition(MouseX, MouseY);

	const FGeometry CanvasGeometry = FloatingWidgetsCanvasPanel->GetCachedGeometry();
	const FVector2D LocalPos = CanvasGeometry.AbsoluteToLocal(FVector2D(MouseX, MouseY));

	if (!TempItemSpriteImage->GetBrush().GetResourceObject()) return;
	if (UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(TempItemSpriteImage->Slot))
	{
		Slot->SetPosition(LocalPos);
	}
}