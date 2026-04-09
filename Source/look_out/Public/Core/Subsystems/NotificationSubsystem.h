// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Core/Data/NotificationData.h"
#include "Core/Enums/NotificationType.h"
#include "NotificationSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNotificationReceived, const FNotificationData&, Notification);

UCLASS()
class LOOK_OUT_API UNotificationSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, Category = "Notification")
	void BroadcastNotification(const FNotificationData& Notification);

	UFUNCTION(BlueprintCallable, Category = "Notification", meta = (AdvancedDisplay = 3))
	void SendNotification(
		const FText& Message,
		const float Duration,
		ENotificationType Priority = ENotificationType::Info
	);

	UPROPERTY(BlueprintAssignable, Category = "Notification")
	FOnNotificationReceived OnNotificationReceived;

private:
	TArray<FNotificationData> PendingNotifications;
};