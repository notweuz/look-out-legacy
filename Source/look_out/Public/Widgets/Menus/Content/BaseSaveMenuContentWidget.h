// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/ListView.h"
#include "Components/ScrollBox.h"
#include "Core/Save/SaveTypes.h"
#include "Save/BaseSaveEntryWidget.h"
#include "BaseSaveMenuContentWidget.generated.h"

/**
 * 
 */
UCLASS()
class LOOK_OUT_API UBaseSaveMenuContentWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Save")
	TSubclassOf<UBaseSaveEntryWidget> SaveEntryWidgetClass;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Save")
	TArray<UBaseSaveEntryWidget*> SaveEntryWidgets;
	
	UPROPERTY(meta=(BindWidget), BlueprintReadOnly, EditDefaultsOnly, Category="Save")
	UScrollBox* SlotsList;
	
	UPROPERTY(meta=(BindWidget), BlueprintReadOnly, EditDefaultsOnly, Category="Save")
	UEditableTextBox* SlotName;
	
	UPROPERTY(meta=(BindWidget), BlueprintReadOnly, EditDefaultsOnly, Category="Save")
	UButton* CreateButton;

	UFUNCTION() void OnSaveSlotsChanged(const TArray<FSaveSlotMeta>& SaveSlots);
	UFUNCTION() void OnCreateButtonClicked();
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};
