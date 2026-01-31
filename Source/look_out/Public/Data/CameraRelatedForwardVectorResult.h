#pragma once

#include "CoreMinimal.h"
#include "CameraRelatedForwardVectorResult.generated.h"

USTRUCT(BlueprintType)
struct FCameraRelatedForwardVectorResult
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	FVector StartVector;
	
	UPROPERTY(BlueprintReadOnly)
	FVector EndVector;
};