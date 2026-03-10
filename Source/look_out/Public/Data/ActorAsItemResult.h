#pragma once

#include "CoreMinimal.h"
#include "Storage/Item.h"
#include "ActorAsItemResult.generated.h"

USTRUCT(BlueprintType)
struct FActorAsItemResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	AActor* Actor;
	
	UPROPERTY(BlueprintReadOnly)
	FItem Item;
	
	UPROPERTY(BlueprintReadOnly)
	bool Success;
};
