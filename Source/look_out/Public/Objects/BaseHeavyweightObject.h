// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Save/ActorSaveData.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Grabbable.h"
#include "Interfaces/Saveable.h"
#include "BaseHeavyweightObject.generated.h"

UCLASS()
class LOOK_OUT_API ABaseHeavyweightObject : public AActor, public IGrabbable, public ISaveable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseHeavyweightObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual EGrabbableObjectType GetGrabbableType_Implementation() override;
	virtual FText GetGrabWidgetText_Implementation() override;
	virtual void OnSave_Implementation(TArray<uint8>& OutData) override;
	virtual void OnLoad_Implementation(const TArray<uint8>& InData) override;
	virtual FString GetSaveID_Implementation() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Save System")
	FGuid PersistentActorId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Save System")
	bool bGeneratePersistentActorIdOnBeginPlay = true;

protected:
	void EnsurePersistentActorId();
};
