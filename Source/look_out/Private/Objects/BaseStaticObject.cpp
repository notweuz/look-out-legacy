// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/BaseStaticObject.h"

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
