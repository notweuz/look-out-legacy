// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Widgets/Gameplay/BasePlayerUI.h"

#include "Blueprint/WidgetTree.h"
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

void UBasePlayerUI::SetInterfaceOpenState(const bool bIsOpened)
{
	bIsAnyInterfaceOpened = bIsOpened;
	UpdateInventoryBackdrop();
}

void UBasePlayerUI::UpdateInventoryBackdrop()
{
	const ESlateVisibility Visibility = bIsAnyInterfaceOpened ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden;

	if (InventoryBackgroundBlur)
	{
		InventoryBackgroundBlur->SetVisibility(ESlateVisibility::Hidden);
	}

	if (InventoryDimBorder)
	{
		InventoryDimBorder->SetVisibility(Visibility);
	}
}

void UBasePlayerUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (WidgetTree)
	{
		UCanvasPanel* RootCanvas = Cast<UCanvasPanel>(WidgetTree->RootWidget);

		if (!RootCanvas)
		{
			UpdateTempItemSpriteImage();
			UpdateInventoryBackdrop();
			return;
		}

		if (!InventoryBackgroundBlur)
		{
			InventoryBackgroundBlur = WidgetTree->ConstructWidget<UBackgroundBlur>(UBackgroundBlur::StaticClass(),
				TEXT("InventoryBackgroundBlur"));
			InventoryBackgroundBlur->SetBlurStrength(0.0f);
			InventoryBackgroundBlur->SetVisibility(ESlateVisibility::Hidden);
			if (UCanvasPanelSlot* BlurSlot = RootCanvas->AddChildToCanvas(InventoryBackgroundBlur))
			{
				BlurSlot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
				BlurSlot->SetOffsets(FMargin(0.0f));
				BlurSlot->SetZOrder(-1000);
			}
		}

		if (!InventoryDimBorder)
		{
			InventoryDimBorder = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("InventoryDimBorder"));
			InventoryDimBorder->SetBrushColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.18f));
			InventoryDimBorder->SetVisibility(ESlateVisibility::Hidden);
			if (UCanvasPanelSlot* DimSlot = RootCanvas->AddChildToCanvas(InventoryDimBorder))
			{
				DimSlot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
				DimSlot->SetOffsets(FMargin(0.0f));
				DimSlot->SetZOrder(-999);
			}
		}
	}

	UpdateTempItemSpriteImage();
	UpdateInventoryBackdrop();
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
