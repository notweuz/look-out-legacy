// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Objects/BaseObject.h"

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
	DefaultSaveObject(this, OutBytes);
}

void ABaseObject::OnLoad_Implementation(const TArray<uint8>& InBytes)
{
	DefaultLoadObject(this, InBytes);
}

EObjectType ABaseObject::GetGrabbableType_Implementation()
{
	return ObjectType;
}

FText ABaseObject::GetGrabWidgetText_Implementation()
{
	switch (ObjectType)
	{
	case EObjectType::Lightweight:
		return FText::FromString("LMB - Grab");
	case EObjectType::Heavyweight:
		return FText::FromString("LMB - Drag");
	case EObjectType::Static:
		return FText::FromString("LMB - Hold");
	default:
		return FText::GetEmpty();
	}
}