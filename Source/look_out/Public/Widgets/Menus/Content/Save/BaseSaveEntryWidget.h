// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "BaseSaveEntryWidget.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveSelected, UBaseSaveEntryWidget*, Widget);

/**
 * 
 */
UCLASS()
class LOOK_OUT_API UBaseSaveEntryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FString SaveName;
	
	UPROPERTY(BlueprintAssignable, Category="Save")
	FOnSaveSelected OnSaveSelected;
	
	UPROPERTY(meta=(BindWidget), BlueprintReadWrite, Category="Save|Info")
	UTextBlock* SaveNameText;
	
	UPROPERTY(meta=(BindWidget), BlueprintReadWrite, Category="Save|Info")
	UTextBlock* DaysPassedText;
	
	UPROPERTY(meta=(BindWidget), BlueprintReadWrite, Category="Save|Info")
	UTextBlock* SaveDateText;
	
	UPROPERTY(meta=(BindWidget), BlueprintReadWrite, Category="Save")
	UButton* SelectButton;
	
	UFUNCTION() void OnSelectButtonClicked();
	UFUNCTION(BlueprintImplementableEvent, Category = "Save") void OnStateChanged(bool Selected);
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};
