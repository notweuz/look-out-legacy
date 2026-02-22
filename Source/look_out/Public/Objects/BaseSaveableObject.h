// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Saveable.h"
#include "BaseSaveableObject.generated.h"

UCLASS()
class LOOK_OUT_API ABaseSaveableObject : public AActor, public ISaveable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseSaveableObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnSave_Implementation(TArray<uint8>& OutData) override;
	
	virtual void OnLoad_Implementation(const TArray<uint8>& InData) override;
	
	virtual FString GetSaveID_Implementation() const override;
};
