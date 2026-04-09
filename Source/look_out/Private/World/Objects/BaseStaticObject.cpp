// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Objects/BaseStaticObject.h"

#include "Utils/SaveUtils.h"

ABaseStaticObject::ABaseStaticObject()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseStaticObject::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseStaticObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

EGrabbableObjectType ABaseStaticObject::GetGrabbableType_Implementation()
{
	return Static;
}

FText ABaseStaticObject::GetGrabWidgetText_Implementation()
{
	return FText::FromString(TEXT("LMB - Hold"));
}

void ABaseStaticObject::OnSave_Implementation(TArray<uint8>& OutBytes)
{
	Super::OnSave_Implementation(OutBytes);
}

void ABaseStaticObject::OnLoad_Implementation(const TArray<uint8>& InBytes)
{
	Super::OnLoad_Implementation(InBytes);
}
