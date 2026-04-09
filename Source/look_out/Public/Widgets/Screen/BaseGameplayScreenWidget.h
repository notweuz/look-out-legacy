// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/HUD/GameHUDWidget.h"
#include "Widgets/Notifications/BaseNotificationCenterWidget.h"
#include "BaseGameplayScreenWidget.generated.h"

/**
 * 
 */
UCLASS()
class LOOK_OUT_API UBaseGameplayScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Screen")
	TSubclassOf<UGameHUDWidget> HUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Screen")
	TSubclassOf<UBaseNotificationCenterWidget> NotificationCenterWidgetClass;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UGameHUDWidget* HUD;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UBaseNotificationCenterWidget* NotificationCenter;
};