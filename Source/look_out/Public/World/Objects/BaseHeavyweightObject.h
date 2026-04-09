// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseObject.h"
#include "Core/Save/SaveTypes.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Grabbable.h"
#include "BaseHeavyweightObject.generated.h"

UCLASS()
class LOOK_OUT_API ABaseHeavyweightObject : public ABaseObject, public IGrabbable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseHeavyweightObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual EGrabbableObjectType GetGrabbableType_Implementation() override;
	virtual FText GetGrabWidgetText_Implementation() override;

	virtual FGuid GetSaveId_Implementation() override { return SaveId; }
	virtual void OnSave_Implementation(TArray<uint8>& OutBytes) override;
	virtual void OnLoad_Implementation(const TArray<uint8>& InBytes) override;
};
