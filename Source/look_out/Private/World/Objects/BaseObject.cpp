// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Objects/BaseObject.h"

#include "Core/Save/SaveManager.h"

ABaseObject::ABaseObject()
{
	PrimaryActorTick.bCanEverTick = true;
	Health = FMath::Max(Definition != nullptr ? Definition->MaxHealth : 100.0f, MaxHealth, 100.0f);
}

void ABaseObject::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseObject::DestroyPermanently_Implementation()
{
	GetGameInstance()->GetSubsystem<USaveManager>()->DestroyActor(this);
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