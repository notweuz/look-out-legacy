// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseNotificationWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "Core/Subsystems/NotificationSubsystem.h"
#include "BaseNotificationCenterWidget.generated.h"

/**
 * 
 */
UCLASS()
class LOOK_OUT_API UBaseNotificationCenterWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UPROPERTY(meta=(BindWidget), EditDefaultsOnly, Category="Notifications")
	UVerticalBox* NotificationsBox;
	
	UPROPERTY(EditDefaultsOnly, Category="Notifications")
	TSubclassOf<UBaseNotificationWidget> NotificationWidgetClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Notifications")
	TArray<UBaseNotificationWidget*> Notifications;
	
	UPROPERTY()
	UNotificationSubsystem* NotificationSubsystem;
	
	UFUNCTION()
	void HandleNotificationReceived(const FNotificationData& Notification);
};
