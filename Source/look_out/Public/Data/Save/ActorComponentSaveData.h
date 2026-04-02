// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponentSaveData.generated.h"

USTRUCT(BlueprintType)
struct LOOK_OUT_API FActorComponentSaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FName ComponentName = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<uint8> ComponentData;
};
