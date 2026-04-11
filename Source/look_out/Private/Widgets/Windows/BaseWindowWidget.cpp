// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Widgets/Windows/BaseWindowWidget.h"

#include "Components/NamedSlot.h"

void UBaseWindowWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CloseButton->OnClicked.AddDynamic(this, &UBaseWindowWidget::Close);
	CloseButton->SetVisibility(ESlateVisibility::Collapsed);
}

void UBaseWindowWidget::SetTitle(const FText& Title)
{
	TitleTextBlock->SetText(Title);
	TitleTextBlock->SetVisibility(ESlateVisibility::Visible);
}

void UBaseWindowWidget::SetBodyWidget(UWidget* Widget)
{
	BodySlot->SetContent(Widget);
}

void UBaseWindowWidget::SetCloseButtonVisibility(ESlateVisibility Visibility)
{
	CloseButton->SetVisibility(Visibility);
}

void UBaseWindowWidget::Close()
{
	RemoveFromParent();
}