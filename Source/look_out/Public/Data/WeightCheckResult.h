// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeightCheckResult.generated.h"

USTRUCT(BlueprintType)
struct FWeightCheckResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	bool bIsHeavy = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsNotHeavy = false;
};