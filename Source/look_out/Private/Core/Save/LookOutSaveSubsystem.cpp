// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#include "Core/Save/LookOutSaveSubsystem.h"

#include "Core/Save/LookOutGlobalSaveGame.h"
#include "Core/Save/LookOutSlotSaveGame.h"
#include "Data/Save/SavedActorData.h"
#include "Components/ActorComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/Brush.h"
#include "Engine/LevelScriptActor.h"
#include "Engine/Level.h"
#include "EngineUtils.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/HUD.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/WorldSettings.h"
#include "Interfaces/Saveable.h"
#include "Kismet/GameplayStatics.h"
#include "Libraries/SaveSystemUtils.h"
#include "Misc/Guid.h"
#include "Misc/LogCategories.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/MemoryWriter.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

namespace LookOutSaveSubsystemPrivate
{
	constexpr TCHAR GlobalSaveSlotName[] = TEXT("LookOut_Global");
	constexpr uint32 SaveUserIndex = 0;

	class FSaveGameArchive final : public FObjectAndNameAsStringProxyArchive
	{
	public:
		explicit FSaveGameArchive(FArchive& InnerArchive)
			: FObjectAndNameAsStringProxyArchive(InnerArchive, true)
		{
			ArIsSaveGame = true;
			ArNoDelta = true;
		}
	};

	bool ShouldSkipActor(const AActor* Actor)
	{
		if (!IsValid(Actor) || Actor->IsActorBeingDestroyed())
		{
			return true;
		}

		if (Actor->HasAnyFlags(RF_Transient | RF_ClassDefaultObject | RF_ArchetypeObject))
		{
			return true;
		}

		if (Actor->IsA<AWorldSettings>() ||
			Actor->IsA<AGameModeBase>() ||
			Actor->IsA<AGameStateBase>() ||
			Actor->IsA<APlayerController>() ||
			Actor->IsA<APlayerState>() ||
			Actor->IsA<APawn>() ||
			Actor->IsA<AHUD>() ||
			Actor->IsA<APlayerStart>() ||
			Actor->IsA<ABrush>() ||
			Actor->IsA<ALevelScriptActor>())
		{
			return true;
		}

		const AActor* Owner = Actor->GetOwner();
		return IsValid(Owner) && (Owner->IsA<APawn>() || Owner->IsA<APlayerController>() || Owner->IsA<APlayerState>());
	}

	void SerializeObject(UObject* Object, TArray<uint8>& Bytes)
	{
		Bytes.Reset();

		if (!IsValid(Object))
		{
			return;
		}

		FMemoryWriter Writer(Bytes, true);
		FSaveGameArchive Archive(Writer);
		Object->Serialize(Archive);
	}

	void DeserializeObject(UObject* Object, const TArray<uint8>& Bytes)
	{
		if (!IsValid(Object) || Bytes.Num() == 0)
		{
			return;
		}

		FMemoryReader Reader(Bytes, true);
		FSaveGameArchive Archive(Reader);
		Object->Serialize(Archive);
	}
}

void ULookOutSaveSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LoadOrCreateGlobalSave();
}

FString ULookOutSaveSubsystem::CreateSaveSlot(const FString& DisplayName)
{
	ULookOutSlotSaveGame* SlotSave = NewObject<ULookOutSlotSaveGame>();
	if (!SlotSave)
	{
		return FString();
	}

	SlotSave->SlotId = FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphens);
	SlotSave->DisplayName = SanitizeDisplayName(DisplayName);
	SlotSave->MapName = GetCurrentMapName();
	SlotSave->CreatedAt = FDateTime::UtcNow();
	SlotSave->UpdatedAt = SlotSave->CreatedAt;

	if (!CaptureWorldState(*SlotSave))
	{
		return FString();
	}

	if (!UGameplayStatics::SaveGameToSlot(SlotSave, BuildSlotStorageName(SlotSave->SlotId),
	                                      LookOutSaveSubsystemPrivate::SaveUserIndex))
	{
		return FString();
	}

	CurrentSlotId = SlotSave->SlotId;

	ULookOutGlobalSaveGame* GlobalSave = LoadOrCreateGlobalSave();
	if (GlobalSave)
	{
		FSaveSlotMetadata NewMetadata;
		NewMetadata.SlotId = SlotSave->SlotId;
		NewMetadata.DisplayName = SlotSave->DisplayName;
		NewMetadata.MapName = SlotSave->MapName;
		NewMetadata.CreatedAt = SlotSave->CreatedAt;
		NewMetadata.UpdatedAt = SlotSave->UpdatedAt;
		GlobalSave->SaveSlots.Add(NewMetadata);
		SaveGlobalSave();
	}

	UE_LOG(LogSaveGame, Log, TEXT("Created save slot '%s' (%s)"), *SlotSave->DisplayName, *SlotSave->SlotId);
	return SlotSave->SlotId;
}

bool ULookOutSaveSubsystem::SaveSlot(const FString& SlotId)
{
	if (SlotId.IsEmpty())
	{
		return false;
	}

	ULookOutSlotSaveGame* SlotSave = LoadSlotObject(SlotId);
	if (!SlotSave)
	{
		SlotSave = NewObject<ULookOutSlotSaveGame>();
		if (!SlotSave)
		{
			return false;
		}

		SlotSave->SlotId = SlotId;
		SlotSave->DisplayName = SlotId;
		SlotSave->CreatedAt = FDateTime::UtcNow();
	}

	SlotSave->MapName = GetCurrentMapName();
	SlotSave->UpdatedAt = FDateTime::UtcNow();

	if (!CaptureWorldState(*SlotSave))
	{
		return false;
	}

	const bool bSaved = UGameplayStatics::SaveGameToSlot(SlotSave, BuildSlotStorageName(SlotId),
	                                                     LookOutSaveSubsystemPrivate::SaveUserIndex);
	if (!bSaved)
	{
		return false;
	}

	CurrentSlotId = SlotId;
	if (ULookOutGlobalSaveGame* GlobalSave = LoadOrCreateGlobalSave())
	{
		FSaveSlotMetadata* Metadata = FindSlotMetadata(SlotId);
		if (!Metadata)
		{
			FSaveSlotMetadata NewMetadata;
			NewMetadata.SlotId = SlotSave->SlotId;
			NewMetadata.DisplayName = SlotSave->DisplayName;
			NewMetadata.MapName = SlotSave->MapName;
			NewMetadata.CreatedAt = SlotSave->CreatedAt;
			NewMetadata.UpdatedAt = SlotSave->UpdatedAt;
			GlobalSave->SaveSlots.Add(NewMetadata);
		}
		else
		{
			Metadata->DisplayName = SlotSave->DisplayName;
			Metadata->MapName = SlotSave->MapName;
			Metadata->CreatedAt = SlotSave->CreatedAt;
			Metadata->UpdatedAt = SlotSave->UpdatedAt;
		}

		SaveGlobalSave();
	}

	UE_LOG(LogSaveGame, Log, TEXT("Saved slot '%s'"), *SlotId);
	return true;
}

bool ULookOutSaveSubsystem::SaveCurrentSlot()
{
	return SaveSlot(CurrentSlotId);
}

bool ULookOutSaveSubsystem::LoadSlot(const FString& SlotId)
{
	ULookOutSlotSaveGame* SlotSave = LoadSlotObject(SlotId);
	if (!SlotSave)
	{
		return false;
	}

	const FString CurrentMapName = GetCurrentMapName();
	if (!SlotSave->MapName.IsEmpty() && !CurrentMapName.IsEmpty() && SlotSave->MapName != CurrentMapName)
	{
		UE_LOG(LogSaveGame, Warning, TEXT("Save slot '%s' belongs to map '%s', current map is '%s'"),
		       *SlotId, *SlotSave->MapName, *CurrentMapName);
		return false;
	}

	if (!ApplyWorldState(*SlotSave))
	{
		return false;
	}

	CurrentSlotId = SlotId;
	UE_LOG(LogSaveGame, Log, TEXT("Loaded slot '%s'"), *SlotId);
	return true;
}

bool ULookOutSaveSubsystem::DeleteSlot(const FString& SlotId)
{
	if (SlotId.IsEmpty())
	{
		return false;
	}

	const bool bDeleted = UGameplayStatics::DeleteGameInSlot(BuildSlotStorageName(SlotId),
	                                                         LookOutSaveSubsystemPrivate::SaveUserIndex);
	if (!bDeleted)
	{
		return false;
	}

	if (ULookOutGlobalSaveGame* GlobalSave = LoadOrCreateGlobalSave())
	{
		GlobalSave->SaveSlots.RemoveAll([&SlotId](const FSaveSlotMetadata& Entry)
		{
			return Entry.SlotId == SlotId;
		});
		SaveGlobalSave();
	}

	if (CurrentSlotId == SlotId)
	{
		CurrentSlotId.Reset();
	}

	return true;
}

void ULookOutSaveSubsystem::SetCurrentSlotId(const FString& SlotId)
{
	CurrentSlotId = SlotId;
}

FString ULookOutSaveSubsystem::GetCurrentSlotId() const
{
	return CurrentSlotId;
}

TArray<FSaveSlotMetadata> ULookOutSaveSubsystem::GetAllSaveSlots() const
{
	if (!CachedGlobalSave)
	{
		return {};
	}

	return CachedGlobalSave->SaveSlots;
}

ULookOutGlobalSaveGame* ULookOutSaveSubsystem::GetGlobalSave()
{
	return LoadOrCreateGlobalSave();
}

bool ULookOutSaveSubsystem::SaveGlobalSave()
{
	ULookOutGlobalSaveGame* GlobalSave = LoadOrCreateGlobalSave();
	return IsValid(GlobalSave) &&
		UGameplayStatics::SaveGameToSlot(GlobalSave, LookOutSaveSubsystemPrivate::GlobalSaveSlotName,
		                                 LookOutSaveSubsystemPrivate::SaveUserIndex);
}

void ULookOutSaveSubsystem::SetGlobalInt(const FName Key, const int32 Value)
{
	if (ULookOutGlobalSaveGame* GlobalSave = LoadOrCreateGlobalSave())
	{
		GlobalSave->IntValues.FindOrAdd(Key) = Value;
	}
}

int32 ULookOutSaveSubsystem::GetGlobalInt(const FName Key, const int32 DefaultValue) const
{
	if (!CachedGlobalSave)
	{
		return DefaultValue;
	}

	if (const int32* Value = CachedGlobalSave->IntValues.Find(Key))
	{
		return *Value;
	}

	return DefaultValue;
}

void ULookOutSaveSubsystem::SetGlobalFloat(const FName Key, const float Value)
{
	if (ULookOutGlobalSaveGame* GlobalSave = LoadOrCreateGlobalSave())
	{
		GlobalSave->FloatValues.FindOrAdd(Key) = Value;
	}
}

float ULookOutSaveSubsystem::GetGlobalFloat(const FName Key, const float DefaultValue) const
{
	if (!CachedGlobalSave)
	{
		return DefaultValue;
	}

	if (const float* Value = CachedGlobalSave->FloatValues.Find(Key))
	{
		return *Value;
	}

	return DefaultValue;
}

void ULookOutSaveSubsystem::SetGlobalBool(const FName Key, const bool Value)
{
	if (ULookOutGlobalSaveGame* GlobalSave = LoadOrCreateGlobalSave())
	{
		GlobalSave->BoolValues.FindOrAdd(Key) = Value;
	}
}

bool ULookOutSaveSubsystem::GetGlobalBool(const FName Key, const bool DefaultValue) const
{
	if (!CachedGlobalSave)
	{
		return DefaultValue;
	}

	if (const bool* Value = CachedGlobalSave->BoolValues.Find(Key))
	{
		return *Value;
	}

	return DefaultValue;
}

void ULookOutSaveSubsystem::SetGlobalString(const FName Key, const FString& Value)
{
	if (ULookOutGlobalSaveGame* GlobalSave = LoadOrCreateGlobalSave())
	{
		GlobalSave->StringValues.FindOrAdd(Key) = Value;
	}
}

FString ULookOutSaveSubsystem::GetGlobalString(const FName Key, const FString& DefaultValue) const
{
	if (!CachedGlobalSave)
	{
		return DefaultValue;
	}

	if (const FString* Value = CachedGlobalSave->StringValues.Find(Key))
	{
		return *Value;
	}

	return DefaultValue;
}

void ULookOutSaveSubsystem::SetGlobalName(const FName Key, const FName Value)
{
	if (ULookOutGlobalSaveGame* GlobalSave = LoadOrCreateGlobalSave())
	{
		GlobalSave->NameValues.FindOrAdd(Key) = Value;
	}
}

FName ULookOutSaveSubsystem::GetGlobalName(const FName Key, const FName DefaultValue) const
{
	if (!CachedGlobalSave)
	{
		return DefaultValue;
	}

	if (const FName* Value = CachedGlobalSave->NameValues.Find(Key))
	{
		return *Value;
	}

	return DefaultValue;
}

FString ULookOutSaveSubsystem::BuildSlotStorageName(const FString& SlotId)
{
	return FString::Printf(TEXT("LookOut_Slot_%s"), *SlotId);
}

FString ULookOutSaveSubsystem::SanitizeDisplayName(const FString& DisplayName)
{
	const FString Trimmed = DisplayName.TrimStartAndEnd();
	return Trimmed.IsEmpty() ? TEXT("New Save") : Trimmed;
}

bool ULookOutSaveSubsystem::CanSerializeComponent(const UActorComponent* Component)
{
	return IsValid(Component) &&
		!Component->HasAnyFlags(RF_Transient | RF_ClassDefaultObject | RF_ArchetypeObject) &&
		Component->GetFName() != NAME_None;
}

bool ULookOutSaveSubsystem::CaptureWorldState(ULookOutSlotSaveGame& SlotSave) const
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}

	SlotSave.SavedActors.Reset();

	for (TActorIterator<AActor> ActorIterator(World); ActorIterator; ++ActorIterator)
	{
		AActor* Actor = *ActorIterator;
		if (LookOutSaveSubsystemPrivate::ShouldSkipActor(Actor))
		{
			continue;
		}

		FSavedActorData SavedActorData;
		if (SnapshotActor(Actor, SavedActorData))
		{
			SlotSave.SavedActors.Add(MoveTemp(SavedActorData));
		}
	}

	return true;
}

bool ULookOutSaveSubsystem::ApplyWorldState(const ULookOutSlotSaveGame& SlotSave) const
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}

	TMap<FString, AActor*> ExistingActorsById;
	for (TActorIterator<AActor> ActorIterator(World); ActorIterator; ++ActorIterator)
	{
		AActor* Actor = *ActorIterator;
		if (LookOutSaveSubsystemPrivate::ShouldSkipActor(Actor))
		{
			continue;
		}

		const FString ActorId = ResolveActorId(Actor);
		if (!ActorId.IsEmpty())
		{
			ExistingActorsById.Add(ActorId, Actor);
		}
	}

	TSet<FString> SavedActorIds;
	for (const FSavedActorData& SavedActorData : SlotSave.SavedActors)
	{
		if (!SavedActorData.ActorId.IsEmpty())
		{
			SavedActorIds.Add(SavedActorData.ActorId);
		}
	}

	for (const TPair<FString, AActor*>& Entry : ExistingActorsById)
	{
		AActor* Actor = Entry.Value;
		if (!IsValid(Actor))
		{
			continue;
		}

		if (!SavedActorIds.Contains(Entry.Key))
		{
			Actor->Destroy();
		}
	}

	for (const FSavedActorData& SavedActorData : SlotSave.SavedActors)
	{
		AActor* Actor = ExistingActorsById.FindRef(SavedActorData.ActorId);
		if (!IsValid(Actor))
		{
			Actor = SpawnActorFromSaveData(SavedActorData);
		}

		if (!IsValid(Actor) || !RestoreActor(Actor, SavedActorData))
		{
			UE_LOG(LogSaveGame, Warning, TEXT("Failed to restore actor '%s'"), *SavedActorData.ActorId);
		}
	}

	return true;
}

bool ULookOutSaveSubsystem::SnapshotActor(AActor* Actor, FSavedActorData& OutActorData) const
{
	if (!IsValid(Actor))
	{
		return false;
	}

	OutActorData.ActorId = ResolveActorId(Actor);
	if (OutActorData.ActorId.IsEmpty())
	{
		return false;
	}

	OutActorData.ActorClass = Actor->GetClass();
	OutActorData.Transform = Actor->GetActorTransform();
	OutActorData.LevelPath = FSaveSystemUtils::NormalizeObjectPath(
		Actor->GetLevel() ? Actor->GetLevel()->GetPathName() : FString());
	OutActorData.bWasPlacedInLevel = FSaveSystemUtils::IsLevelPlacedActor(Actor);
	OutActorData.bImplementsSaveable = Actor->GetClass()->ImplementsInterface(USaveable::StaticClass());

	LookOutSaveSubsystemPrivate::SerializeObject(Actor, OutActorData.SaveGameData);

	if (OutActorData.bImplementsSaveable)
	{
		ISaveable::Execute_OnSave(Actor, OutActorData.CustomSaveData);
	}

	TInlineComponentArray<UActorComponent*> Components;
	Actor->GetComponents(Components);

	for (UActorComponent* Component : Components)
	{
		if (!CanSerializeComponent(Component))
		{
			continue;
		}

		FActorComponentSaveData ComponentSaveData;
		ComponentSaveData.ComponentName = Component->GetFName();
		LookOutSaveSubsystemPrivate::SerializeObject(Component, ComponentSaveData.ComponentData);
		OutActorData.SavedComponents.Add(MoveTemp(ComponentSaveData));
	}

	return true;
}

bool ULookOutSaveSubsystem::RestoreActor(AActor* Actor, const FSavedActorData& SavedActorData) const
{
	if (!IsValid(Actor))
	{
		return false;
	}

	Actor->SetActorTransform(SavedActorData.Transform, false, nullptr, ETeleportType::TeleportPhysics);
	LookOutSaveSubsystemPrivate::DeserializeObject(Actor, SavedActorData.SaveGameData);

	for (const FActorComponentSaveData& SavedComponent : SavedActorData.SavedComponents)
	{
		TInlineComponentArray<UActorComponent*> Components;
		Actor->GetComponents(Components);

		UActorComponent* const* ExistingComponentPtr = Components.FindByPredicate(
			[&SavedComponent](const UActorComponent* Component)
			{
				return IsValid(Component) && Component->GetFName() == SavedComponent.ComponentName;
			});

		if (ExistingComponentPtr && IsValid(*ExistingComponentPtr))
		{
			LookOutSaveSubsystemPrivate::DeserializeObject(*ExistingComponentPtr, SavedComponent.ComponentData);
		}
	}

	if (SavedActorData.bImplementsSaveable && Actor->GetClass()->ImplementsInterface(USaveable::StaticClass()))
	{
		ISaveable::Execute_OnLoad(Actor, SavedActorData.CustomSaveData);
	}

	return true;
}

AActor* ULookOutSaveSubsystem::SpawnActorFromSaveData(const FSavedActorData& SavedActorData) const
{
	UWorld* World = GetWorld();
	if (!World || SavedActorData.ActorClass.IsNull())
	{
		return nullptr;
	}

	UClass* ActorClass = SavedActorData.ActorClass.TryLoadClass<AActor>();
	if (!ActorClass)
	{
		return nullptr;
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* SpawnedActor = World->SpawnActor<AActor>(ActorClass, SavedActorData.Transform, SpawnParameters);
	return SpawnedActor;
}

FString ULookOutSaveSubsystem::ResolveActorId(const AActor* Actor) const
{
	if (!IsValid(Actor))
	{
		return FString();
	}

	if (FSaveSystemUtils::IsLevelPlacedActor(Actor))
	{
		return FSaveSystemUtils::BuildStableLevelActorId(Actor);
	}

	if (Actor->GetClass()->ImplementsInterface(USaveable::StaticClass()))
	{
		return ISaveable::Execute_GetSaveID(const_cast<AActor*>(Actor));
	}

	return FString();
}

FString ULookOutSaveSubsystem::GetCurrentMapName() const
{
	if (const UWorld* World = GetWorld())
	{
		return FSaveSystemUtils::NormalizeObjectPath(World->GetOutermost()->GetName());
	}

	return FString();
}

ULookOutSlotSaveGame* ULookOutSaveSubsystem::LoadSlotObject(const FString& SlotId) const
{
	if (SlotId.IsEmpty())
	{
		return nullptr;
	}

	return Cast<ULookOutSlotSaveGame>(UGameplayStatics::LoadGameFromSlot(
		BuildSlotStorageName(SlotId), LookOutSaveSubsystemPrivate::SaveUserIndex));
}

ULookOutGlobalSaveGame* ULookOutSaveSubsystem::LoadOrCreateGlobalSave()
{
	if (CachedGlobalSave)
	{
		return CachedGlobalSave;
	}

	CachedGlobalSave = Cast<ULookOutGlobalSaveGame>(UGameplayStatics::LoadGameFromSlot(
		LookOutSaveSubsystemPrivate::GlobalSaveSlotName, LookOutSaveSubsystemPrivate::SaveUserIndex));

	if (!CachedGlobalSave)
	{
		CachedGlobalSave = NewObject<ULookOutGlobalSaveGame>();
	}

	return CachedGlobalSave;
}

FSaveSlotMetadata* ULookOutSaveSubsystem::FindSlotMetadata(const FString& SlotId)
{
	ULookOutGlobalSaveGame* GlobalSave = LoadOrCreateGlobalSave();
	if (!GlobalSave)
	{
		return nullptr;
	}

	return GlobalSave->SaveSlots.FindByPredicate([&SlotId](const FSaveSlotMetadata& Entry)
	{
		return Entry.SlotId == SlotId;
	});
}

const FSaveSlotMetadata* ULookOutSaveSubsystem::FindSlotMetadata(const FString& SlotId) const
{
	if (!CachedGlobalSave)
	{
		return nullptr;
	}

	return CachedGlobalSave->SaveSlots.FindByPredicate([&SlotId](const FSaveSlotMetadata& Entry)
	{
		return Entry.SlotId == SlotId;
	});
}
