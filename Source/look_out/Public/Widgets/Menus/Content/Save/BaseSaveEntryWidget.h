// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "BaseSaveEntryWidget.generated.h"

/**
 * 
 */
UCLASS()
class LOOK_OUT_API UBaseSaveEntryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FString SaveName;
	
	UPROPERTY(meta=(BindWidget), BlueprintReadWrite, Category="Save")
	UTextBlock* SaveNameText;
	
	UPROPERTY(meta=(BindWidget), BlueprintReadWrite, Category="Save")
	UTextBlock* SaveDateText;
	
	UPROPERTY(meta=(BindWidget), BlueprintReadWrite, Category="Save")
	UButton* DeleteButton;
	
	UPROPERTY(meta=(BindWidget), BlueprintReadWrite, Category="Save")
	UButton* LoadButton;
	
	UFUNCTION() void OnDeleteButtonPressed();
	UFUNCTION() void OnLoadButtonPressed();
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};
