// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/Data/ItemDefinition.h"
#include "Core/Save/SaveTypes.h"
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
		TSubclassOf<AActor> ActorClass,
		UObject* Outer,
		float MaxHealth
	);
	
	UFUNCTION(BlueprintCallable, Category = "Save|Utils")
	static void LoadSaveProperties(UObject* TargetObject, const TArray<uint8>& Bytes);
	
	UFUNCTION(BlueprintCallable, Category = "Save|Utils", CustomThunk, meta = (CustomStructureParam = "OutValue", BlueprintInternalUseOnly = "true"))
	static bool GetSaveGameProperty(const FItemSaveRecord& Record, FName PropertyName, int32& OutValue);
	
	DECLARE_FUNCTION(execGetSaveGameProperty);

	UFUNCTION(BlueprintPure, Category = "Save|Utils")
	static void GetItemStorage(const FItemSaveRecord& Record, TArray<FItemSaveRecord>& OutItems, float& OutCurrentWeight);
};
