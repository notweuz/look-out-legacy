// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TimeManagerSaveData.generated.h"

USTRUCT(BlueprintType)
struct LOOK_OUT_API FTimeManagerSaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FGuid SaveId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 Day = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	float Time = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	float DayLength = 3600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	float RealHoursPerDay = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	float TimeDilation = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	bool bTimeStopped = false;
};
