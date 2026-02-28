// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Characters/Components/InventoryComponent.h"

#include "Characters/BaseCharacter.h"

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
	Hotbar.SetNum(HotbarSize);
}

void UInventoryComponent::ScrollActiveItem_Implementation(const float Delta)
{
	if (HotbarSize == 0)
	{
		CurrentActiveItemIndex = -1;
		return;
	}

	const int32 DeltaInt = FMath::RoundToInt(Delta);
	CurrentActiveItemIndex = (CurrentActiveItemIndex + DeltaInt) % HotbarSize;
	if (CurrentActiveItemIndex < 0)
	{
		CurrentActiveItemIndex += HotbarSize;
	}
}
