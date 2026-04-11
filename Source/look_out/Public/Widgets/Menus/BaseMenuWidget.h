// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/NamedSlot.h"
#include "Components/TextBlock.h"
#include "BaseMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class LOOK_OUT_API UBaseMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget), BlueprintReadOnly, Category="Window")
	UNamedSlot* BodySlot;
	
	UPROPERTY(meta=(BindWidget), BlueprintReadWrite, Category="Window")
	UTextBlock* TitleTextBlock;
	
	UPROPERTY(meta=(BindWidget), BlueprintReadOnly, Category="Window")
	UButton* CloseButton;
	
	UFUNCTION(BlueprintCallable, Category="Window")
	void SetBodyWidget(UWidget* Widget);
	
	UFUNCTION(BlueprintCallable, Category="Window")
	void Close();
};
