// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#include "Widgets/Gameplay/Hotbar/BasePlayerInventorySlot.h"
#include "Components/SizeBox.h"

void UBasePlayerInventorySlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (SlotSizeBox)
	{
		SlotSizeBox->SetWidthOverride(SlotSize);
		SlotSizeBox->SetHeightOverride(SlotSize);
	}

	ClearSlot();
}

void UBasePlayerInventorySlot::SetSlotImage_Implementation(UTexture2D* Texture)
{
	if (!Sprite) return;

	if (Texture)
	{
		Sprite->SetBrushFromTexture(Texture);
		Sprite->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Sprite->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UBasePlayerInventorySlot::SetActive_Implementation(bool bIsActive)
{
	if (ColorFill)
	{
		ColorFill->SetColorAndOpacity(bIsActive ? ActiveColor : InActiveColor);
	}

	if (SpriteBorder)
	{
		SpriteBorder->SetBrushColor(bIsActive ? ActiveColor : FLinearColor::Transparent);
	}
}

void UBasePlayerInventorySlot::SetItemCount_Implementation(int32 Count)
{
	if (!ItemCountText) return;

	if (Count > 1)
	{
		ItemCountText->SetVisibility(ESlateVisibility::Visible);
		ItemCountText->SetText(FText::AsNumber(Count));
	}
	else
	{
		ItemCountText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UBasePlayerInventorySlot::SetSlotIndex_Implementation(int32 Index)
{
	if (SlotNumberText)
	{
		SlotNumberText->SetText(FText::AsNumber(Index + 1));
	}
}

void UBasePlayerInventorySlot::ClearSlot_Implementation()
{
	if (Sprite)
	{
		Sprite->SetVisibility(ESlateVisibility::Hidden);
	}

	if (ItemCountText)
	{
		ItemCountText->SetVisibility(ESlateVisibility::Hidden);
	}

	if (ColorFill)
	{
		ColorFill->SetColorAndOpacity(EmptySlotColor);
	}

	if (SpriteBorder)
	{
		SpriteBorder->SetBrushColor(FLinearColor::Transparent);
	}
}