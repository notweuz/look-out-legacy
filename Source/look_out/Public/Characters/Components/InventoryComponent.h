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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int CurrentActiveItemIndex = -1;
	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", SaveGame)
	TArray<FItem> Hotbar;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", SaveGame)
	FItem SecondHand;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
	void ScrollActiveItem(float Delta);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", SaveGame)
	int HotbarSize = 10;
	
protected:
	UPROPERTY()
	class ABaseCharacter* OwnerCharacter;
};
