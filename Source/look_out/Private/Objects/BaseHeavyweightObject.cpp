// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/BaseHeavyweightObject.h"

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
	SaveUtils::Save(this, OutBytes);
}

void ABaseHeavyweightObject::OnLoad_Implementation(const TArray<uint8>& InBytes)
{
	SaveUtils::Load(this, InBytes);
}