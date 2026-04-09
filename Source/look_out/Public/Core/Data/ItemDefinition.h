// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDefinition.generated.h"

UCLASS(BlueprintType)
class LOOK_OUT_API UItemDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item")
	FText DisplayName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item")
	float Weight = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item")
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item")
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item")
	TSoftClassPtr<AActor> ActorClass;
};
