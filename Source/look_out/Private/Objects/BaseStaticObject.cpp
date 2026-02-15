// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/BaseStaticObject.h"

// Sets default values
ABaseStaticObject::ABaseStaticObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseStaticObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseStaticObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

EGrabbableObjectType ABaseStaticObject::GetGrabbableType_Implementation()
{
	return Static;
}
