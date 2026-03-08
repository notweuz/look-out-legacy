// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ItemTag.h"
#include "Item.generated.h"

USTRUCT(BlueprintType)
struct LOOK_OUT_API FItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TSoftClassPtr<AActor> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 Quantity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 ItemWeight = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FItemTag> SavedTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	UTexture2D* ItemIcon;
};
