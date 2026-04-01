// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Widgets/BasePlayerWindowWidget.h"

#include "Components/NamedSlot.h"
#include "InputCoreTypes.h"

void UBasePlayerWindowWidget::OnCloseClicked_Implementation()
{
	RemoveFromParent();
}

void UBasePlayerWindowWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (CloseButton)
	{
		CloseButton->OnClicked.Clear();
		CloseButton->OnClicked.AddDynamic(this, &UBasePlayerWindowWidget::OnCloseClicked);
	}
}

FReply UBasePlayerWindowWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bIsDraggingWindow = true;
		DragStartMousePosition = InMouseEvent.GetScreenSpacePosition();
		DragStartWindowTranslation = GetRenderTransform().Translation;

		return FReply::Handled().CaptureMouse(TakeWidget());
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UBasePlayerWindowWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bIsDraggingWindow && InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bIsDraggingWindow = false;
		return FReply::Handled().ReleaseMouseCapture();
	}

	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

FReply UBasePlayerWindowWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bIsDraggingWindow)
	{
		const FVector2D MouseDelta = InMouseEvent.GetScreenSpacePosition() - DragStartMousePosition;
		SetRenderTranslation(DragStartWindowTranslation + MouseDelta);
		return FReply::Handled();
	}

	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}
