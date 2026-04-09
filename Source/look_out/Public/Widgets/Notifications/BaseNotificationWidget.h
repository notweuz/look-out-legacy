// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Core/Data/NotificationData.h"
#include "BaseNotificationWidget.generated.h"

UCLASS()
class LOOK_OUT_API UBaseNotificationWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitNotification(const FNotificationData& InData);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* MessageText;

	UPROPERTY(meta=(BindWidget))
	UImage* TypeIcon;

	UPROPERTY(EditDefaultsOnly, Category="Notification|Icons")
	UTexture2D* IconInfo;

	UPROPERTY(EditDefaultsOnly, Category="Notification|Icons")
	UTexture2D* IconWarning;

	UPROPERTY(EditDefaultsOnly, Category="Notification|Icons")
	UTexture2D* IconError;

	UPROPERTY(EditDefaultsOnly, Category="Notification|Icons")
	UTexture2D* IconDebug;

private:
	FNotificationData Data;
	FTimerHandle DismissTimerHandle;

	void ApplyData();
	UTexture2D* GetIconForType(ENotificationType Type) const;

	UFUNCTION()
	void DismissSelf();
};