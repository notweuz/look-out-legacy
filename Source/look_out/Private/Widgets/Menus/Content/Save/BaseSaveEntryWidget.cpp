// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Widgets/Menus/Content/Save/BaseSaveEntryWidget.h"

#include "Core/Save/SaveManager.h"
#include "Kismet/GameplayStatics.h"

void UBaseSaveEntryWidget::OnSelectButtonClicked()
{
	OnSaveSelected.Broadcast(this);
}

void UBaseSaveEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	SelectButton->OnClicked.AddDynamic(this, &UBaseSaveEntryWidget::OnSelectButtonClicked);
}

void UBaseSaveEntryWidget::NativeDestruct()
{
	Super::NativeDestruct();
	
	SelectButton->OnClicked.RemoveAll(this);
}
