// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/WrapBox.h"
#include "Data/Storage/Item.h"
#include "Widgets/Gameplay/BasePlayerInventorySlot.h"
#include "PlayerInventoryWindowWidget.generated.h"

/**
 * 
 */
UCLASS()
class LOOK_OUT_API UPlayerInventoryWindowWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category="Inventory")
	UWrapBox* InventorySlotsBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Inventory")
	TSubclassOf<UBasePlayerInventorySlot> InventorySlotClass;

	UPROPERTY(BlueprintReadWrite, Category="Inventory")
	TArray<UBasePlayerInventorySlot*> InventorySlotWidgets;

	UFUNCTION(BlueprintCallable, Category="Inventory")
	void UpdateInventorySlots(TArray<FItem>& ItemList);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	void ClearInventorySlots();
};
