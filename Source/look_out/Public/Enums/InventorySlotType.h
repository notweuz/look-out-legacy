// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Class.h"
#include "InventorySlotType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum EInventorySlotType
{
	PlayerInventory,
	ExternalInventory,
	Hotbar,
	SecondHand,
};
