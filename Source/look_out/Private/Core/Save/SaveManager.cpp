#include "Core/Save/SaveManager.h"

#include "Characters/BaseCharacter.h"
#include "Characters/Components/PlayerInventoryComponent.h"
#include "Interfaces/Saveable.h"
#include "Utils/SaveUtils.h"
#include "World/Objects/BaseObject.h"
#include "Kismet/GameplayStatics.h"

const FString USaveManager::RegistrySlot = "SaveRegistry";
const FString USaveManager::GlobalSlot   = "GlobalSave";

void USaveManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    LoadRegistry();
    LoadGlobal();

    SessionStartTime             = FDateTime::Now();
    GlobalSave->LastPlayedAt     = SessionStartTime;
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

void USaveManager::CreateSave(const FString& SlotName, ABaseCharacter* Player)
{
    FString SlotId = GenerateSlotId();

    UGameSaveGame* SaveGame = Cast<UGameSaveGame>(
        UGameplayStatics::CreateSaveGameObject(UGameSaveGame::StaticClass())
    );

    SaveGame->SlotName        = SlotName;
    SaveGame->SavedAt         = FDateTime::Now();
    SaveGame->PlaytimeSeconds = 0.f;

    CollectWorldData(SaveGame);
    CollectPlayerData(SaveGame, Player);

    UGameplayStatics::SaveGameToSlot(SaveGame, SlotId, 0);

    FSaveSlotMeta Meta;
    Meta.SlotId          = SlotId;
    Meta.SlotName        = SlotName;
    Meta.SavedAt         = SaveGame->SavedAt;
    Meta.PlaytimeSeconds = 0.f;
    Registry->Slots.Add(Meta);
    SaveRegistry();

    GlobalSave->TotalRunsStarted++;
    SaveGlobal();

    ActiveSlotId = SlotId;
    
    OnSaveSlotsChanged.Broadcast(Registry->Slots);
}

void USaveManager::OverwriteSave(const FString& SlotId, ABaseCharacter* Player)
{
    UGameSaveGame* SaveGame = Cast<UGameSaveGame>(
        UGameplayStatics::LoadGameFromSlot(SlotId, 0)
    );
    if (!SaveGame) return;

    float SessionTime              = CalcSessionTime();
    SaveGame->SavedAt              = FDateTime::Now();
    SaveGame->PlaytimeSeconds     += SessionTime;
    SaveGame->WorldState.PlaytimeSeconds = SaveGame->PlaytimeSeconds;

    CollectWorldData(SaveGame);
    CollectPlayerData(SaveGame, Player);

    UGameplayStatics::SaveGameToSlot(SaveGame, SlotId, 0);

    for (FSaveSlotMeta& Meta : Registry->Slots)
    {
        if (Meta.SlotId == SlotId)
        {
            Meta.SavedAt         = SaveGame->SavedAt;
            Meta.PlaytimeSeconds = SaveGame->PlaytimeSeconds;
            break;
        }
    }
    SaveRegistry();

    GlobalSave->TotalPlaytimeSeconds += SessionTime;
    SessionStartTime = FDateTime::Now();
    SaveGlobal();

    OnSaveSlotsChanged.Broadcast(Registry->Slots);
}

void USaveManager::LoadSave(const FString& SlotId, ABaseCharacter* Player)
{
    UGameSaveGame* SaveGame = Cast<UGameSaveGame>(
        UGameplayStatics::LoadGameFromSlot(SlotId, 0)
    );
    if (!SaveGame) return;

    ActiveSlotId = SlotId;
    
    RestoreWorldData(SaveGame);
    RestorePlayerData(SaveGame, Player);

    SessionStartTime = FDateTime::Now();
}

void USaveManager::DeleteSave(const FString& SlotId)
{
    UGameplayStatics::DeleteGameInSlot(SlotId, 0);

    Registry->Slots.RemoveAll([&](const FSaveSlotMeta& Meta) {
        return Meta.SlotId == SlotId;
    });
    SaveRegistry();

    OnSaveSlotsChanged.Broadcast(Registry->Slots);
}

void USaveManager::RegisterDestroyed(const FString& SlotId, FGuid SaveId)
{
    UGameSaveGame* SaveGame = Cast<UGameSaveGame>(
        UGameplayStatics::LoadGameFromSlot(SlotId, 0)
    );
    if (!SaveGame) return;

    SaveGame->DestroyedActorIds.AddUnique(SaveId);
    UGameplayStatics::SaveGameToSlot(SaveGame, SlotId, 0);
}

void USaveManager::SetFlag(const FString& SlotId, FName Key, bool Value)
{
    UGameSaveGame* SaveGame = Cast<UGameSaveGame>(
        UGameplayStatics::LoadGameFromSlot(SlotId, 0)
    );
    if (!SaveGame) return;
    SaveGame->WorldState.SetFlag(Key, Value);
    UGameplayStatics::SaveGameToSlot(SaveGame, SlotId, 0);
}

void USaveManager::IncrementCounter(const FString& SlotId, FName Key, int32 Amount)
{
    UGameSaveGame* SaveGame = Cast<UGameSaveGame>(
        UGameplayStatics::LoadGameFromSlot(SlotId, 0)
    );
    if (!SaveGame) return;
    SaveGame->WorldState.IncrementCounter(Key, Amount);
    UGameplayStatics::SaveGameToSlot(SaveGame, SlotId, 0);
}

bool USaveManager::GetFlag(const FString& SlotId, FName Key, bool Default)
{
    UGameSaveGame* SaveGame = Cast<UGameSaveGame>(
        UGameplayStatics::LoadGameFromSlot(SlotId, 0)
    );
    if (!SaveGame) return Default;
    return SaveGame->WorldState.GetFlag(Key, Default);
}

int32 USaveManager::GetCounter(const FString& SlotId, FName Key)
{
    UGameSaveGame* SaveGame = Cast<UGameSaveGame>(
        UGameplayStatics::LoadGameFromSlot(SlotId, 0)
    );
    if (!SaveGame) return 0;
    return SaveGame->WorldState.GetCounter(Key);
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

void USaveManager::DestroyActor(AActor* Actor)
{
    if (!Actor || !Actor->Implements<USaveable>()) return;

    FGuid SaveId = ISaveable::Execute_GetSaveId(Actor);
    RegisterDestroyed(ActiveSlotId, SaveId);
    Actor->Destroy();
}

void USaveManager::CollectWorldData(UGameSaveGame* SaveGame)
{
    SaveGame->ActorRecords.Empty();

    TArray<AActor*> Actors;
    UGameplayStatics::GetAllActorsWithInterface(
        GetWorld(), USaveable::StaticClass(), Actors
    );

    for (AActor* Actor : Actors)
    {
        FActorSaveRecord Record;
        Record.SaveId      = ISaveable::Execute_GetSaveId(Actor);
        Record.ActorClass  = Actor->GetClass();
        Record.Location    = Actor->GetActorLocation();
        Record.Rotation    = Actor->GetActorRotation();
        Record.Scale       = Actor->GetActorScale3D();
        Record.Persistence = GetActorPersistence(Actor);
        ISaveable::Execute_OnSave(Actor, Record.Bytes);
        SaveGame->ActorRecords.Add(Record);
    }

    TSet<FGuid> ActiveIds;
    for (const FActorSaveRecord& Record : SaveGame->ActorRecords)
        ActiveIds.Add(Record.SaveId);

    SaveGame->DestroyedActorIds = SaveGame->DestroyedActorIds.FilterByPredicate(
        [&](const FGuid& Id) { return ActiveIds.Contains(Id); }
    );
}

void USaveManager::RestoreWorldData(UGameSaveGame* SaveGame)
{
    TArray<AActor*> Actors;
    UGameplayStatics::GetAllActorsWithInterface(
        GetWorld(), USaveable::StaticClass(), Actors
    );

    TMap<FGuid, AActor*> ActorMap;
    for (AActor* Actor : Actors)
        ActorMap.Add(ISaveable::Execute_GetSaveId(Actor), Actor);

    TSet<FGuid> SavedIds;
    for (const FActorSaveRecord& Record : SaveGame->ActorRecords)
        SavedIds.Add(Record.SaveId);

    for (AActor* Actor : Actors)
    {
        FGuid Id = ISaveable::Execute_GetSaveId(Actor);
        if (!SavedIds.Contains(Id) &&
            GetActorPersistence(Actor) == EActorPersistence::Placed)
        {
            Actor->Destroy();
        }
    }

    for (const FActorSaveRecord& Record : SaveGame->ActorRecords)
    {
        if (SaveGame->DestroyedActorIds.Contains(Record.SaveId)) continue;

        AActor* Target = ActorMap.FindRef(Record.SaveId);

        const bool bWasSpawnedDeferred = !Target;
        if (bWasSpawnedDeferred)
        {
            Target = GetWorld()->SpawnActorDeferred<AActor>(
                Record.ActorClass.LoadSynchronous(),
                FTransform(Record.Rotation, Record.Location, Record.Scale)
            );
        }

        if (!Target)
        {
            continue;
        }

        Target->SetActorLocation(Record.Location);
        Target->SetActorRotation(Record.Rotation);
        Target->SetActorScale3D(Record.Scale);

        if (bWasSpawnedDeferred)
        {
            UGameplayStatics::FinishSpawningActor(
                Target,
                FTransform(Record.Rotation, Record.Location, Record.Scale)
            );
        }

        ISaveable::Execute_OnLoad(Target, Record.Bytes);
    }
}

void USaveManager::CollectPlayerData(UGameSaveGame* SaveGame, ABaseCharacter* Player)
{
    FPlayerSaveData& Data = SaveGame->PlayerData;
    Data.Location = Player->GetActorLocation();
    Data.Rotation = Player->GetActorRotation();
    SaveUtils::Save(Player, Data.CharacterBytes);

    UPlayerInventoryComponent* Inv =
        Player->FindComponentByClass<UPlayerInventoryComponent>();
    if (Inv)
    {
        Inv->SaveToRecords(Data.InventoryItems);
        Data.HotbarSlots     = Inv->HotbarSlots;
        Data.ActiveSlotIndex = Inv->ActiveSlotIndex;
    }
}

void USaveManager::RestorePlayerData(UGameSaveGame* SaveGame, ABaseCharacter* Player)
{
    const FPlayerSaveData& Data = SaveGame->PlayerData;
    Player->SetActorLocation(Data.Location);
    Player->SetActorRotation(Data.Rotation);
    SaveUtils::Load(Player, Data.CharacterBytes);

    UPlayerInventoryComponent* Inv =
        Player->FindComponentByClass<UPlayerInventoryComponent>();
    if (Inv)
    {
        Inv->LoadFromRecords(Data.InventoryItems);
        Inv->HotbarSlots     = Data.HotbarSlots;
        Inv->ActiveSlotIndex = Data.ActiveSlotIndex;
        Inv->EquipActiveItem();
    }
}

EActorPersistence USaveManager::GetActorPersistence(AActor* Actor) const
{
    if (ABaseObject* A = Cast<ABaseObject>(Actor))
        return A->Persistence;
    return EActorPersistence::Placed;
}

float USaveManager::CalcSessionTime() const
{
    FTimespan Duration = FDateTime::Now() - SessionStartTime;
    return static_cast<float>(Duration.GetTotalSeconds());
}

void USaveManager::SaveRegistry()
{
    UGameplayStatics::SaveGameToSlot(Registry, RegistrySlot, 0);
}

void USaveManager::LoadRegistry()
{
    Registry = Cast<USaveSlotRegistry>(
        UGameplayStatics::LoadGameFromSlot(RegistrySlot, 0)
    );
    if (!Registry)
        Registry = Cast<USaveSlotRegistry>(
            UGameplayStatics::CreateSaveGameObject(USaveSlotRegistry::StaticClass())
        );
}

void USaveManager::SaveGlobal()
{
    UGameplayStatics::SaveGameToSlot(GlobalSave, GlobalSlot, 0);
}

void USaveManager::LoadGlobal()
{
    GlobalSave = Cast<UGlobalSaveGame>(
        UGameplayStatics::LoadGameFromSlot(GlobalSlot, 0)
    );
    if (!GlobalSave)
        GlobalSave = Cast<UGlobalSaveGame>(
            UGameplayStatics::CreateSaveGameObject(UGlobalSaveGame::StaticClass())
        );
}

FString USaveManager::GenerateSlotId()
{
    return FString::Printf(TEXT("Save_%s"), *FGuid::NewGuid().ToString());
}
