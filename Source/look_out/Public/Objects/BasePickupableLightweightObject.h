// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseLightweightObject.h"
#include "Interfaces/Pickupable.h"
#include "BasePickupableLightweightObject.generated.h"

UCLASS()
class LOOK_OUT_API ABasePickupableLightweightObject : public ABaseLightweightObject, public IPickupable
{
	GENERATED_BODY()

public:
	ABasePickupableLightweightObject();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	UItemDefinition* GetDefinition_Implementation() override;
};
