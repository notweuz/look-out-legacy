#include "Core/StorageComponent.h"

bool UStorageComponent::CanFit(float ItemWeight) const
{
	return (CurrentWeight + ItemWeight) <= MaxWeight;
}

bool UStorageComponent::AddItem(const FItemSaveRecord& Record)
{
	if (!CanFit(Record.Weight)) return false;

	Items.Add(Record);
	CurrentWeight += Record.Weight;
	OnStorageChanged.Broadcast();
	return true;
}

bool UStorageComponent::RemoveItem(int32 Index)
{
	if (!Items.IsValidIndex(Index)) return false;

	CurrentWeight -= Items[Index].Weight;
	Items.RemoveAt(Index);
	OnStorageChanged.Broadcast();
	return true;
}

bool UStorageComponent::TransferItem(int32 Index, UStorageComponent* Target)
{
	if (!Target || !Items.IsValidIndex(Index)) return false;

	FItemSaveRecord Record = Items[Index];
	if (!Target->AddItem(Record)) return false;

	CurrentWeight -= Record.Weight;
	Items.RemoveAt(Index);
	OnStorageChanged.Broadcast();
	return true;
}

void UStorageComponent::SaveToRecords(TArray<FItemSaveRecord>& OutRecords) const
{
	OutRecords = Items;
}

void UStorageComponent::LoadFromRecords(const TArray<FItemSaveRecord>& InRecords)
{
	Items = InRecords;
	CurrentWeight = 0.f;
	for (const FItemSaveRecord& Record : Items)
		CurrentWeight += Record.Weight;
	OnStorageChanged.Broadcast();
}