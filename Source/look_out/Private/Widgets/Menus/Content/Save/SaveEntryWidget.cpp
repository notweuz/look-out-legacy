// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Widgets/Menus/Content/Save/SaveEntryWidget.h"

#include "Core/Save/SaveManager.h"
#include "Kismet/GameplayStatics.h"

void USaveEntryWidget::OnDeleteButtonPressed()
{
	USaveManager* SaveManager = GetGameInstance()->GetSubsystem<USaveManager>();
	if (!SaveManager) return;
	
	SaveManager->DeleteSave(SaveName);
}

void USaveEntryWidget::OnLoadButtonPressed()
{
	UGameplayStatics::OpenLevel(this, TEXT("MainMap"));
}

void USaveEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	DeleteButton->OnClicked.AddDynamic(this, &USaveEntryWidget::OnDeleteButtonPressed);
	LoadButton->OnClicked.AddDynamic(this, &USaveEntryWidget::OnLoadButtonPressed);
}
