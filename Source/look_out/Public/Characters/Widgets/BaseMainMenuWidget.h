// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "BaseMainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class LOOK_OUT_API UBaseMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UImage* LogoImage;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "Main Buttons")
	UButton* StartButton;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "Main Buttons")
	UButton* SettingsButton;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "Main Buttons")
	UButton* CreditsButton;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "Main Buttons")
	UButton* ExitButton;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "Links")
	UButton* DiscordButton;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "Links")
	UButton* TelegramButton;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "Links")
	UButton* BoostyButton;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "Info")
	UTextBlock* Version;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "Info")
	UTextBlock* Authors;

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnExitClicked();
};
