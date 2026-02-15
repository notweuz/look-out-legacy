// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/BaseLightweightObject.h"

// Sets default values
ABaseLightweightObject::ABaseLightweightObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseLightweightObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseLightweightObject::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

}

EGrabbableObjectType ABaseLightweightObject::GetGrabbableType_Implementation()
{
	return Lightweight;
}

