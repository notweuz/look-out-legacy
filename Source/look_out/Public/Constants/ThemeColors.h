// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ThemeColors.generated.h"

/**
 * 
 */
UCLASS()
class LOOK_OUT_API UThemeColors : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Inventory")
	static FLinearColor SelectedSlotBackground();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Inventory")
	static FLinearColor SelectedSlotBorder();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Inventory")
	static FLinearColor SlotBackground();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Inventory")
	static FLinearColor SlotBorder();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Inventory")
	static FLinearColor MovingSlotBorder();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Inventory")
	static FLinearColor MovingSlotBackground();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Inventory")
	static FLinearColor GetLinearColorFromHex(const FString& InHex);
};
