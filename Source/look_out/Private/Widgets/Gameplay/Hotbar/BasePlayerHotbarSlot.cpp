// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Widgets/Gameplay/Hotbar/BasePlayerHotbarSlot.h"

void UBasePlayerHotbarSlot::SetSlotImage_Implementation(UTexture2D* Texture)
{
	Sprite->SetBrushFromTexture(Texture);
}

void UBasePlayerHotbarSlot::SetActive_Implementation(bool bIsActive)
{
	ColorFill->SetColorAndOpacity(bIsActive ? ActiveColor : InActiveColor);
}