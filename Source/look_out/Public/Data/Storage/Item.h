// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

USTRUCT(BlueprintType)
struct LOOK_OUT_API FItem
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TSoftClassPtr<AActor> ItemClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 Quantity = 1;
};