// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/Data/ItemDefinition.h"
#include "ItemHelper.generated.h"

UCLASS()
class LOOK_OUT_API UItemHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Inventory Definition|Manual")
	static UItemDefinition* BuildItemDefinition(
		FText Name,
		FText Description, 
		float Weight, 
		UTexture2D* Icon, 
		AActor* ActorClass
	);
};
