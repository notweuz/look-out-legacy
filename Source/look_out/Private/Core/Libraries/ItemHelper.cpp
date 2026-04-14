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
	if (!TargetObject || Bytes.Num() == 0)
		return;

	FMemoryReader MemoryReader(Bytes, true);
	FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
	Archive.ArIsSaveGame = true;
	Archive.ArNoDelta = true;

	TargetObject->Serialize(Archive);
}

bool UItemHelper::GetSaveGameProperty(const FItemSaveRecord& Record, FName PropertyName, int32& OutValue)
{
	check(0);
	return false;
}

DEFINE_FUNCTION(UItemHelper::execGetSaveGameProperty)
{
	P_GET_STRUCT(FItemSaveRecord, Record);
	P_GET_PROPERTY(FNameProperty, PropertyName);

	Stack.StepCompiledIn<FProperty>(nullptr);
	void* OutValuePtr = Stack.MostRecentPropertyAddress;
	const FProperty* OutProperty = Stack.MostRecentProperty;

	P_FINISH;

	if (!OutValuePtr || !OutProperty || Record.ItemClass.IsNull() || Record.Bytes.Num() == 0)
	{
		*static_cast<bool*>(RESULT_PARAM) = false;
		return;
	}

	UClass* ItemClass = Record.ItemClass.Get();
	if (!ItemClass)
	{
		*static_cast<bool*>(RESULT_PARAM) = false;
		return;
	}

	UObject* TempObject = NewObject<UObject>(GetTransientPackage(), ItemClass);
	if (!TempObject)
	{
		*static_cast<bool*>(RESULT_PARAM) = false;
		return;
	}

	LoadSaveProperties(TempObject, Record.Bytes);

	if (const FProperty* SourceProp = ItemClass->FindPropertyByName(PropertyName))
	{
		SourceProp->CopySingleValue(OutValuePtr, SourceProp->ContainerPtrToValuePtr<void>(TempObject));

		TempObject->ConditionalBeginDestroy();
		*static_cast<bool*>(RESULT_PARAM) = true;
		return;
	}

	TempObject->ConditionalBeginDestroy();
	*static_cast<bool*>(RESULT_PARAM) = false;
}

void UItemHelper::GetItemStorage(const FItemSaveRecord& Record, 
								 TArray<FItemSaveRecord>& OutItems, 
								 float& OutCurrentWeight)
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

	LoadSaveProperties(TempActor, Record.Bytes);

	if (UStorageComponent* Storage = TempActor->FindComponentByClass<UStorageComponent>())
	{
		OutItems = Storage->Items;
		OutCurrentWeight = Storage->CurrentWeight;
	}

	TempActor->ConditionalBeginDestroy();
}