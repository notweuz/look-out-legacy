#include "Core/Save/SaveManager.h"

#include "Camera/CameraComponent.h"
#include "Characters/BaseCharacter.h"
#include "Characters/Components/PlayerInventoryComponent.h"
#include "Commandlets/WorldPartitionCommandletHelpers.h"
#include "Engine/AssetManager.h"
#include "Interfaces/Saveable.h"
#include "Utils/SaveUtils.h"
#include "Kismet/GameplayStatics.h"

const FString USaveManager::RegistrySlot = TEXT("SaveRegistry");
const FString USaveManager::GlobalSlot = TEXT("GlobalSave");

void USaveManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LoadRegistry();
	LoadGlobal();

	SessionStartTime = FDateTime::Now();
	GlobalSave->LastPlayedAt = SessionStartTime;

	if (GlobalSave->FirstPlayedAt.GetTicks() == 0)
		GlobalSave->FirstPlayedAt = SessionStartTime;

	SaveGlobal();
}

void USaveManager::Deinitialize()
{
	GlobalSave->TotalPlaytimeSeconds += CalcSessionTime();
	SaveGlobal();

	Super::Deinitialize();
}


void USaveManager::CreateSave(const FString& BaseName)
{
	const FString SlotName = GenerateSlotName(BaseName);

	UGameSaveGame* SaveGame = Cast<UGameSaveGame>(
		UGameplayStatics::CreateSaveGameObject(UGameSaveGame::StaticClass()));

	SaveGame->SlotName = SlotName;
	SaveGame->SavedAt = FDateTime::Now();
	SaveGame->PlaytimeSeconds = 0.f;

	UGameplayStatics::SaveGameToSlot(SaveGame, SlotName, 0);

	FSaveSlotMeta Meta;
	Meta.SlotName = SlotName;
	Meta.SavedAt = SaveGame->SavedAt;
	Meta.PlaytimeSeconds = 0.f;
	Registry->Slots.Add(Meta);
	SaveRegistry();

	++GlobalSave->TotalRunsStarted;
	SaveGlobal();

	ActiveSlotName = SlotName;
	OnSaveSlotsChanged.Broadcast(Registry->Slots);
}

void USaveManager::LoadSave(const FString& SlotName)
{
	UGameSaveGame* SaveGame = Cast<UGameSaveGame>(
		UGameplayStatics::LoadGameFromSlot(SlotName, 0));

	if (!SaveGame)
	{
		UE_LOG(LogTemp, Warning, TEXT("USaveManager::LoadSave — slot '%s' not found"), *SlotName);
		return;
	}

	ActiveSlotName = SlotName;
	SessionStartTime = FDateTime::Now();

	if (SaveGame->bFirstStart) return;
	RestoreWorldData(SaveGame);
}

void USaveManager::SaveCurrent()
{
	if (ActiveSlotName.IsEmpty()) return;

	UGameSaveGame* SaveGame = LoadSlotOrNull(ActiveSlotName);
	if (!SaveGame) return;

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	ABaseCharacter* Player = Cast<ABaseCharacter>(PC->GetPawn());
	if (!Player) return;

	SaveGame->SavedAt = FDateTime::Now();
	SaveGame->PlaytimeSeconds += CalcSessionTime();
	SaveGame->bFirstStart = false;
	SessionStartTime = FDateTime::Now();

	CollectWorldData(SaveGame);
	CollectPlayerData(SaveGame, Player);

	UGameplayStatics::SaveGameToSlot(SaveGame, ActiveSlotName, 0);
	FlushSlotMeta(SaveGame);
	SaveRegistry();

	OnSaveSlotsChanged.Broadcast(Registry->Slots);
}

void USaveManager::DeleteSave(const FString& SlotName)
{
	UGameplayStatics::DeleteGameInSlot(SlotName, 0);

	Registry->Slots.RemoveAll([&SlotName](const FSaveSlotMeta& Meta)
	{
		return Meta.SlotName == SlotName;
	});
	SaveRegistry();

	OnSaveSlotsChanged.Broadcast(Registry->Slots);
}


void USaveManager::SetFlag(const FString& SlotName, FName Key, bool Value)
{
	if (UGameSaveGame* SaveGame = LoadSlotOrNull(SlotName))
	{
		SaveGame->WorldState.SetFlag(Key, Value);
		UGameplayStatics::SaveGameToSlot(SaveGame, SlotName, 0);
	}
}

void USaveManager::IncrementCounter(const FString& SlotName, FName Key, int32 Amount)
{
	if (UGameSaveGame* SaveGame = LoadSlotOrNull(SlotName))
	{
		SaveGame->WorldState.IncrementCounter(Key, Amount);
		UGameplayStatics::SaveGameToSlot(SaveGame, SlotName, 0);
	}
}

bool USaveManager::GetFlag(const FString& SlotName, FName Key, bool Default)
{
	const UGameSaveGame* SaveGame = LoadSlotOrNull(SlotName);
	return SaveGame ? SaveGame->WorldState.GetFlag(Key, Default) : Default;
}

int32 USaveManager::GetCounter(const FString& SlotName, FName Key)
{
	const UGameSaveGame* SaveGame = LoadSlotOrNull(SlotName);
	return SaveGame ? SaveGame->WorldState.GetCounter(Key) : 0;
}


void USaveManager::IncrementGlobalCounter(FName Key, int32 Amount)
{
	GlobalSave->IncrementGlobalCounter(Key, Amount);
	SaveGlobal();
}

void USaveManager::SetGlobalFlag(FName Key, bool Value)
{
	GlobalSave->SetGlobalFlag(Key, Value);
	SaveGlobal();
}

int32 USaveManager::GetGlobalCounter(FName Key)
{
	return GlobalSave->GetGlobalCounter(Key);
}

bool USaveManager::GetGlobalFlag(FName Key, bool Default)
{
	return GlobalSave->GetGlobalFlag(Key, Default);
}


void USaveManager::CollectWorldData(UGameSaveGame* SaveGame)
{
	SaveGame->ActorRecords.Empty();

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USaveable::StaticClass(), Actors);

	AActor* ItemInHand = nullptr;
	if (ABaseCharacter* Player = Cast<ABaseCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn()))
	{
		if (UPlayerInventoryComponent* Inventory = Player->PlayerInventoryComponent)
		{
			ItemInHand = Inventory->ItemInHand;
		}
	}
	
	for (AActor* Actor : Actors)
	{
		if (Actor == ItemInHand) continue;
		
		FActorSaveRecord Record;
		Record.ActorClass = Actor->GetClass();
		Record.Location = Actor->GetActorLocation();
		Record.Rotation = Actor->GetActorRotation();
		Record.Scale = Actor->GetActorScale3D();
		ISaveable::Execute_OnSave(Actor, Record.Bytes);
		SaveGame->ActorRecords.Add(Record);
	}
}

void USaveManager::RestoreWorldData(UGameSaveGame* SaveGame)
{
	TArray<FSoftObjectPath> PathsToLoad;
	PathsToLoad.Reserve(SaveGame->ActorRecords.Num());
	for (const FActorSaveRecord& Record : SaveGame->ActorRecords)
	{
		if (!Record.ActorClass.IsNull())
			PathsToLoad.Add(Record.ActorClass.ToSoftObjectPath());
	}

	TArray<AActor*> ExistingActors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USaveable::StaticClass(), ExistingActors);
	for (AActor* Actor : ExistingActors)
		Actor->Destroy();

	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		PathsToLoad,
		[this, SaveGame]()
		{
			SpawnRestoredActors(SaveGame);
		});
}

void USaveManager::SpawnRestoredActors(UGameSaveGame* SaveGame)
{
	for (const FActorSaveRecord& Record : SaveGame->ActorRecords)
	{
		if (Record.ActorClass.IsNull())
		{
			UE_LOG(LogTemp, Warning, TEXT("SpawnRestoredActors: null ActorClass, skipping"));
			continue;
		}

		UClass* LoadedClass = Record.ActorClass.Get();
		if (!LoadedClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("SpawnRestoredActors: failed to resolve class '%s', skipping"),
			       *Record.ActorClass.ToString());
			continue;
		}

		AActor* Actor = GetWorld()->SpawnActor<AActor>(
			LoadedClass,
			FTransform(Record.Rotation, Record.Location, Record.Scale));

		if (!Actor)
		{
			UE_LOG(LogTemp, Warning, TEXT("SpawnRestoredActors: SpawnActor failed for class '%s'"),
			       *LoadedClass->GetName());
			continue;
		}

		ISaveable::Execute_OnLoad(Actor, Record.Bytes);
		ISaveable::Execute_OnPostLoadFromSave(Actor);
	}

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	ABaseCharacter* Player = PC ? Cast<ABaseCharacter>(PC->GetPawn()) : nullptr;

	if (Player)
	{
		RestorePlayerData(SaveGame, Player);
	}
	else
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("SpawnRestoredActors: could not find player pawn to restore — "
			       "make sure the pawn is spawned before LoadSave is called"));
	}
}


void USaveManager::CollectPlayerData(UGameSaveGame* SaveGame, ABaseCharacter* Player)
{
	FPlayerSaveData& Data = SaveGame->PlayerData;
	Data.Location = Player->GetActorLocation();
	Data.Rotation = Player->GetActorRotation();
	Data.CameraRotation = Player->Camera->GetRelativeLocation();

	if (UPlayerInventoryComponent* Inv = Player->FindComponentByClass<UPlayerInventoryComponent>())
	{
		Inv->SaveToRecords(Data.InventoryItems);
		Data.ActiveSlotIndex = Inv->ActiveSlotIndex;
	}
}

void USaveManager::RestorePlayerData(UGameSaveGame* SaveGame, ABaseCharacter* Player)
{
	const FPlayerSaveData& Data = SaveGame->PlayerData;

	if (SaveGame->bFirstStart)
	{
		UE_LOG(LogTemp, Log, TEXT("RestorePlayerData: no character bytes — fresh save, skipping"));
		return;
	}

	Player->SetActorLocation(Data.Location);
	Player->SetActorRotation(Data.Rotation);
	Player->Camera->SetRelativeLocation(Data.CameraRotation);

	if (UPlayerInventoryComponent* Inv = Player->FindComponentByClass<UPlayerInventoryComponent>())
	{
		Inv->LoadFromRecords(Data.InventoryItems);
		Inv->ActiveSlotIndex = Data.ActiveSlotIndex;
		Inv->EquipActiveItem();
	}
}


UGameSaveGame* USaveManager::LoadSlotOrNull(const FString& SlotName) const
{
	UGameSaveGame* SaveGame = Cast<UGameSaveGame>(
		UGameplayStatics::LoadGameFromSlot(SlotName, 0));

	if (!SaveGame)
		UE_LOG(LogTemp, Warning, TEXT("USaveManager::LoadSlotOrNull — slot '%s' not found"), *SlotName);

	return SaveGame;
}

void USaveManager::FlushSlotMeta(const UGameSaveGame* SaveGame)
{
	for (FSaveSlotMeta& Meta : Registry->Slots)
	{
		if (Meta.SlotName == SaveGame->SlotName)
		{
			Meta.SavedAt = SaveGame->SavedAt;
			Meta.PlaytimeSeconds = SaveGame->PlaytimeSeconds;
			return;
		}
	}
}

float USaveManager::CalcSessionTime() const
{
	return static_cast<float>((FDateTime::Now() - SessionStartTime).GetTotalSeconds());
}

FString USaveManager::GenerateSlotName(const FString& BaseName)
{
	return FString::Printf(TEXT("Save_%s"), *BaseName);
}


void USaveManager::SaveRegistry()
{
	UGameplayStatics::SaveGameToSlot(Registry, RegistrySlot, 0);
}

void USaveManager::LoadRegistry()
{
	Registry = Cast<USaveSlotRegistry>(UGameplayStatics::LoadGameFromSlot(RegistrySlot, 0));
	if (!Registry)
		Registry = Cast<USaveSlotRegistry>(
			UGameplayStatics::CreateSaveGameObject(USaveSlotRegistry::StaticClass()));
}

void USaveManager::SaveGlobal()
{
	UGameplayStatics::SaveGameToSlot(GlobalSave, GlobalSlot, 0);
}

void USaveManager::LoadGlobal()
{
	GlobalSave = Cast<UGlobalSaveGame>(UGameplayStatics::LoadGameFromSlot(GlobalSlot, 0));
	if (!GlobalSave)
		GlobalSave = Cast<UGlobalSaveGame>(
			UGameplayStatics::CreateSaveGameObject(UGlobalSaveGame::StaticClass()));
}
