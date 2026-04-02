// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/Save/ActorComponentSaveData.h"
#include "UObject/SoftObjectPath.h"
#include "SavedActorData.generated.h"

USTRUCT(BlueprintType)
struct LOOK_OUT_API FSavedActorData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString ActorId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FSoftClassPath ActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FTransform Transform = FTransform::Identity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString LevelPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	bool bWasPlacedInLevel = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	bool bImplementsSaveable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<uint8> SaveGameData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<uint8> CustomSaveData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FActorComponentSaveData> SavedComponents;
};
