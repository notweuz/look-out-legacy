// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/Save/SavedActorData.h"
#include "GameFramework/SaveGame.h"
#include "LookOutSlotSaveGame.generated.h"

UCLASS(BlueprintType)
class LOOK_OUT_API ULookOutSlotSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Save System")
	FString SlotId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Save System")
	FString DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Save System")
	FString MapName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Save System")
	FDateTime CreatedAt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Save System")
	FDateTime UpdatedAt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Save System")
	TArray<FSavedActorData> SavedActors;
};
