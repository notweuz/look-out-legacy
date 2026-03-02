// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#include "Widgets/Gameplay/Hotbar/BasePlayerInventorySlot.h"
#include "Components/SizeBox.h"

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

void UBasePlayerInventorySlot::SetActive_Implementation(const bool IsActive)
{
	bIsActive = IsActive;
	if (!bIsActive)
	{
		Sprite->SetVisibility(ESlateVisibility::Hidden);
	}
	StateChanged(IsActive);
}