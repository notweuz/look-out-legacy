// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Containers/StaticArray.h"
#include "Core/Components/StorageComponent.h"
#include "Widgets/Gameplay/BasePlayerInventorySlot.h"
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

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ScrollActiveItem(float Delta);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void CollectItem();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void DropHotbarItem();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UpdateHandItem(int OldItemIndex);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void InteractWithItemInHand();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	bool HasItemInHand() const;
	
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

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void OnInventoryWindowClicked();
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void OnSlotClicked(UBasePlayerInventorySlot* ClickedSlot, bool IsRMB);

	void OnInventoryWindowClickedFor(UStorageComponent* TargetStorage);
	
private:
	UFUNCTION()
	void ProcessItemDragNDrop(UBasePlayerInventorySlot* ClickedSlot);

	FItem* GetItemForSlot(const UBasePlayerInventorySlot* Slot);
	AActor* GetItemInHandActor() const;
	AActor* SpawnActorFromItem(
		const FItem& Item,
		const FTransform& SpawnTransform,
		ESpawnActorCollisionHandlingMethod CollisionHandling) const;
	bool IsValidHotbarIndex(int32 Index) const;
	void SaveActorStateToHotbarIndex(int32 HotbarIndex, AActor* Actor);
	void RemoveHandItemActors(int32 PreviousHotbarIndex);
	class UBasePlayerUI* GetPlayerUI() const;

	void UpdateInventoryUI() const;
	
	void UpdateHotbarUI() const;
	
	void UpdateEntireUI() const;
	
protected:
	UPROPERTY()
	class ABaseCharacter* OwnerCharacter;
};
