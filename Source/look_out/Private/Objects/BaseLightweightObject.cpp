// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/BaseLightweightObject.h"

ABaseLightweightObject::ABaseLightweightObject()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseLightweightObject::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseLightweightObject::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

EGrabbableObjectType ABaseLightweightObject::GetGrabbableType_Implementation()
{
	return Lightweight;
}

FText ABaseLightweightObject::GetGrabWidgetText_Implementation()
{
	return FText::FromString(TEXT("LMB - Grab"));
}
