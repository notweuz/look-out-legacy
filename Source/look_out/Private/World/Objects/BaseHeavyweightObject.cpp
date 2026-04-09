// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Objects/BaseHeavyweightObject.h"

#include "Utils/SaveUtils.h"

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

void ABaseHeavyweightObject::OnSave_Implementation(TArray<uint8>& OutBytes)
{
	Super::OnSave_Implementation(OutBytes);
}

void ABaseHeavyweightObject::OnLoad_Implementation(const TArray<uint8>& InBytes)
{
	Super::OnLoad_Implementation(InBytes);
}