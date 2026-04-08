#pragma once
#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveTypes.generated.h"

USTRUCT(BlueprintType)
struct FActorSaveRecord
{
	GENERATED_BODY()

	UPROPERTY(SaveGame) FGuid SaveId;
	UPROPERTY(SaveGame) TSoftClassPtr<AActor> ActorClass;
	UPROPERTY(SaveGame) FVector Location;
	UPROPERTY(SaveGame) FRotator Rotation;
	UPROPERTY(SaveGame) FVector Scale = FVector::OneVector;
	UPROPERTY(SaveGame) TArray<uint8> Bytes;
};

USTRUCT(BlueprintType)
struct FItemSaveRecord
{
	GENERATED_BODY()

	UPROPERTY(SaveGame) TSoftClassPtr<AActor> ItemClass;
	UPROPERTY(SaveGame) TArray<uint8> Bytes;
};

UCLASS()
class LOOK_OUT_API UWorldSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame) TArray<FActorSaveRecord> ActorRecords;
	UPROPERTY(SaveGame) TArray<FGuid> DestroyedActorIds;
};

UCLASS()
class LOOK_OUT_API UPlayerSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame) FVector Location;
	UPROPERTY(SaveGame) FRotator Rotation;
	UPROPERTY(SaveGame) TArray<uint8> CharacterBytes;
	UPROPERTY(SaveGame) TArray<FItemSaveRecord> InventoryItems;
};