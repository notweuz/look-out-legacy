// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Characters/Components/PlayerObjectHintsComponent.h"

// Sets default values for this component's properties
UPlayerObjectHintsComponent::UPlayerObjectHintsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerObjectHintsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPlayerObjectHintsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

