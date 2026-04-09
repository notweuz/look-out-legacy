// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "World/Objects/BasePickupableObject.h"

#include "Core/Libraries/ItemHelper.h"


// Sets default values
ABasePickupableObject::ABasePickupableObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABasePickupableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasePickupableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UItemDefinition* ABasePickupableObject::GetDefinition_Implementation()
{
	if (Definition)
	{
		return Definition;
	} 

	Definition = UItemHelper::BuildItemDefinition(
		DisplayName,
		Description,
		Weight,
		Icon,
		GetClass(),
		this,
		MaxHealth
	);
	return Definition;
}
