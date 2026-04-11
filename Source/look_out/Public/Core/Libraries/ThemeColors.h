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
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="General")
	static FSlateBrush MakeBrush(FLinearColor Color);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="General")
	static FLinearColor GeneralBackground();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="General")
	static FLinearColor DarkerBackground();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="General")
	static FLinearColor GeneralBorder();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="General")
	static FLinearColor GeneralClose();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="General")
	static FLinearColor GeneralSelected();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Styles")
	static FButtonStyle DefaultButtonStyle();
};
