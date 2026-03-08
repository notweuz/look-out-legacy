// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Core/Components/StorageComponent.h"

#include "Interfaces/Storeable.h"

// Sets default values for this component's properties
UStorageComponent::UStorageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
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

UStorageComponent* UStorageComponent::GetStorageComponentFromActor(AActor* Actor)
{
	return Actor ? Actor->FindComponentByClass<UStorageComponent>() : nullptr;
}

UStorageComponent* UStorageComponent::GetStorageLink()
{
	return this;
}

AActor* UStorageComponent::RetrieveItem_Implementation(const int32 Index, const FTransform SpawnTransform)
{
	if (!Storage.IsValidIndex(Index))
	{
		UE_LOG(LogTemp, Warning, TEXT("StorageComponent: Invalid index %d"), Index);
		return nullptr;
	}

	FItem& Item = Storage[Index];

	const TSubclassOf<AActor> ActorClass = Item.ItemClass.LoadSynchronous();
	if (!ActorClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("StorageComponent: Failed to load class at index %d"), Index);
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorClass, SpawnTransform, SpawnParams);
	if (!SpawnedActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("StorageComponent: Failed to spawn actor"));
		return nullptr;
	}

	IStoreable::Execute_ApplyItemTags(SpawnedActor, Item.SavedTags);

	Item.Quantity--;
	if (Item.Quantity <= 0)
	{
		Storage.RemoveAt(Index);
	}

	return SpawnedActor;
}

int32 UStorageComponent::GetRemainingStorage_Implementation()
{
	int32 TotalWeight = 0;

	for (const FItem& Item : Storage)
	{
		TotalWeight += Item.ItemWeight * Item.Quantity;
	}

	return MaxStorageWeight - TotalWeight;
}

void UStorageComponent::AddItem_Implementation(AActor* Actor)
{
	if (!Actor || !Actor->Implements<UStoreable>())
	{
		UE_LOG(LogTemp, Warning, TEXT("StorageComponent: Actor does not implement IStoreable"));
		return;
	}

	const int32 ItemWeight = IStoreable::Execute_GetItemWeight(Actor);

	if (GetRemainingStorage() < ItemWeight)
	{
		UE_LOG(LogTemp, Warning, TEXT("StorageComponent: Not enough storage weight"));
		// TODO: UI notification
		return;
	}

	FItem NewItem;
	NewItem.ItemClass = Actor->GetClass();
	NewItem.ItemWeight = ItemWeight;
	NewItem.Quantity = 1;
	NewItem.SavedTags = IStoreable::Execute_GetItemTags(Actor);

	Storage.Add(NewItem);
	Actor->Destroy();
}

FItem UStorageComponent::RemoveItem_Implementation(const int32 Index)
{
	if (!Storage.IsValidIndex(Index))
	{
		UE_LOG(LogTemp, Warning, TEXT("StorageComponent: Invalid index %d"), Index);
		return FItem();
	}

	FItem RemovedItem = Storage[Index];
	Storage.RemoveAt(Index);
	return RemovedItem;
}

bool UStorageComponent::TransferItem_Implementation(UStorageComponent* OldStorage, const int32 ItemIndex)
{
	if (!OldStorage)
	{
		return false;
	}

	if (!OldStorage->Storage.IsValidIndex(ItemIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("StorageComponent: Invalid index %d in source storage"), ItemIndex);
		return false;
	}

	if (const FItem Item = OldStorage->Storage[ItemIndex]; GetRemainingStorage() < Item.ItemWeight)
	{
		UE_LOG(LogTemp, Warning, TEXT("StorageComponent: Not enough space in target storage"));
		return false;
	}

	const FItem Removed = OldStorage->RemoveItem(ItemIndex);
	Storage.Add(Removed);
	return true;
}
