// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "Widgets/Common/BaseButton.h"
#include "Widgets/Menus/BaseMenuWidget.h"
#include "Widgets/Menus/Content/BaseSaveMenuContentWidget.h"
#include "Widgets/Notifications/BaseNotificationCenterWidget.h"
#include "BaseMainMenuScreenWidget.generated.h"

/**
 * 
 */
UCLASS()
class LOOK_OUT_API UBaseMainMenuScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UImage* LogoImage;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "MainScreen|Main Buttons") UBaseButton* StartButton;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "MainScreen|Main Buttons") UBaseButton* SettingsButton;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "MainScreen|Main Buttons") UBaseButton* CreditsButton;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "MainScreen|Main Buttons") UBaseButton* ExitButton;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "MainScreen|Links") UButton* DiscordButton;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "MainScreen|Links") UButton* TelegramButton;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "MainScreen|Links") UButton* BoostyButton;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "MainScreen|Info") UTextBlock* Version;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "MainScreen|Info") URichTextBlock* Authors;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "MainScreen|Menus") UNamedSlot* MenuSlot;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MainScreen|Menus")
	TSubclassOf<UBaseMenuWidget> MenuWidgetClass;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MainScreen|Menus")
	TSubclassOf<UBaseSaveMenuContentWidget> SaveMenuContentWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "MainScreen|Notifications") TSubclassOf<UBaseNotificationCenterWidget> NotificationCenterWidgetClass;
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category="MainScreen|Notifications") UBaseNotificationCenterWidget* NotificationCenter;
	
	UFUNCTION() void OnStartButtonPressed();
protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnExitClicked();
};
