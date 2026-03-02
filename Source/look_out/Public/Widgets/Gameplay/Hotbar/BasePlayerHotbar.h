// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BasePlayerInventorySlot.h"
#include "Blueprint/UserWidget.h"
#include "Components/BackgroundBlur.h"
#include "Data/Storage/Item.h"
#include "BasePlayerHotbar.generated.h"

/**
 * 
 */
UCLASS()
class LOOK_OUT_API UBasePlayerHotbar : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hotbar Slots")
	TArray<FItem> Hotbar;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hotbar Slots")
	TArray<UBasePlayerInventorySlot*> Slots;

	UPROPERTY(meta = (BindWidget))
	UBackgroundBlur* BackgroundBlur;

	UPROPERTY(meta = (BindWidget))
	UBorder* Background;
	
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Hotbar Slots")
	void UpdateHotbarSlots(TArray<FItem>& ItemList);
};
