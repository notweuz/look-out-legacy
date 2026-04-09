// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/Enums/NotificationType.h"
#include "BaseNotificationWidget.generated.h"

/**
 * 
 */
UCLASS()
class LOOK_OUT_API UBaseNotificationWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category="Notifications")
	void ShowNotification(const FText& Title, const FText& Message, float Duration, ENotificationType NotificationType);
};
