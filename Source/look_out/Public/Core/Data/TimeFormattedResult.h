#pragma once

#include "CoreMinimal.h"
#include "TimeFormattedResult.generated.h"

USTRUCT(BlueprintType)
struct FTimeFormattedResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	int Day = 0;

	UPROPERTY(BlueprintReadOnly)
	int Hour = 0;

	UPROPERTY(BlueprintReadOnly)
	int Minute = 0;

	UPROPERTY(BlueprintReadOnly)
	int Second = 0;
};
