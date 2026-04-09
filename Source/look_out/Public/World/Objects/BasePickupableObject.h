// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseObject.h"
#include "Interfaces/Pickupable.h"
#include "BasePickupableObject.generated.h"

UCLASS()
class LOOK_OUT_API ABasePickupableObject : public ABaseObject, public IPickupable
{
	GENERATED_BODY()

public:
	ABasePickupableObject();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual UItemDefinition* GetDefinition_Implementation() override;
};
