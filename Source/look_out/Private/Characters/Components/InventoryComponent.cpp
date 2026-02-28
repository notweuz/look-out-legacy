// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Characters/Components/InventoryComponent.h"

#include "Characters/BaseCharacter.h"

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
}
