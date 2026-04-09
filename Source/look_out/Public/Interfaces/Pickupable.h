// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/Data/ItemDefinition.h"
#include "UObject/Interface.h"
#include "Pickupable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UPickupable : public UInterface { GENERATED_BODY() };

class LOOK_OUT_API IPickupable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category="Pickup") 
	UItemDefinition* GetDefinition();
};
