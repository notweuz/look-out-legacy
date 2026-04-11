// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Widgets/Menus/BaseMenuWidget.h"

#include "Components/NamedSlot.h"

void UBaseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CloseButton->OnClicked.AddDynamic(this, &UBaseMenuWidget::Close);
}

void UBaseMenuWidget::SetBodyWidget(UWidget* Widget)
{
	BodySlot->SetContent(Widget);
}

void UBaseMenuWidget::Close()
{
	RemoveFromParent();
}