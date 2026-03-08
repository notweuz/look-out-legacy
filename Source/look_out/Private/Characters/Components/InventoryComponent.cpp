// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Characters/Components/InventoryComponent.h"

#include "Characters/BaseCharacter.h"
#include "Characters/Components/GrabbingComponent.h"
#include "Interfaces/Storeable.h"

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
	Hotbar.SetNum(HotbarSize);
}

void UInventoryComponent::UpdateHandItem_Implementation(int OldItemIndex)
{
	if (!OwnerCharacter)
	{
		return;
	}

	if (OldItemIndex == CurrentActiveItemIndex)
	{
		return;
	}
	
	OwnerCharacter->PlayerUIComponent->PlayerUI->Hotbar->SetActiveHotbarSlot(OldItemIndex, CurrentActiveItemIndex);
	if (Hotbar.IsValidIndex(OldItemIndex) && !Hotbar[OldItemIndex].ItemClass.IsNull())
	{
		TArray<USceneComponent*> ChildComponents;
		OwnerCharacter->HandSceneComponent->GetChildrenComponents(false, ChildComponents);

		for (USceneComponent* ChildComp : ChildComponents)
		{
			if (!ChildComp)
			{
				continue;
			}

			AActor* AttachedActor = ChildComp->GetOwner();
			if (!AttachedActor || AttachedActor == OwnerCharacter)
			{
				continue;
			}

			if (AttachedActor->Implements<UStoreable>())
			{
				Hotbar[OldItemIndex].SavedTags = IStoreable::Execute_GetItemTags(AttachedActor);
				UE_LOG(LogTemp, Log, TEXT("InventoryComponent: Saved tags for hotbar index %d"), OldItemIndex);
			}

			AttachedActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			AttachedActor->Destroy();

			UE_LOG(LogTemp, Log, TEXT("InventoryComponent: Removed hand item from hotbar index %d"), OldItemIndex);
			break;
		}
	}

	if (!Hotbar.IsValidIndex(CurrentActiveItemIndex) || Hotbar[CurrentActiveItemIndex].ItemClass.IsNull())
	{
		UE_LOG(LogTemp, Log, TEXT("InventoryComponent: No item at new hotbar index %d"), CurrentActiveItemIndex);
		return;
	}

	FItem& NewItem = Hotbar[CurrentActiveItemIndex];

	const TSubclassOf<AActor> ActorClass = NewItem.ItemClass.LoadSynchronous();
	if (!ActorClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryComponent: Failed to load class at index %d"), CurrentActiveItemIndex);
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FTransform HandTransform = OwnerCharacter->HandSceneComponent->GetComponentTransform();
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorClass, HandTransform, SpawnParams);
	if (!SpawnedActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryComponent: Failed to spawn hand item at index %d"),
		       CurrentActiveItemIndex);
		return;
	}

	if (SpawnedActor->Implements<UStoreable>() && NewItem.SavedTags.Num() > 0)
	{
		IStoreable::Execute_ApplyItemTags(SpawnedActor, NewItem.SavedTags);
		UE_LOG(LogTemp, Log, TEXT("InventoryComponent: Applied saved tags to hand item at index %d"),
		       CurrentActiveItemIndex);
	}

	if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(SpawnedActor->GetRootComponent()))
	{
		PrimComp->SetSimulatePhysics(false);
		PrimComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	SpawnedActor->AttachToComponent(
		OwnerCharacter->HandSceneComponent,
		FAttachmentTransformRules::SnapToTargetIncludingScale
	);

	UE_LOG(LogTemp, Log, TEXT("InventoryComponent: Spawned and attached hand item at index %d"),
	       CurrentActiveItemIndex);
}

void UInventoryComponent::DropHotbarItem_Implementation()
{
	if (!OwnerCharacter)
	{
		return;
	}

	if (!Hotbar.IsValidIndex(CurrentActiveItemIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryComponent: no item in current hotbar index %d"),
		       CurrentActiveItemIndex);
		return;
	}

	const FItem& Item = Hotbar[CurrentActiveItemIndex];
	const TSubclassOf<AActor> ActorClass = Item.ItemClass.LoadSynchronous();
	if (!ActorClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryComponent: Failed to load class at index %d"), CurrentActiveItemIndex);
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	const FVector SpawnLocation = OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorForwardVector() * 100.0f;
	const FRotator SpawnRotation = OwnerCharacter->GetActorRotation();
	const FTransform SpawnTransform(SpawnRotation, SpawnLocation);

	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorClass, SpawnTransform, SpawnParams);
	if (!SpawnedActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryComponent: Failed to spawn actor from hotbar index %d"),
		       CurrentActiveItemIndex);
		return;
	}

	IStoreable::Execute_ApplyItemTags(SpawnedActor, Item.SavedTags);
	Hotbar[CurrentActiveItemIndex] = FItem();
	UE_LOG(LogTemp, Log, TEXT("InventoryComponent: dropped item from hotbar index %d"), CurrentActiveItemIndex);
}

void UInventoryComponent::CollectItem_Implementation()
{
	if (!OwnerCharacter)
	{
		return;
	}

	const auto [Start, End] = OwnerCharacter->GetForwardVectorRelatedToCamera(
		OwnerCharacter->GrabbingComponent->GrabDistance);

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

	if (!HitActor || !HitActor->GetClass()->ImplementsInterface(UStoreable::StaticClass()))
	{
		UE_LOG(LogTemp, Log, TEXT("InventoryComponent: hit actor is null or not storeable"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("InventoryComponent executing on %s"), *HitActor->GetName())
	AddItem(HitActor);
}

void UInventoryComponent::ScrollActiveItem_Implementation(const float Delta)
{
	if (HotbarSize == 0)
	{
		CurrentActiveItemIndex = -1;
		return;
	}

	const int32 DeltaInt = FMath::RoundToInt(Delta);
	if (DeltaInt == 0)
	{
		return;
	}
	const int32 OldActiveItemIndex = CurrentActiveItemIndex;
	CurrentActiveItemIndex = (CurrentActiveItemIndex + DeltaInt) % HotbarSize;
	if (CurrentActiveItemIndex < 0)
	{
		CurrentActiveItemIndex += HotbarSize;
	}

	UpdateHandItem(OldActiveItemIndex);
}
