// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ActorSaveData.generated.h"

USTRUCT(BlueprintType)
struct LOOK_OUT_API FActorSaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FGuid PersistentActorId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FTransform Transform = FTransform::Identity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	bool bHiddenInGame = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	bool bCollisionEnabled = true;
};
