// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Characters/Components/InventoryComponent.h"

#include "Characters/BaseCharacter.h"
#include "Characters/Components/GrabbingComponent.h"
#include "Interfaces/Grabbable.h"
#include "Interfaces/Interactable.h"
#include "Interfaces/Storeable.h"

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
	Hotbar.SetNum(HotbarSize);
}

void UInventoryComponent::CollectItem_Implementation()
{
	if (!OwnerCharacter) return;

	const auto [Start, End] = OwnerCharacter->GetForwardVectorRelatedToCamera(OwnerCharacter->GrabbingComponent->GrabDistance);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerCharacter);
	
#if WITH_EDITOR
	DrawDebugLine(OwnerCharacter->GetWorld(), Start, End, FColor::Green, false, 10.0f);
#endif
	
	
	if (!GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		return;
	}

	AActor* HitActor = Hit.GetActor();

	if (!HitActor && HitActor->GetClass()->ImplementsInterface(UStoreable::StaticClass()))
	{
		UE_LOG(LogTemp, Log, TEXT("InventoryComponent found a storeable actor %s"), *HitActor->GetName())
		return;
	}

	if (HitActor)
	{
		UE_LOG(LogTemp, Log, TEXT("InventoryComponent executing on %s"), *HitActor->GetName())
		AddItem(HitActor);
	}
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
