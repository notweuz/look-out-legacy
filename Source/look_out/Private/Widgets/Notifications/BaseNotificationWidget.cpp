// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Widgets/Notifications/BaseNotificationWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Animation/WidgetAnimation.h"

void UBaseNotificationWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UBaseNotificationWidget::InitNotification(const FNotificationData& InData)
{
    Data = InData;
    ApplyData();

    if (SlideIn)
    {
        PlayAnimation(SlideIn);
    }

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            DismissTimerHandle,
            this,
            &UBaseNotificationWidget::StartDismiss,
            Data.Duration,
            false
        );
    }
}

void UBaseNotificationWidget::StartDismiss()
{
    if (SlideOut)
    {
        PlayAnimationForward(SlideOut);

        FWidgetAnimationDynamicEvent AnimFinished;
        AnimFinished.BindDynamic(this, &UBaseNotificationWidget::DismissSelf);
        BindToAnimationFinished(SlideOut, AnimFinished);
    }
    else
    {
        DismissSelf();
    }
}

void UBaseNotificationWidget::DismissSelf()
{
    RemoveFromParent();
}

void UBaseNotificationWidget::ApplyData()
{
    if (MessageText)
    {
        MessageText->SetText(Data.Message);
    }
    if (TypeIcon)
    {
        if (UTexture2D* Icon = GetIconForType(Data.NotificationType))
        {
            TypeIcon->SetBrushFromTexture(Icon);
        }
    }
}

UTexture2D* UBaseNotificationWidget::GetIconForType(ENotificationType Type) const
{
    switch (Type)
    {
        case ENotificationType::Info:    return IconInfo;
        case ENotificationType::Warning: return IconWarning;
        case ENotificationType::Error:   return IconError;
        case ENotificationType::Debug:   return IconDebug;
        default:                         return IconInfo;
    }
}