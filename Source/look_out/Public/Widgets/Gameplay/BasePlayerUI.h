// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Hotbar/BasePlayerHotbar.h"
#include "BasePlayerUI.generated.h"

/**
 * 
 */
UCLASS()
class LOOK_OUT_API UBasePlayerUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBasePlayerHotbar* Hotbar;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UNamedSlot* SoloInventorySlot;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UNamedSlot* InventorySlot1;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UNamedSlot* InventorySlot2;
};
