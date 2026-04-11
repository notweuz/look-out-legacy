// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Widgets/Menus/Content/Save/BaseSaveEntryWidget.h"

#include "Core/Save/SaveManager.h"
#include "Kismet/GameplayStatics.h"

void UBaseSaveEntryWidget::OnDeleteButtonPressed()
{
	USaveManager* SaveManager = GetGameInstance()->GetSubsystem<USaveManager>();
	if (!SaveManager) return;
	
	SaveManager->DeleteSave(SaveName);
}

void UBaseSaveEntryWidget::OnLoadButtonPressed()
{
	USaveManager* SaveManager = GetGameInstance()->GetSubsystem<USaveManager>();
	SaveManager->ActiveSlotName = SaveName;
	UGameplayStatics::OpenLevel(this, TEXT("MainMap"));
}

void UBaseSaveEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	DeleteButton->OnClicked.AddDynamic(this, &UBaseSaveEntryWidget::OnDeleteButtonPressed);
	LoadButton->OnClicked.AddDynamic(this, &UBaseSaveEntryWidget::OnLoadButtonPressed);
}

void UBaseSaveEntryWidget::NativeDestruct()
{
	Super::NativeDestruct();
	
	DeleteButton->OnClicked.RemoveAll(this);
	LoadButton->OnClicked.RemoveAll(this);
}
