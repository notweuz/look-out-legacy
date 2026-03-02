// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
	FLinearColor ActiveColor = FLinearColor::FromSRGBColor(FColor::FromHex("FFA065CC"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
	FLinearColor EmptySlotColor = FLinearColor::FromSRGBColor(FColor::FromHex("3A3A3A80"));

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetSlotImage(UTexture2D* Texture);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetActive(bool IsActive);
};