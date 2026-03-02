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
	
	UPROPERTY(meta = (BindWidget))
	UBasePlayerInventorySlot* Slot1;
	
	UPROPERTY(meta = (BindWidget))
	UBasePlayerInventorySlot* Slot2;
	
	UPROPERTY(meta = (BindWidget))
	UBasePlayerInventorySlot* Slot3;
	
	UPROPERTY(meta = (BindWidget))
	UBasePlayerInventorySlot* Slot4;
	
	UPROPERTY(meta = (BindWidget))
	UBasePlayerInventorySlot* Slot5;
	
	UPROPERTY(meta = (BindWidget))
	UBasePlayerInventorySlot* Slot6;
	
	UPROPERTY(meta = (BindWidget))
	UBasePlayerInventorySlot* Slot7;
	
	UPROPERTY(meta = (BindWidget))
	UBasePlayerInventorySlot* Slot8;
	
	UPROPERTY(meta = (BindWidget))
	UBasePlayerInventorySlot* Slot9;
	
	UPROPERTY(meta = (BindWidget))
	UBasePlayerInventorySlot* Slot10;
	
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Hotbar Slots")
	void UpdateHotbarSlots(TArray<FItem>& ItemList);
};
