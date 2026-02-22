// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Objects/BaseSaveableObject.h"

// Sets default values
ABaseSaveableObject::ABaseSaveableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseSaveableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseSaveableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseSaveableObject::OnSave_Implementation(TArray<uint8>& OutData)
{
	ISaveable::OnSave_Implementation(OutData);
}

void ABaseSaveableObject::OnLoad_Implementation(const TArray<uint8>& InData)
{
	ISaveable::OnLoad_Implementation(InData);
}

FString ABaseSaveableObject::GetSaveID_Implementation() const
{
	return GetName();
}

