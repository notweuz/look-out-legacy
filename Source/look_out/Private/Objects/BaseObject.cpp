// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/BaseObject.h"

#include "Utils/SaveUtils.h"

ABaseObject::ABaseObject()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseObject::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseObject::OnSave_Implementation(TArray<uint8>& OutBytes)
{
	SaveUtils::Save(this, OutBytes);
}

void ABaseObject::OnLoad_Implementation(const TArray<uint8>& InBytes)
{
	SaveUtils::Load(this, InBytes);
}