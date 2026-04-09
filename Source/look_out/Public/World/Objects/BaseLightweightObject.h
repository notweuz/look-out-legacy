// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseObject.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Grabbable.h"
#include "BaseLightweightObject.generated.h"

UCLASS(Blueprintable, BlueprintType)
class LOOK_OUT_API ABaseLightweightObject : public ABaseObject, public IGrabbable
{
	GENERATED_BODY()

public:
	ABaseLightweightObject();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual EGrabbableObjectType GetGrabbableType_Implementation() override;
	virtual FText GetGrabWidgetText_Implementation() override;
	
	virtual FGuid GetSaveId_Implementation() override { return SaveId; }
	virtual void OnSave_Implementation(TArray<uint8>& OutBytes) override;
	virtual void OnLoad_Implementation(const TArray<uint8>& InBytes) override;
};
