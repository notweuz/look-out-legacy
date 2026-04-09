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

    UFUNCTION(BlueprintCallable, Category="Inventory")
    void DropActiveItem();
    
    UFUNCTION(BlueprintCallable, Category="Hotbar")
    bool AssignToHotbar(int32 ItemIndex, int32 HotbarSlot);

    UFUNCTION(BlueprintCallable, Category="Hotbar")
    void ClearHotbarSlot(int32 HotbarSlot);

    UFUNCTION(BlueprintCallable, Category="Hotbar")
    void SetActiveSlot(int32 SlotIndex);

    UFUNCTION(BlueprintCallable, Category="Hotbar")
    void NextSlot();

    UFUNCTION(BlueprintCallable, Category="Hotbar")
    void PrevSlot();
    
    UFUNCTION(BlueprintCallable, Category="Hotbar")
    void ScrollHotbar(int Delta);

    UFUNCTION(BlueprintPure, Category="Hotbar")
    bool GetActiveItem(FItemSaveRecord& OutRecord) const;

    UFUNCTION(BlueprintPure, Category="Hotbar")
    int32 GetActiveItemIndex() const;

    UFUNCTION(BlueprintCallable, Category="Hand")
    void EquipActiveItem();

    UFUNCTION(BlueprintCallable, Category="Hand")
    void UnequipItem();

private:
    UPROPERTY()
    ABaseCharacter* OwnerCharacter;
    
    void RefreshHandItem();

    void ShiftHotbarIndicesAfterRemoval(int32 RemovedItemIndex);
};