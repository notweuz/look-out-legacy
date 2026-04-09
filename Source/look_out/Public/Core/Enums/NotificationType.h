// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Class.h"
#include "NotificationType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum ENotificationType
{
	Info,
	Error,
	Warning,
	Debug
};
