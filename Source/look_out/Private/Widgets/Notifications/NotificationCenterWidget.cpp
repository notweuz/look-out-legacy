// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Widgets/Notifications/NotificationCenterWidget.h"

void UNotificationCenterWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (UWorld* World = GetWorld())
	{
		NotificationSubsystem = World->GetGameInstance()->GetSubsystem<UNotificationSubsystem>();
		if (NotificationSubsystem)
		{
			NotificationSubsystem->OnNotificationReceived.AddDynamic(this, &UNotificationCenterWidget::HandleNotificationReceived);
		}
	}
}

void UNotificationCenterWidget::NativeDestruct()
{
	if (NotificationSubsystem)
	{
		NotificationSubsystem->OnNotificationReceived.RemoveDynamic(this, &UNotificationCenterWidget::HandleNotificationReceived);
	}
	Super::NativeDestruct();
}

void UNotificationCenterWidget::HandleNotificationReceived(const FNotificationData& Notification)
{
	if (!NotificationWidgetClass || !NotificationsBox) return;

	UBaseNotificationWidget* NewNotif = CreateWidget<UBaseNotificationWidget>(this, NotificationWidgetClass);
	if (NewNotif)
	{
		NewNotif->ShowNotification(Notification.Title, Notification.Message, Notification.Duration, Notification.NotificationType);
		NotificationsBox->AddChild(NewNotif);
	}
}
