// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Objects/BasePickupableLightweightObject.h"

#include "Core/Libraries/ItemHelper.h"


// Sets default values
ABasePickupableLightweightObject::ABasePickupableLightweightObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABasePickupableLightweightObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasePickupableLightweightObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UItemDefinition* ABasePickupableLightweightObject::GetDefinition_Implementation()
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
		this
	);
	return Definition;
}
