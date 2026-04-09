// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Widgets/Notifications/BaseNotificationCenterWidget.h"

void UBaseNotificationCenterWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (UWorld* World = GetWorld())
	{
		NotificationSubsystem = World->GetGameInstance()->GetSubsystem<UNotificationSubsystem>();
		if (NotificationSubsystem)
		{
			NotificationSubsystem->OnNotificationReceived.AddDynamic(this, &UBaseNotificationCenterWidget::HandleNotificationReceived);
		}
	}
}

void UBaseNotificationCenterWidget::NativeDestruct()
{
	if (NotificationSubsystem)
	{
		NotificationSubsystem->OnNotificationReceived.RemoveDynamic(this, &UBaseNotificationCenterWidget::HandleNotificationReceived);
	}
	Super::NativeDestruct();
}

void UBaseNotificationCenterWidget::HandleNotificationReceived(const FNotificationData& Notification)
{
	if (!NotificationWidgetClass || !NotificationsBox) return;

	UBaseNotificationWidget* NewNotif = CreateWidget<UBaseNotificationWidget>(this, NotificationWidgetClass);
	if (NewNotif)
	{
		NewNotif->InitNotification(Notification);
		NotificationsBox->AddChild(NewNotif);
	}
}
