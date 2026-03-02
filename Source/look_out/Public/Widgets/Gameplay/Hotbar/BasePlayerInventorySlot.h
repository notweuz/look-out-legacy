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
	USizeBox* SlotSizeBox;

	UPROPERTY(meta = (BindWidget))
	UImage* Sprite;

	UPROPERTY(meta = (BindWidget))
	UBorder* SpriteBorder;

	UPROPERTY(meta = (BindWidget))
	UImage* ColorFill;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SlotNumberText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCountText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot|Size")
	float SlotSize = 64.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
	FLinearColor InActiveColor = FLinearColor::FromSRGBColor(FColor::FromHex("BCBCBCB8"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
	FLinearColor ActiveColor = FLinearColor::FromSRGBColor(FColor::FromHex("FFA065CC"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
	FLinearColor EmptySlotColor = FLinearColor::FromSRGBColor(FColor::FromHex("3A3A3A80"));

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetSlotImage(UTexture2D* Texture);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetActive(bool bIsActive);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetItemCount(int32 Count);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetSlotIndex(int32 Index);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ClearSlot();
};