// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/BaseHeavyweightObject.h"

// Sets default values
ABaseHeavyweightObject::ABaseHeavyweightObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABaseHeavyweightObject::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABaseHeavyweightObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

EGrabbableObjectType ABaseHeavyweightObject::GetGrabbableType_Implementation()
{
	return Heavyweight;
}

FText ABaseHeavyweightObject::GetGrabWidgetText_Implementation()
{
	return FText::FromString(TEXT("LMB - Drag"));
}
