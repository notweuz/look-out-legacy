// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BasePlayerHotbarSlot.h"
#include "Blueprint/UserWidget.h"
#include "Components/BackgroundBlur.h"
#include "BasePlayerHotbar.generated.h"

/**
 * 
 */
UCLASS()
class LOOK_OUT_API UBasePlayerHotbar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TArray<UBasePlayerHotbarSlot*> HotbarSlots;
	
	UPROPERTY(meta = (BindWidget))
	UBackgroundBlur* BackgroundBlur;
	
	UPROPERTY(meta = (BindWidget))
	UImage* BackgroundFill;
};
