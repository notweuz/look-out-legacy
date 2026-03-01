// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "BasePlayerHotbarSlot.generated.h"

/**
 * 
 */
UCLASS()
class LOOK_OUT_API UBasePlayerHotbarSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UImage* Sprite;
	
	UPROPERTY(meta = (BindWidget))
	UBorder* SpriteBorder;
	
	UPROPERTY(meta = (BindWidget))
	UImage* ColorFill;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SlotNumberText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Colors")
	FLinearColor InActiveColor = FLinearColor::FromSRGBColor(FColor::FromHex("BCBCBCB8"));
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Colors")
	FLinearColor ActiveColor = FLinearColor::FromSRGBColor(FColor::FromHex("FFA065CC"));
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetSlotImage(UTexture2D* Texture);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetActive(bool bIsActive);
};
