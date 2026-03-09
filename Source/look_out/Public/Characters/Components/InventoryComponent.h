// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Containers/StaticArray.h"
#include "Core/Components/StorageComponent.h"
#include "InventoryComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LOOK_OUT_API UInventoryComponent : public UStorageComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
	void ScrollActiveItem(float Delta);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
	void CollectItem();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
	void DropHotbarItem();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
	void UpdateHandItem(int OldItemIndex);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", SaveGame)
	TArray<FItem> Hotbar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", SaveGame)
	FItem SecondHand;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", SaveGame)
	FItem TempItem;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", SaveGame)
	int HotbarSize = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int CurrentActiveItemIndex = -1;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool IsHotbarFull() const;
protected:
	UPROPERTY()
	class ABaseCharacter* OwnerCharacter;
};
