// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Class.h"
#include "GrabbableObjectType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum EGrabbableObjectType
{
	Lightweight,
	Heavyweight,
	Static,
	None
};
