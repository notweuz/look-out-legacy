#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/Save/SaveTypes.h"
#include "../Save/SaveTypes.h"
#include "StorageComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnStorageChanged);

UCLASS(ClassGroup=Inventory, meta=(BlueprintSpawnableComponent))
class LOOK_OUT_API UStorageComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Storage")
	float MaxWeight = 50.0f;

	UPROPERTY(BlueprintReadOnly, Category="Storage")
	float CurrentWeight = 0.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, SaveGame)
	TArray<FItemSaveRecord> Items;

	FOnStorageChanged OnStorageChanged;

	UFUNCTION(BlueprintCallable, Category="Storage")
	bool AddItem(const FItemSaveRecord& Record);

	UFUNCTION(BlueprintCallable, Category="Storage")
	bool RemoveItem(int32 Index);

	UFUNCTION(BlueprintCallable, Category="Storage")
	bool TransferItem(int32 Index, UStorageComponent* Target);

	UFUNCTION(BlueprintCallable, Category="Storage")
	bool MoveItem(int32 FromIndex, int32 ToIndex);

	UFUNCTION(BlueprintCallable, Category="Storage")
	bool MoveItemUp(int32 Index);

	UFUNCTION(BlueprintCallable, Category="Storage")
	bool MoveItemDown(int32 Index);

	UFUNCTION(BlueprintPure, Category="Storage")
	bool CanFit(float ItemWeight) const;

	UFUNCTION(BlueprintPure, Category="Storage")
	float GetRemainingWeight() const { return MaxWeight - CurrentWeight; }

	virtual void SaveToRecords(TArray<FItemSaveRecord>& OutRecords) const;
	void LoadFromRecords(const TArray<FItemSaveRecord>& InRecords);

protected:
	virtual void OnItemMoved(int32 FromIndex, int32 ToIndex);
};
