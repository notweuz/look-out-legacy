// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#include "Widgets/Gameplay/BasePlayerInventorySlot.h"
#include "Components/SizeBox.h"
#include "Libraries/UIHelpers.h"

void UBasePlayerInventorySlot::SetSlotImage_Implementation(UTexture2D* Texture)
{
	if (!Sprite)
	{
		return;
	}

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
	StateChanged(IsActive);
}

void UBasePlayerInventorySlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (!Sprite->GetBrush().GetResourceObject()) Sprite->SetVisibility(ESlateVisibility::Hidden);
}

FReply UBasePlayerInventorySlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (const ABaseCharacter* Player = UUIHelpers::GetBasePlayerFromWidget(this); Player != nullptr)
	{
		if (const FKey EffectingButton = InMouseEvent.GetEffectingButton(); EffectingButton == EKeys::LeftMouseButton ||
			EffectingButton == EKeys::RightMouseButton)
		{
			Player->InventoryComponent->OnSlotClicked(this, EffectingButton == EKeys::RightMouseButton);
		}
	}

	return FReply::Handled();
}
