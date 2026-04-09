// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Save/SaveTypes.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Grabbable.h"
#include "Interfaces/Saveable.h"
#include "BaseStaticObject.generated.h"

UCLASS()
class LOOK_OUT_API ABaseStaticObject : public AActor, public IGrabbable, public ISaveable
{
	GENERATED_BODY()

public:
	ABaseStaticObject();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(SaveGame)
	FGuid SaveId = FGuid::NewGuid();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Save")
	EActorPersistence Persistence = EActorPersistence::Placed;
	
	virtual void Tick(float DeltaTime) override;
	virtual EGrabbableObjectType GetGrabbableType_Implementation() override;
	virtual FText GetGrabWidgetText_Implementation() override;

	virtual FGuid GetSaveId_Implementation() override { return SaveId; }
	virtual void OnSave_Implementation(TArray<uint8>& OutBytes) override;
	virtual void OnLoad_Implementation(const TArray<uint8>& InBytes) override;
};
