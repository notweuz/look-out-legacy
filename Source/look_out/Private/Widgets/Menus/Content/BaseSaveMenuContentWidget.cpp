// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Widgets/Menus/Content/BaseSaveMenuContentWidget.h"

#include <string>

#include "Core/Save/SaveManager.h"

void UBaseSaveMenuContentWidget::OnSaveSlotsChanged(const TArray<FSaveSlotMeta>& Slots)
{
	for (UWidget* Widget : SlotsList->GetAllChildren())
	{
		if (UBaseSaveEntryWidget* Entry = Cast<UBaseSaveEntryWidget>(Widget))
		{
			Entry->OnSaveSelected.RemoveAll(this);
		}
	}
	
	SlotsList->ClearChildren();
	
	for (const FSaveSlotMeta& Slot : Slots)
	{
		UBaseSaveEntryWidget* SaveEntryWidget = CreateWidget<UBaseSaveEntryWidget>(GetWorld(), SaveEntryWidgetClass);
		SaveEntryWidget->SaveName = Slot.SlotName;
		SaveEntryWidget->SaveDateText->SetText(FText::FromString(Slot.SavedAt.ToString()));
		SaveEntryWidget->SaveNameText->SetText(FText::FromString(Slot.SlotName));
		
		if (const UGameSaveGame* SaveGame = GetGameInstance()->GetSubsystem<USaveManager>()->GetSave(Slot.SlotName))
		{
			SaveEntryWidget->DaysPassedText->SetText(FText::FromString(FString::FromInt(SaveGame->WorldState.Day)));
		} else
		{
			SaveEntryWidget->SaveDateText->SetText(FText::GetEmpty());
		}
		
		SaveEntryWidget->OnSaveSelected.AddDynamic(this, &UBaseSaveMenuContentWidget::OnSaveSelected);
		
		SlotsList->AddChild(SaveEntryWidget);
	}
}

void UBaseSaveMenuContentWidget::OnSaveSelected(UBaseSaveEntryWidget* SaveEntryWidget)
{
	if (SelectedSaveEntryWidget && SelectedSaveEntryWidget != SaveEntryWidget)
	{
		SelectedSaveEntryWidget->OnStateChanged(false);
	}
	
	SelectedSaveEntryWidget = SaveEntryWidget;
	if (SelectedSaveEntryWidget)
	{
		SelectedSaveEntryWidget->OnStateChanged(true);
		SelectedSaveName = SelectedSaveEntryWidget->SaveName;
	}
}

void UBaseSaveMenuContentWidget::OnCreateButtonClicked()
{
	USaveManager* SaveManager = GetGameInstance()->GetSubsystem<USaveManager>();
	if (!SaveManager) return;
	
	SaveManager->CreateSave(SlotName->GetText().ToString());
	OnSaveSlotsChanged(SaveManager->GetAllSlots());
}

void UBaseSaveMenuContentWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	CreateButton->OnClicked.AddDynamic(this, &UBaseSaveMenuContentWidget::OnCreateButtonClicked);
	
	USaveManager* SaveManager = GetGameInstance()->GetSubsystem<USaveManager>();
	if (!SaveManager) return;
	
	SaveManager->OnSaveSlotsChanged.AddUObject(
		this,
		&UBaseSaveMenuContentWidget::OnSaveSlotsChanged
	);
	
	OnSaveSlotsChanged(SaveManager->GetAllSlots());
}

void UBaseSaveMenuContentWidget::NativeDestruct()
{
	Super::NativeDestruct();
	
	USaveManager* SaveManager = GetGameInstance()->GetSubsystem<USaveManager>();
	if (!SaveManager) return;

	CreateButton->OnClicked.RemoveAll(this);
	SaveManager->OnSaveSlotsChanged.RemoveAll(this);
}
