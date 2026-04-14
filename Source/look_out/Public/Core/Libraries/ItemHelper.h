#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Core/Data/ItemDefinition.h"
#include "Core/Save/SaveTypes.h"
#include "ItemHelper.generated.h"

UCLASS()
class LOOK_OUT_API UItemHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Inventory Definition|Manual")
	static UItemDefinition* BuildItemDefinition(
		FText Name,
		FText Description,
		float Weight,
		UTexture2D* Icon,
		TSubclassOf<AActor> ActorClass,
		UObject* Outer = nullptr,
		float MaxHealth = 100.0f
	);

	UFUNCTION(BlueprintCallable, Category = "Item|Utils")
	static void LoadSaveProperties(UObject* TargetObject, const TArray<uint8>& Bytes);

	UFUNCTION(BlueprintPure, Category = "Item|Utils")
	static float GetFloatProperty(const FItemSaveRecord& Record, FName PropertyName);

	UFUNCTION(BlueprintPure, Category = "Item|Utils")
	static int32 GetIntProperty(const FItemSaveRecord& Record, FName PropertyName);

	UFUNCTION(BlueprintPure, Category = "Item|Utils")
	static bool GetBoolProperty(const FItemSaveRecord& Record, FName PropertyName);

	UFUNCTION(BlueprintPure, Category = "Item|Utils")
	static FString GetStringProperty(const FItemSaveRecord& Record, FName PropertyName);

	UFUNCTION(BlueprintPure, Category = "Item|Utils")
	static FText GetTextProperty(const FItemSaveRecord& Record, FName PropertyName);

	UFUNCTION(BlueprintPure, Category = "Item|Utils")
	static FName GetNameProperty(const FItemSaveRecord& Record, FName PropertyName);

	UFUNCTION(BlueprintPure, Category = "Item|Utils")
	static void GetStorageProperty(const FItemSaveRecord& Record, TArray<FItemSaveRecord>& OutItems, float& OutCurrentWeight);

private:
	static void LoadSavePropertiesInternal(UObject* TargetObject, const TArray<uint8>& Bytes);
};