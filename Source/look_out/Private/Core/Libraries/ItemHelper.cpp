// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#include "Core/Libraries/ItemHelper.h"
#include "Core/Components/StorageComponent.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

UItemDefinition* UItemHelper::BuildItemDefinition(FText Name, FText Description, float Weight,
                                                  UTexture2D* Icon, TSubclassOf<AActor> ActorClass,
                                                  UObject* Outer, float MaxHealth)
{
	UObject* DefinitionOuter = IsValid(Outer) ? Outer : GetTransientPackage();

	UItemDefinition* Definition = NewObject<UItemDefinition>(
		DefinitionOuter,
		UItemDefinition::StaticClass(),
		NAME_None,
		RF_Transient
	);

	Definition->ActorClass = ActorClass;
	Definition->Weight = Weight;
	Definition->Icon = Icon;
	Definition->DisplayName = Name;
	Definition->Description = Description;
	Definition->MaxHealth = MaxHealth;

	return Definition;
}

void UItemHelper::LoadSaveProperties(UObject* TargetObject, const TArray<uint8>& Bytes)
{
	LoadSavePropertiesInternal(TargetObject, Bytes);
}

void UItemHelper::LoadSavePropertiesInternal(UObject* TargetObject, const TArray<uint8>& Bytes)
{
	if (!TargetObject || Bytes.Num() == 0) return;

	FMemoryReader MemoryReader(Bytes, true);
	FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
	Archive.ArIsSaveGame = true;
	Archive.ArNoDelta = true;

	TargetObject->Serialize(Archive);
}

float UItemHelper::GetFloatProperty(const FItemSaveRecord& Record, FName PropertyName)
{
	if (Record.ItemClass.IsNull() || Record.Bytes.Num() == 0)
		return 0.0f;

	UClass* Class = Record.ItemClass.Get();
	if (!Class) return 0.0f;

	UObject* Temp = NewObject<UObject>(GetTransientPackage(), Class);
	if (!Temp) return 0.0f;

	LoadSavePropertiesInternal(Temp, Record.Bytes);

	float Value = 0.0f;
	if (const FProperty* Prop = Class->FindPropertyByName(PropertyName))
	{
		Prop->GetValue_InContainer(Temp, &Value);
	}

	Temp->ConditionalBeginDestroy();
	return Value;
}

int32 UItemHelper::GetIntProperty(const FItemSaveRecord& Record, FName PropertyName)
{
	if (Record.ItemClass.IsNull() || Record.Bytes.Num() == 0)
		return 0;

	UClass* Class = Record.ItemClass.Get();
	if (!Class) return 0;

	UObject* Temp = NewObject<UObject>(GetTransientPackage(), Class);
	if (!Temp) return 0;

	LoadSavePropertiesInternal(Temp, Record.Bytes);

	int32 Value = 0;
	if (const FProperty* Prop = Class->FindPropertyByName(PropertyName))
	{
		Prop->GetValue_InContainer(Temp, &Value);
	}

	Temp->ConditionalBeginDestroy();
	return Value;
}

bool UItemHelper::GetBoolProperty(const FItemSaveRecord& Record, FName PropertyName)
{
	if (Record.ItemClass.IsNull() || Record.Bytes.Num() == 0)
		return false;

	UClass* Class = Record.ItemClass.Get();
	if (!Class) return false;

	UObject* Temp = NewObject<UObject>(GetTransientPackage(), Class);
	if (!Temp) return false;

	LoadSavePropertiesInternal(Temp, Record.Bytes);

	bool Value = false;
	if (const FProperty* Prop = Class->FindPropertyByName(PropertyName))
	{
		Prop->GetValue_InContainer(Temp, &Value);
	}

	Temp->ConditionalBeginDestroy();
	return Value;
}

FString UItemHelper::GetStringProperty(const FItemSaveRecord& Record, FName PropertyName)
{
	if (Record.ItemClass.IsNull() || Record.Bytes.Num() == 0)
		return FString();

	UClass* Class = Record.ItemClass.Get();
	if (!Class) return FString();

	UObject* Temp = NewObject<UObject>(GetTransientPackage(), Class);
	if (!Temp) return FString();

	LoadSavePropertiesInternal(Temp, Record.Bytes);

	FString Value;
	if (const FProperty* Prop = Class->FindPropertyByName(PropertyName))
	{
		Prop->GetValue_InContainer(Temp, &Value);
	}

	Temp->ConditionalBeginDestroy();
	return Value;
}

FText UItemHelper::GetTextProperty(const FItemSaveRecord& Record, FName PropertyName)
{
	if (Record.ItemClass.IsNull() || Record.Bytes.Num() == 0)
		return FText::GetEmpty();

	UClass* Class = Record.ItemClass.Get();
	if (!Class) return FText::GetEmpty();

	UObject* Temp = NewObject<UObject>(GetTransientPackage(), Class);
	if (!Temp) return FText::GetEmpty();

	LoadSavePropertiesInternal(Temp, Record.Bytes);

	FText Value;
	if (const FProperty* Prop = Class->FindPropertyByName(PropertyName))
	{
		Prop->GetValue_InContainer(Temp, &Value);
	}

	Temp->ConditionalBeginDestroy();
	return Value;
}

FName UItemHelper::GetNameProperty(const FItemSaveRecord& Record, FName PropertyName)
{
	if (Record.ItemClass.IsNull() || Record.Bytes.Num() == 0)
		return FName();

	UClass* Class = Record.ItemClass.Get();
	if (!Class) return FName();

	UObject* Temp = NewObject<UObject>(GetTransientPackage(), Class);
	if (!Temp) return FName();

	LoadSavePropertiesInternal(Temp, Record.Bytes);

	FName Value;
	if (const FProperty* Prop = Class->FindPropertyByName(PropertyName))
	{
		Prop->GetValue_InContainer(Temp, &Value);
	}

	Temp->ConditionalBeginDestroy();
	return Value;
}

void UItemHelper::GetStorageProperty(const FItemSaveRecord& Record, TArray<FItemSaveRecord>& OutItems, float& OutCurrentWeight)
{
	OutItems.Empty();
	OutCurrentWeight = 0.0f;

	if (Record.ItemClass.IsNull() || Record.Bytes.Num() == 0)
		return;

	UClass* Class = Record.ItemClass.Get();
	if (!Class || !Class->IsChildOf(AActor::StaticClass()))
		return;

	AActor* TempActor = NewObject<AActor>(GetTransientPackage(), Class);
	if (!TempActor)
		return;

	LoadSavePropertiesInternal(TempActor, Record.Bytes);

	if (UStorageComponent* Storage = TempActor->FindComponentByClass<UStorageComponent>())
	{
		OutItems = Storage->Items;
		OutCurrentWeight = Storage->CurrentWeight;
	}

	TempActor->ConditionalBeginDestroy();
}

void UItemHelper::SetFloatProperty(FItemSaveRecord& Record, FName PropertyName, float Value)
{
	if (Record.ItemClass.IsNull())
		return;

	UClass* Class = Record.ItemClass.Get();
	if (!Class) return;

	UObject* Temp = NewObject<UObject>(GetTransientPackage(), Class);
	if (!Temp) return;

	if (Record.Bytes.Num() > 0)
	{
		LoadSavePropertiesInternal(Temp, Record.Bytes);
	}

	if (FProperty* Prop = Class->FindPropertyByName(PropertyName))
	{
		Prop->SetValue_InContainer(Temp, &Value);
	}

	FMemoryWriter MemoryWriter(Record.Bytes, true);
	FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
	Archive.ArIsSaveGame = true;
	Archive.ArNoDelta = true;

	Temp->Serialize(Archive);
	Temp->ConditionalBeginDestroy();
}

void UItemHelper::SetIntProperty(FItemSaveRecord& Record, FName PropertyName, int32 Value)
{
	if (Record.ItemClass.IsNull())
		return;

	UClass* Class = Record.ItemClass.Get();
	if (!Class) return;

	UObject* Temp = NewObject<UObject>(GetTransientPackage(), Class);
	if (!Temp) return;

	if (Record.Bytes.Num() > 0)
	{
		LoadSavePropertiesInternal(Temp, Record.Bytes);
	}

	if (FProperty* Prop = Class->FindPropertyByName(PropertyName))
	{
		Prop->SetValue_InContainer(Temp, &Value);
	}

	FMemoryWriter MemoryWriter(Record.Bytes, true);
	FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
	Archive.ArIsSaveGame = true;
	Archive.ArNoDelta = true;

	Temp->Serialize(Archive);
	Temp->ConditionalBeginDestroy();
}

void UItemHelper::SetBoolProperty(FItemSaveRecord& Record, FName PropertyName, bool Value)
{
	if (Record.ItemClass.IsNull())
		return;

	UClass* Class = Record.ItemClass.Get();
	if (!Class) return;

	UObject* Temp = NewObject<UObject>(GetTransientPackage(), Class);
	if (!Temp) return;

	if (Record.Bytes.Num() > 0)
	{
		LoadSavePropertiesInternal(Temp, Record.Bytes);
	}

	if (FProperty* Prop = Class->FindPropertyByName(PropertyName))
	{
		Prop->SetValue_InContainer(Temp, &Value);
	}

	FMemoryWriter MemoryWriter(Record.Bytes, true);
	FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
	Archive.ArIsSaveGame = true;
	Archive.ArNoDelta = true;

	Temp->Serialize(Archive);
	Temp->ConditionalBeginDestroy();
}

void UItemHelper::SetStringProperty(FItemSaveRecord& Record, FName PropertyName, const FString& Value)
{
	if (Record.ItemClass.IsNull())
		return;

	UClass* Class = Record.ItemClass.Get();
	if (!Class) return;

	UObject* Temp = NewObject<UObject>(GetTransientPackage(), Class);
	if (!Temp) return;

	if (Record.Bytes.Num() > 0)
	{
		LoadSavePropertiesInternal(Temp, Record.Bytes);
	}

	if (FProperty* Prop = Class->FindPropertyByName(PropertyName))
	{
		Prop->SetValue_InContainer(Temp, &Value);
	}

	FMemoryWriter MemoryWriter(Record.Bytes, true);
	FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
	Archive.ArIsSaveGame = true;
	Archive.ArNoDelta = true;

	Temp->Serialize(Archive);
	Temp->ConditionalBeginDestroy();
}

void UItemHelper::SetTextProperty(FItemSaveRecord& Record, FName PropertyName, const FText& Value)
{
	if (Record.ItemClass.IsNull())
		return;

	UClass* Class = Record.ItemClass.Get();
	if (!Class) return;

	UObject* Temp = NewObject<UObject>(GetTransientPackage(), Class);
	if (!Temp) return;

	if (Record.Bytes.Num() > 0)
	{
		LoadSavePropertiesInternal(Temp, Record.Bytes);
	}

	if (FProperty* Prop = Class->FindPropertyByName(PropertyName))
	{
		Prop->SetValue_InContainer(Temp, &Value);
	}

	FMemoryWriter MemoryWriter(Record.Bytes, true);
	FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
	Archive.ArIsSaveGame = true;
	Archive.ArNoDelta = true;

	Temp->Serialize(Archive);
	Temp->ConditionalBeginDestroy();
}

void UItemHelper::SetNameProperty(FItemSaveRecord& Record, FName PropertyName, FName Value)
{
	if (Record.ItemClass.IsNull())
		return;

	UClass* Class = Record.ItemClass.Get();
	if (!Class) return;

	UObject* Temp = NewObject<UObject>(GetTransientPackage(), Class);
	if (!Temp) return;

	if (Record.Bytes.Num() > 0)
	{
		LoadSavePropertiesInternal(Temp, Record.Bytes);
	}

	if (FProperty* Prop = Class->FindPropertyByName(PropertyName))
	{
		Prop->SetValue_InContainer(Temp, &Value);
	}

	FMemoryWriter MemoryWriter(Record.Bytes, true);
	FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
	Archive.ArIsSaveGame = true;
	Archive.ArNoDelta = true;

	Temp->Serialize(Archive);
	Temp->ConditionalBeginDestroy();
}

bool UItemHelper::IsValidItemRecord(const FItemSaveRecord& Record)
{
	return !Record.ItemClass.IsNull() && Record.Bytes.Num() > 0;
}

bool UItemHelper::HasProperty(const FItemSaveRecord& Record, FName PropertyName)
{
	if (Record.ItemClass.IsNull())
		return false;

	UClass* Class = Record.ItemClass.Get();
	if (!Class) return false;

	return Class->FindPropertyByName(PropertyName) != nullptr;
}

TArray<FName> UItemHelper::GetAllPropertyNames(const FItemSaveRecord& Record)
{
	TArray<FName> PropertyNames;
	
	if (Record.ItemClass.IsNull())
		return PropertyNames;

	UClass* Class = Record.ItemClass.Get();
	if (!Class) return PropertyNames;

	for (TFieldIterator<FProperty> It(Class); It; ++It)
	{
		PropertyNames.Add(It->GetFName());
	}

	return PropertyNames;
}

float UItemHelper::GetItemWeight(const FItemSaveRecord& Record)
{
	return GetFloatProperty(Record, TEXT("Weight"));
}

FText UItemHelper::GetItemDisplayName(const FItemSaveRecord& Record)
{
	return GetTextProperty(Record, TEXT("DisplayName"));
}

FItemSaveRecord UItemHelper::CreateItemRecord(TSubclassOf<AActor> ItemClass)
{
	FItemSaveRecord Record;
	Record.ItemClass = ItemClass;
	Record.Bytes.Empty();
	return Record;
}

FItemSaveRecord UItemHelper::CreateItemRecordFromDefinition(UItemDefinition* ItemDef)
{
	FItemSaveRecord Record;
	if (IsValid(ItemDef))
	{
		Record.ItemClass = ItemDef->ActorClass;
		Record.Bytes.Empty();
	}
	return Record;
}

void UItemHelper::SetStorageProperty(FItemSaveRecord& Record, const TArray<FItemSaveRecord>& Items, float CurrentWeight)
{
	if (Record.ItemClass.IsNull())
		return;

	UClass* Class = Record.ItemClass.Get();
	if (!Class || !Class->IsChildOf(AActor::StaticClass()))
		return;

	AActor* TempActor = NewObject<AActor>(GetTransientPackage(), Class);
	if (!TempActor) return;

	if (Record.Bytes.Num() > 0)
	{
		LoadSavePropertiesInternal(TempActor, Record.Bytes);
	}

	if (UStorageComponent* Storage = TempActor->FindComponentByClass<UStorageComponent>())
	{
		Storage->Items = Items;
		Storage->CurrentWeight = CurrentWeight;

		FMemoryWriter MemoryWriter(Record.Bytes, true);
		FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
		Archive.ArIsSaveGame = true;
		Archive.ArNoDelta = true;

		TempActor->Serialize(Archive);
	}

	TempActor->ConditionalBeginDestroy();
}

int32 UItemHelper::GetStorageItemCount(const FItemSaveRecord& Record)
{
	TArray<FItemSaveRecord> Items;
	float Weight;
	GetStorageProperty(Record, Items, Weight);
	return Items.Num();
}

bool UItemHelper::IsStorageEmpty(const FItemSaveRecord& Record)
{
	return GetStorageItemCount(Record) == 0;
}

bool UItemHelper::AreItemsEqual(const FItemSaveRecord& RecordA, const FItemSaveRecord& RecordB)
{
	if (RecordA.ItemClass != RecordB.ItemClass)
		return false;

	if (RecordA.Bytes.Num() != RecordB.Bytes.Num())
		return false;

	return RecordA.Bytes == RecordB.Bytes;
}

bool UItemHelper::IsSameItemType(const FItemSaveRecord& RecordA, const FItemSaveRecord& RecordB)
{
	return RecordA.ItemClass == RecordB.ItemClass;
}
