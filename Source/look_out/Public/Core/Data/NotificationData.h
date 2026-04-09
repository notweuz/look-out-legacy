// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Core/Enums/NotificationType.h"
#include "NotificationData.generated.h"

USTRUCT(BlueprintType)
struct FNotificationData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Notification")
	FText Message;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Notification")
	ENotificationType NotificationType;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Notification")
	float Duration = 3.0f;
};
