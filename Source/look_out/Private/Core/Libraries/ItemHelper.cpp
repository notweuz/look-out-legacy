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
		return FString::Empty();

	UClass* Class = Record.ItemClass.Get();
	if (!Class) return FString::Empty();

	UObject* Temp = NewObject<UObject>(GetTransientPackage(), Class);
	if (!Temp) return FString::Empty();

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
