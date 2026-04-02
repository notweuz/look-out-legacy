// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SaveSlotMetadata.generated.h"

USTRUCT(BlueprintType)
struct LOOK_OUT_API FSaveSlotMetadata
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString SlotId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString MapName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FDateTime CreatedAt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FDateTime UpdatedAt;
};
