#include "Core/Save/SaveManager.h"

#include "Characters/BaseCharacter.h"
#include "Characters/Components/PlayerInventoryComponent.h"
#include "Engine/AssetManager.h"
#include "Interfaces/Saveable.h"
#include "Utils/SaveUtils.h"
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

void USaveManager::CreateSave(const FString& BaseName)
{
    FString SlotName = GenerateSlotName(BaseName);

    UGameSaveGame* SaveGame = Cast<UGameSaveGame>(
        UGameplayStatics::CreateSaveGameObject(UGameSaveGame::StaticClass())
    );
    SaveGame->SlotName        = SlotName;
    SaveGame->SavedAt         = FDateTime::Now();
    SaveGame->PlaytimeSeconds = 0.f;

    UGameplayStatics::SaveGameToSlot(SaveGame, SlotName, 0);

    FSaveSlotMeta Meta;
    Meta.SlotName        = SlotName;
    Meta.SavedAt         = SaveGame->SavedAt;
    Meta.PlaytimeSeconds = 0.f;
    Registry->Slots.Add(Meta);
    SaveRegistry();

    GlobalSave->TotalRunsStarted++;
    SaveGlobal();

    ActiveSlotName = SlotName;
    OnSaveSlotsChanged.Broadcast(Registry->Slots);
}

void USaveManager::LoadSave(const FString& SlotName)
{
    UGameSaveGame* SaveGame = Cast<UGameSaveGame>(
        UGameplayStatics::LoadGameFromSlot(SlotName, 0)
    );
    if (!SaveGame) return;

    ActiveSlotName = SlotName;
    SessionStartTime = FDateTime::Now();

    RestoreWorldData(SaveGame);

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (!PC) return;

    ABaseCharacter* Player = Cast<ABaseCharacter>(PC->GetPawn());
    if (!Player) return;

    RestorePlayerData(SaveGame, Player);
}

void USaveManager::SaveCurrent(ABaseCharacter* Player)
{
    if (ActiveSlotName.IsEmpty()) return;

    UGameSaveGame* SaveGame = Cast<UGameSaveGame>(
        UGameplayStatics::LoadGameFromSlot(ActiveSlotName, 0)
    );
    if (!SaveGame) return;

    SaveGame->SavedAt         = FDateTime::Now();
    SaveGame->PlaytimeSeconds += CalcSessionTime();

    CollectWorldData(SaveGame);
    CollectPlayerData(SaveGame, Player);

    UGameplayStatics::SaveGameToSlot(SaveGame, ActiveSlotName, 0);

    for (FSaveSlotMeta& Meta : Registry->Slots)
    {
        if (Meta.SlotName == ActiveSlotName)
        {
            Meta.SavedAt         = SaveGame->SavedAt;
            Meta.PlaytimeSeconds = SaveGame->PlaytimeSeconds;
            break;
        }
    }
    SaveRegistry();
    OnSaveSlotsChanged.Broadcast(Registry->Slots);
}

void USaveManager::DeleteSave(const FString& SlotName)
{
    UGameplayStatics::DeleteGameInSlot(SlotName, 0);

    Registry->Slots.RemoveAll([&](const FSaveSlotMeta& Meta) {
        return Meta.SlotName == SlotName;
    });
    SaveRegistry();

    OnSaveSlotsChanged.Broadcast(Registry->Slots);
}

void USaveManager::SetFlag(const FString& SlotName, FName Key, bool Value)
{
    UGameSaveGame* SaveGame = Cast<UGameSaveGame>(
        UGameplayStatics::LoadGameFromSlot(SlotName, 0)
    );
    if (!SaveGame) return;
    SaveGame->WorldState.SetFlag(Key, Value);
    UGameplayStatics::SaveGameToSlot(SaveGame, SlotName, 0);
}

void USaveManager::IncrementCounter(const FString& SlotName, FName Key, int32 Amount)
{
    UGameSaveGame* SaveGame = Cast<UGameSaveGame>(
        UGameplayStatics::LoadGameFromSlot(SlotName, 0)
    );
    if (!SaveGame) return;
    SaveGame->WorldState.IncrementCounter(Key, Amount);
    UGameplayStatics::SaveGameToSlot(SaveGame, SlotName, 0);
}

bool USaveManager::GetFlag(const FString& SlotName, FName Key, bool Default)
{
    UGameSaveGame* SaveGame = Cast<UGameSaveGame>(
        UGameplayStatics::LoadGameFromSlot(SlotName, 0)
    );
    if (!SaveGame) return Default;
    return SaveGame->WorldState.GetFlag(Key, Default);
}

int32 USaveManager::GetCounter(const FString& SlotName, FName Key)
{
    UGameSaveGame* SaveGame = Cast<UGameSaveGame>(
        UGameplayStatics::LoadGameFromSlot(SlotName, 0)
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
        Record.ActorClass  = Actor->GetClass();
        Record.Location    = Actor->GetActorLocation();
        Record.Rotation    = Actor->GetActorRotation();
        Record.Scale       = Actor->GetActorScale3D();
        ISaveable::Execute_OnSave(Actor, Record.Bytes);
        SaveGame->ActorRecords.Add(Record);
    }
}

void USaveManager::RestoreWorldData(UGameSaveGame* SaveGame)
{
    if (SaveGame->ActorRecords.IsEmpty())
        return;
    
    TArray<FSoftObjectPath> ClassesToLoad;
    for (const FActorSaveRecord& Record : SaveGame->ActorRecords)
    {
        if (!Record.ActorClass.IsNull())
            ClassesToLoad.Add(Record.ActorClass.ToSoftObjectPath());
    }

    TArray<AActor*> ExistingActors;
    UGameplayStatics::GetAllActorsWithInterface(
        GetWorld(), USaveable::StaticClass(), ExistingActors
    );
    for (AActor* Actor : ExistingActors)
        Actor->Destroy();

    TSharedPtr<FStreamableHandle> Handle = UAssetManager::GetStreamableManager()
        .RequestAsyncLoad(
            ClassesToLoad,
            [this, SaveGame]()
            {
                SpawnRestoredActors(SaveGame);
            }
        );
}

void USaveManager::SpawnRestoredActors(UGameSaveGame* SaveGame)
{
    for (const FActorSaveRecord& Record : SaveGame->ActorRecords)
    {
        if (Record.ActorClass.IsNull())
        {
            UE_LOG(LogTemp, Warning, TEXT("RestoreWorldData: null ActorClass, skipping"));
            continue;
        }

        UClass* LoadedClass = Record.ActorClass.Get();
        if (!LoadedClass)
        {
            UE_LOG(LogTemp, Warning,
                TEXT("RestoreWorldData: failed to load class %s, skipping"),
                *Record.ActorClass.ToString());
            continue;
        }

        AActor* Actor = GetWorld()->SpawnActor<AActor>(
            LoadedClass,
            FTransform(Record.Rotation, Record.Location, Record.Scale)
        );

        if (!Actor)
        {
            UE_LOG(LogTemp, Warning,
                TEXT("RestoreWorldData: SpawnActor failed for class %s"),
                *LoadedClass->GetName());
            continue;
        }

        ISaveable::Execute_OnLoad(Actor, Record.Bytes);
        ISaveable::Execute_OnPostLoadFromSave(Actor);
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
    
    if (!Data.CharacterBytes.IsEmpty())
    {
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
}

float USaveManager::CalcSessionTime() const
{
    FTimespan Duration = FDateTime::Now() - SessionStartTime;
    return static_cast<float>(Duration.GetTotalSeconds());
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
