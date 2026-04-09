#include "Core/Components/StorageComponent.h"

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

bool UStorageComponent::MoveItem(int32 FromIndex, int32 ToIndex)
{
	if (!Items.IsValidIndex(FromIndex) || !Items.IsValidIndex(ToIndex)) return false;
	if (FromIndex == ToIndex) return false;

	const FItemSaveRecord Record = Items[FromIndex];
	Items.RemoveAt(FromIndex);
	Items.Insert(Record, ToIndex);

	OnItemMoved(FromIndex, ToIndex);
	OnStorageChanged.Broadcast();
	return true;
}

bool UStorageComponent::MoveItemUp(int32 Index)
{
	return MoveItem(Index, Index - 1);
}

bool UStorageComponent::MoveItemDown(int32 Index)
{
	return MoveItem(Index, Index + 1);
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

void UStorageComponent::OnItemMoved(int32 FromIndex, int32 ToIndex)
{
}
