// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Core/Components/StorageComponent.h"

#include "Interfaces/Storeable.h"
#include "Misc/LogCategories.h"

// Sets default values for this component's properties
UStorageComponent::UStorageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

int64 UStorageComponent::GetItemTotalWeight(const FItem& Item)
{
	if (Item.ItemClass.IsNull())
	{
		return 0;
	}

	const int64 PerItem = FMath::Max<int64>(0, Item.ItemWeight);
	const int64 Qty = FMath::Max<int64>(0, Item.Quantity);
	return PerItem * Qty;
}

int64 UStorageComponent::GetCurrentWeight() const
{
	int64 TotalWeight = 0;

	for (const FItem& Item : Storage)
	{
		TotalWeight += GetItemTotalWeight(Item);
	}

	return TotalWeight;
}

bool UStorageComponent::CanAddItem(const FItem& Item) const
{
	const int64 NewTotal = GetCurrentWeight() + GetItemTotalWeight(Item);
	return NewTotal <= static_cast<int64>(MaxStorageWeight);
}

bool UStorageComponent::CanSwapItems(const FItem& Incoming, const FItem& Outgoing) const
{
	const int64 NewTotal = GetCurrentWeight() + GetItemTotalWeight(Incoming) - GetItemTotalWeight(Outgoing);
	return NewTotal <= static_cast<int64>(MaxStorageWeight);
}


// Called when the game starts
void UStorageComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UStorageComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FActorAsItemResult UStorageComponent::GetActorAsItemResult(AActor* Actor)
{
	FActorAsItemResult Result;
	
	if (!Actor || !Actor->Implements<UStoreable>())
	{
		UE_LOG(LogLookOutGame, Warning, TEXT("StorageComponent: Actor does not implement IStoreable"));
		Result.Actor = nullptr;
		Result.Success = false;
		return Result;
	}

	const int32 ItemWeight = IStoreable::Execute_GetItemWeight(Actor);
	FItem NewItem;
	NewItem.ItemClass = Actor->GetClass();
	NewItem.ItemWeight = ItemWeight;
	NewItem.Quantity = 1;
	NewItem.SavedTags = IStoreable::Execute_GetItemTags(Actor);
	NewItem.ItemIcon = IStoreable::Execute_GetItemIcon(Actor);
	
	Result.Success = true;
	Result.Item = NewItem;
	Result.Actor = Actor;
	
	return Result;
}

UStorageComponent* UStorageComponent::GetStorageComponentFromActor(AActor* Actor)
{
	return Actor ? Actor->FindComponentByClass<UStorageComponent>() : nullptr;
}

UStorageComponent* UStorageComponent::GetStorageLink()
{
	return this;
}

void UStorageComponent::AddItem_Implementation(AActor* Actor)
{
	FActorAsItemResult Result = GetActorAsItemResult(Actor);
	if (!Result.Success) return;
	
	if (!CanAddItem(Result.Item))
	{
		UE_LOG(LogLookOutGame, Warning, TEXT("StorageComponent: Not enough storage weight"));
		// TODO: UI notification
		return;
	}

	Storage.Add(Result.Item);
	Actor->Destroy();
}

FItem UStorageComponent::RemoveItem_Implementation(const int32 Index)
{
	if (!Storage.IsValidIndex(Index))
	{
		UE_LOG(LogLookOutGame, Warning, TEXT("StorageComponent: Invalid index %d"), Index);
		return FItem();
	}

	FItem RemovedItem = Storage[Index];
	Storage.RemoveAt(Index);
	return RemovedItem;
}
