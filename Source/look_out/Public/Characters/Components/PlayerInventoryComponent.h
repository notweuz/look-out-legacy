#pragma once
#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Core/Components/StorageComponent.h"
#include "Core/Save/SaveTypes.h"
#include "PlayerInventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHotbarChanged,     int32 /*SlotIndex*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnActiveSlotChanged, int32 /*NewSlotIndex*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemEquipped,      AActor* /*SpawnedActor*/);
DECLARE_MULTICAST_DELEGATE(FOnItemUnequipped);

UCLASS(ClassGroup=Inventory, meta=(BlueprintSpawnableComponent))
class LOOK_OUT_API UPlayerInventoryComponent : public UStorageComponent
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Hotbar")
    int32 HotbarSize = 10;
    
    UPROPERTY(BlueprintReadOnly, Category="Hotbar")
    TArray<int32> HotbarSlots;

    UPROPERTY(BlueprintReadOnly, Category="Hotbar")
    int32 ActiveSlotIndex = 0;

    UPROPERTY(BlueprintReadOnly, Category="Hotbar")
    TObjectPtr<AActor> ItemInHand;

    FOnHotbarChanged     OnHotbarChanged;
    FOnActiveSlotChanged OnActiveSlotChanged;
    FOnItemEquipped      OnItemEquipped;
    FOnItemUnequipped    OnItemUnequipped;

    virtual void BeginPlay() override;
    
    UFUNCTION(BlueprintCallable, Category="Inventory")
    bool TryPickup(AActor* Actor);
    
    UFUNCTION(BlueprintCallable, Category="Inventory")
    void Collect();

    UFUNCTION(BlueprintCallable, Category="Inventory|Hotbar")
    void DropActiveItem();
    
    UFUNCTION(BlueprintCallable, Category="Inventory|Hotbar")
    bool AssignToHotbar(int32 ItemIndex, int32 HotbarSlot);

    UFUNCTION(BlueprintCallable, Category="Inventory|Hotbar")
    void ClearHotbarSlot(int32 HotbarSlot);

    UFUNCTION(BlueprintCallable, Category="Inventory|Hotbar")
    void SetActiveSlot(int32 SlotIndex);

    UFUNCTION(BlueprintCallable, Category="Inventory|Hotbar")
    void NextSlot();

    UFUNCTION(BlueprintCallable, Category="Inventory|Hotbar")
    void PrevSlot();
    
    UFUNCTION(BlueprintCallable, Category="Inventory|Hotbar")
    void ScrollHotbar(int Delta);

    UFUNCTION(BlueprintPure, Category="Inventory|Hotbar")
    bool GetActiveItem(FItemSaveRecord& OutRecord) const;

    UFUNCTION(BlueprintPure, Category="Inventory|Hotbar")
    int32 GetActiveItemIndex() const;

    void SaveToRecords(TArray<FItemSaveRecord>& OutRecords) const;

    UFUNCTION(BlueprintCallable, Category="Hand")
    void EquipActiveItem();

    UFUNCTION(BlueprintCallable, Category="Hand")
    void UnequipItem();
    
    UFUNCTION(BlueprintCallable, Category="Hand")
    void ToggleEquip();
    
    UFUNCTION(BlueprintCallable, Category="Hand")
    void InteractWithActiveItem();

private:
    UPROPERTY()
    ABaseCharacter* OwnerCharacter;

    UPROPERTY()
    int32 EquippedItemIndex = INDEX_NONE;
    
    void RefreshHandItem();
    void SaveEquippedItemState();

    void ShiftHotbarIndicesAfterRemoval(int32 RemovedItemIndex);
    virtual void OnItemMoved(int32 FromIndex, int32 ToIndex) override;
};
