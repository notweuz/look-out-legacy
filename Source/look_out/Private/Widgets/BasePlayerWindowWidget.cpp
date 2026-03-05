// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Widgets/BasePlayerWindowWidget.h"

#include "Components/NamedSlot.h"

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
