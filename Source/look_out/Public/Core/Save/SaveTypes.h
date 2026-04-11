#pragma once
#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveTypes.generated.h"

USTRUCT(BlueprintType)
struct FItemSaveRecord
{
    GENERATED_BODY()

    UPROPERTY(SaveGame) TSoftClassPtr<AActor> ItemClass;
    UPROPERTY(SaveGame) TArray<uint8> Bytes;
    UPROPERTY(SaveGame) float Weight = 0.f;
};

USTRUCT(BlueprintType)
struct FActorSaveRecord
{
    GENERATED_BODY()

    UPROPERTY(SaveGame) TSoftClassPtr<AActor> ActorClass;
    UPROPERTY(SaveGame) FVector Location;
    UPROPERTY(SaveGame) FRotator Rotation;
    UPROPERTY(SaveGame) FVector Scale = FVector::OneVector;
    UPROPERTY(SaveGame) TArray<uint8> Bytes;
};

USTRUCT(BlueprintType)
struct FPlayerSaveData
{
    GENERATED_BODY()

    UPROPERTY(SaveGame) FVector Location;
    UPROPERTY(SaveGame) FRotator Rotation;
    UPROPERTY(SaveGame) TArray<uint8> CharacterBytes;
    UPROPERTY(SaveGame) TArray<FItemSaveRecord> InventoryItems;
    UPROPERTY(SaveGame) TArray<int32> HotbarSlots;
    UPROPERTY(SaveGame) int32 ActiveSlotIndex = 0;
};

USTRUCT(BlueprintType)
struct FWorldStateData
{
    GENERATED_BODY()

    UPROPERTY(SaveGame, BlueprintReadWrite)
    TMap<FName, bool> Flags;

    UPROPERTY(SaveGame, BlueprintReadWrite)
    TMap<FName, int32> Counters;

    UPROPERTY(SaveGame, BlueprintReadWrite)
    float PlaytimeSeconds = 0.f;

    void SetFlag(FName Key, bool Value) { Flags.Add(Key, Value); }
    bool GetFlag(FName Key, bool Default = false) const
    {
        const bool* Val = Flags.Find(Key);
        return Val ? *Val : Default;
    }

    void IncrementCounter(FName Key, int32 Amount = 1)
    {
        int32& Val = Counters.FindOrAdd(Key, 0);
        Val += Amount;
    }
    int32 GetCounter(FName Key) const
    {
        const int32* Val = Counters.Find(Key);
        return Val ? *Val : 0;
    }
};

UCLASS()
class LOOK_OUT_API UGameSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY(SaveGame, BlueprintReadOnly)
    FString SlotName;

    UPROPERTY(SaveGame, BlueprintReadOnly)
    FDateTime SavedAt;

    UPROPERTY(SaveGame, BlueprintReadOnly)
    float PlaytimeSeconds = 0.f;

    UPROPERTY(SaveGame)
    TArray<FActorSaveRecord> ActorRecords;

    UPROPERTY(SaveGame, BlueprintReadWrite)
    FWorldStateData WorldState;

    UPROPERTY(SaveGame)
    FPlayerSaveData PlayerData;
};

UCLASS()
class LOOK_OUT_API UGlobalSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY(SaveGame, BlueprintReadOnly)
    float TotalPlaytimeSeconds = 0.f;

    UPROPERTY(SaveGame, BlueprintReadOnly)
    FDateTime FirstPlayedAt;

    UPROPERTY(SaveGame, BlueprintReadOnly)
    FDateTime LastPlayedAt;

    UPROPERTY(SaveGame, BlueprintReadWrite)
    TMap<FName, int32> GlobalCounters;

    UPROPERTY(SaveGame, BlueprintReadWrite)
    TMap<FName, bool> GlobalFlags;

    UPROPERTY(SaveGame, BlueprintReadOnly)
    int32 TotalRunsStarted = 0;

    void IncrementGlobalCounter(FName Key, int32 Amount = 1)
    {
        int32& Val = GlobalCounters.FindOrAdd(Key, 0);
        Val += Amount;
    }
    int32 GetGlobalCounter(FName Key) const
    {
        const int32* Val = GlobalCounters.Find(Key);
        return Val ? *Val : 0;
    }
    void SetGlobalFlag(FName Key, bool Value) { GlobalFlags.Add(Key, Value); }
    bool GetGlobalFlag(FName Key, bool Default = false) const
    {
        const bool* Val = GlobalFlags.Find(Key);
        return Val ? *Val : Default;
    }
};

USTRUCT(BlueprintType)
struct FSaveSlotMeta
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly) FString SlotName;
    UPROPERTY(BlueprintReadOnly) FDateTime SavedAt;
    UPROPERTY(BlueprintReadOnly) float PlaytimeSeconds = 0.f;
};

UCLASS()
class LOOK_OUT_API USaveSlotRegistry : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY(SaveGame)
    TArray<FSaveSlotMeta> Slots;
};