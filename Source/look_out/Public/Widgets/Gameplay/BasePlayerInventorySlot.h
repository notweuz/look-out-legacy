// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "BasePlayerInventorySlot.generated.h"

UCLASS()
class LOOK_OUT_API UBasePlayerInventorySlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UImage* Sprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Slot")
	bool bIsActive = false;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetSlotImage(UTexture2D* Texture);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetActive(bool IsActive);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void StateChanged(bool IsActive);
};
