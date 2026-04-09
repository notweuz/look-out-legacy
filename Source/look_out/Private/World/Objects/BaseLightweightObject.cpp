// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Objects/BaseLightweightObject.h"

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

void ABaseLightweightObject::OnSave_Implementation(TArray<uint8>& OutBytes)
{
	Super::OnSave_Implementation(OutBytes);
}

void ABaseLightweightObject::OnLoad_Implementation(const TArray<uint8>& InBytes)
{
	Super::OnLoad_Implementation(InBytes);
}
