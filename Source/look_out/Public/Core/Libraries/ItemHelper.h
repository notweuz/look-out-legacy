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

	
	UFUNCTION(BlueprintCallable, Category = "Item|Utils")
	static void SetFloatProperty(FItemSaveRecord& Record, FName PropertyName, float Value);

	UFUNCTION(BlueprintCallable, Category = "Item|Utils")
	static void SetIntProperty(FItemSaveRecord& Record, FName PropertyName, int32 Value);

	UFUNCTION(BlueprintCallable, Category = "Item|Utils")
	static void SetBoolProperty(FItemSaveRecord& Record, FName PropertyName, bool Value);

	UFUNCTION(BlueprintCallable, Category = "Item|Utils")
	static void SetStringProperty(FItemSaveRecord& Record, FName PropertyName, const FString& Value);

	UFUNCTION(BlueprintCallable, Category = "Item|Utils")
	static void SetTextProperty(FItemSaveRecord& Record, FName PropertyName, const FText& Value);

	UFUNCTION(BlueprintCallable, Category = "Item|Utils")
	static void SetNameProperty(FItemSaveRecord& Record, FName PropertyName, FName Value);

	UFUNCTION(BlueprintPure, Category = "Item|Utils")
	static bool IsValidItemRecord(const FItemSaveRecord& Record);

	UFUNCTION(BlueprintPure, Category = "Item|Utils")
	static bool HasProperty(const FItemSaveRecord& Record, FName PropertyName);

	UFUNCTION(BlueprintPure, Category = "Item|Utils")
	static TArray<FName> GetAllPropertyNames(const FItemSaveRecord& Record);

	UFUNCTION(BlueprintPure, Category = "Item|Utils")
	static float GetItemWeight(const FItemSaveRecord& Record);

	UFUNCTION(BlueprintPure, Category = "Item|Utils")
	static FText GetItemDisplayName(const FItemSaveRecord& Record);

	
	UFUNCTION(BlueprintCallable, Category = "Item|Utils")
	static FItemSaveRecord CreateItemRecord(TSubclassOf<AActor> ItemClass);

	UFUNCTION(BlueprintCallable, Category = "Item|Utils")
	static FItemSaveRecord CreateItemRecordFromDefinition(UItemDefinition* ItemDef);

	
	UFUNCTION(BlueprintCallable, Category = "Item|Utils")
	static void SetStorageProperty(FItemSaveRecord& Record, const TArray<FItemSaveRecord>& Items, float CurrentWeight);

	UFUNCTION(BlueprintPure, Category = "Item|Utils")
	static int32 GetStorageItemCount(const FItemSaveRecord& Record);

	UFUNCTION(BlueprintPure, Category = "Item|Utils")
	static bool IsStorageEmpty(const FItemSaveRecord& Record);

	
	UFUNCTION(BlueprintPure, Category = "Item|Utils")
	static bool AreItemsEqual(const FItemSaveRecord& RecordA, const FItemSaveRecord& RecordB);

	UFUNCTION(BlueprintPure, Category = "Item|Utils")
	static bool IsSameItemType(const FItemSaveRecord& RecordA, const FItemSaveRecord& RecordB);

private:
	static void LoadSavePropertiesInternal(UObject* TargetObject, const TArray<uint8>& Bytes);
};