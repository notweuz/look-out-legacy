// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/BaseHeavyweightObject.h"

ABaseHeavyweightObject::ABaseHeavyweightObject()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseHeavyweightObject::BeginPlay()
{
	Super::BeginPlay();
}

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
