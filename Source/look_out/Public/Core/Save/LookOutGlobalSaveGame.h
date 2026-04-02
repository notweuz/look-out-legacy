// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/Save/SaveSlotMetadata.h"
#include "GameFramework/SaveGame.h"
#include "LookOutGlobalSaveGame.generated.h"

UCLASS(BlueprintType)
class LOOK_OUT_API ULookOutGlobalSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Save System")
	TArray<FSaveSlotMetadata> SaveSlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Global Data")
	TMap<FName, int32> IntValues;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Global Data")
	TMap<FName, float> FloatValues;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Global Data")
	TMap<FName, bool> BoolValues;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Global Data")
	TMap<FName, FString> StringValues;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Global Data")
	TMap<FName, FName> NameValues;
};
