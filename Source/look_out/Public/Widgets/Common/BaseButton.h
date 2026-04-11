// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "BaseButton.generated.h"

/**
 * 
 */
UCLASS()
class LOOK_OUT_API UBaseButton : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;
	
	UPROPERTY(meta=(BindWidget), BlueprintReadWrite, Category="Button")
	UButton* Button;
	
	UPROPERTY(meta=(BindWidget), BlueprintReadWrite, Category="Button")
	UTextBlock* Text;
private:
	void SetupStyle();
};
