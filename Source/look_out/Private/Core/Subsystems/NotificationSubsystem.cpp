// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#include "Core/Subsystems/NotificationSubsystem.h"

void UNotificationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UNotificationSubsystem::BroadcastNotification(const FNotificationData& Notification)
{
	OnNotificationReceived.Broadcast(Notification);
}

void UNotificationSubsystem::SendNotification(
	const FText& Title,
	const FText& Message,
	float Duration,
	ENotificationType Type)
{
	FNotificationData Data;
	Data.Title = Title;
	Data.Message = Message;
	Data.Duration = Duration;
	Data.NotificationType = Type;

	BroadcastNotification(Data);
}