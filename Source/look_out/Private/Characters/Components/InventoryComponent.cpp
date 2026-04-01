// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Characters/Components/InventoryComponent.h"

#include "Characters/BaseCharacter.h"
#include "Characters/Components/GrabbingComponent.h"
#include "Characters/Components/PlayerUIComponent.h"
#include "Interfaces/Interactable.h"
#include "Interfaces/Storeable.h"
#include "Misc/LogCategories.h"
#include "Widgets/Gameplay/BasePlayerUI.h"
#include "Widgets/Gameplay/Hotbar/BasePlayerHotbar.h"

namespace InventoryComponentPrivate
{
	constexpr float DropSpawnDistance = 100.0f;

	bool IsValidItem(const FItem& Item)
	{
		return !Item.ItemClass.IsNull();
	}
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
	HotbarSize = FMath::Max(0, HotbarSize);
	Hotbar.SetNum(HotbarSize);
}

bool UInventoryComponent::IsHotbarFull() const
{
	for (const FItem& Item : Hotbar)
	{
		if (!InventoryComponentPrivate::IsValidItem(Item))
		{
			return false;
		}
	}

	return true;
}

void UInventoryComponent::OnSlotClicked(UBasePlayerInventorySlot* ClickedSlot, bool IsRMB)
{
	if (!IsRMB)
	{
		ProcessItemDragNDrop(ClickedSlot);
	}
}

void UInventoryComponent::OnInventoryWindowClicked()
{
	OnInventoryWindowClickedFor(this);
}

void UInventoryComponent::OnInventoryWindowClickedFor(UStorageComponent* TargetStorage)
{
	if (!OwnerCharacter)
	{
		return;
	}

	if (TempItem.ItemClass.IsNull())
	{
		return;
	}

	if (!TargetStorage)
	{
		TargetStorage = this;
	}

	if (!TargetStorage->CanAddItem(TempItem))
	{
		UE_LOG(LogInventory, Warning, TEXT("InventoryComponent: Not enough storage weight for drag&drop to storage"));
		// TODO: UI notification
		return;
	}

	TargetStorage->Storage.Add(TempItem);
	TempItem = FItem();

	UpdateEntireUI();
}

void UInventoryComponent::ProcessItemDragNDrop(UBasePlayerInventorySlot* ClickedSlot)
{
	if (!ClickedSlot || !OwnerCharacter)
	{
		return;
	}

	FItem* SlotItemPtr = GetItemForSlot(ClickedSlot);
	if (!SlotItemPtr)
	{
		return;
	}

	FItem& SlotItem = *SlotItemPtr;

	if (TempItem.ItemClass.IsNull() && SlotItem.ItemClass.IsNull())
	{
		return;
	}

	UStorageComponent* TargetStorage = ClickedSlot->InventorySlotType == EInventorySlotType::Hotbar
		                                   ? this
		                                   : ClickedSlot->LinkedStorage.Get();

	if (ClickedSlot->InventorySlotType != EInventorySlotType::Hotbar && !TargetStorage)
	{
		return;
	}

	if (ClickedSlot->InventorySlotType != EInventorySlotType::Hotbar && !TempItem.ItemClass.IsNull() &&
		!TargetStorage->CanSwapItems(TempItem, SlotItem))
	{
		UE_LOG(LogInventory, Warning,
		       TEXT("InventoryComponent: Not enough storage weight for drag&drop swap to storage"));
		// TODO: UI notification
		return;
	}

	Swap(TempItem, SlotItem);

	if (ClickedSlot->InventorySlotType != EInventorySlotType::Hotbar && SlotItem.ItemClass.IsNull())
	{
		const int32 SlotIndex = ClickedSlot->InventorySlotIndex;
		if (TargetStorage->Storage.IsValidIndex(SlotIndex))
		{
			TargetStorage->Storage.RemoveAt(SlotIndex);
		}
	}

	UpdateInventoryUI();

	if (ClickedSlot->InventorySlotType == EInventorySlotType::Hotbar &&
		ClickedSlot->InventorySlotIndex == CurrentActiveItemIndex)
	{
		UpdateHandItem(CurrentActiveItemIndex);
	}
}

UBasePlayerUI* UInventoryComponent::GetPlayerUI() const
{
	if (!OwnerCharacter || !OwnerCharacter->PlayerUIComponent)
	{
		return nullptr;
	}

	return OwnerCharacter->PlayerUIComponent->PlayerUI;
}

void UInventoryComponent::UpdateInventoryUI() const
{
	if (UBasePlayerUI* PlayerUI = GetPlayerUI())
	{
		PlayerUI->UpdateEntireInventory();
	}
}

void UInventoryComponent::UpdateHotbarUI() const
{
	if (UBasePlayerUI* PlayerUI = GetPlayerUI(); PlayerUI && PlayerUI->Hotbar)
	{
		PlayerUI->Hotbar->UpdateHotbarSlots();
	}
}

void UInventoryComponent::UpdateEntireUI() const
{
	UpdateInventoryUI();
}

FItem* UInventoryComponent::GetItemForSlot(const UBasePlayerInventorySlot* Slot)
{
	if (!Slot)
	{
		return nullptr;
	}

	const int32 SlotIndex = Slot->InventorySlotIndex;
	UE_LOG(LogInventory, Display, TEXT("Clicked on slot %d (bIsHotbar=%d)"), SlotIndex,
	       Slot->InventorySlotType == EInventorySlotType::Hotbar ? 1 : 0);

	if (Slot->InventorySlotType == EInventorySlotType::Hotbar)
	{
		if (!IsValidHotbarIndex(SlotIndex))
		{
			UE_LOG(LogInventory, Warning, TEXT("InventoryComponent: Invalid hotbar index %d"), SlotIndex);
			return nullptr;
		}

		return &Hotbar[SlotIndex];
	}

	if (!Slot->LinkedStorage)
	{
		UE_LOG(LogInventory, Warning, TEXT("InventoryComponent: Slot has no linked storage"));
		return nullptr;
	}

	if (!Slot->LinkedStorage->Storage.IsValidIndex(SlotIndex))
	{
		UE_LOG(LogInventory, Warning, TEXT("InventoryComponent: Invalid storage index %d"), SlotIndex);
		return nullptr;
	}

	return &Slot->LinkedStorage->Storage[SlotIndex];
}

AActor* UInventoryComponent::GetItemInHandActor() const
{
	if (!OwnerCharacter || !OwnerCharacter->HandSceneComponent)
	{
		return nullptr;
	}

	if (!IsValidHotbarIndex(CurrentActiveItemIndex) ||
		!InventoryComponentPrivate::IsValidItem(Hotbar[CurrentActiveItemIndex]))
	{
		return nullptr;
	}

	TArray<USceneComponent*> ChildComponents;
	OwnerCharacter->HandSceneComponent->GetChildrenComponents(false, ChildComponents);

	for (const USceneComponent* ChildComp : ChildComponents)
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

		return AttachedActor;
	}

	return nullptr;
}

bool UInventoryComponent::IsValidHotbarIndex(const int32 Index) const
{
	return Hotbar.IsValidIndex(Index);
}

void UInventoryComponent::SaveActorStateToHotbarIndex(const int32 HotbarIndex, AActor* Actor)
{
	if (!Actor || !IsValidHotbarIndex(HotbarIndex) || !InventoryComponentPrivate::IsValidItem(Hotbar[HotbarIndex]))
	{
		return;
	}

	if (Actor->Implements<UStoreable>())
	{
		Hotbar[HotbarIndex].SavedTags = IStoreable::Execute_GetItemTags(Actor);
		UE_LOG(LogInventory, Log, TEXT("InventoryComponent: Saved tags for hotbar index %d"), HotbarIndex);
	}
}

void UInventoryComponent::RemoveHandItemActors(const int32 PreviousHotbarIndex)
{
	if (!OwnerCharacter || !OwnerCharacter->HandSceneComponent)
	{
		return;
	}

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

		if (PreviousHotbarIndex != CurrentActiveItemIndex)
		{
			SaveActorStateToHotbarIndex(PreviousHotbarIndex, AttachedActor);
		}

		AttachedActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		AttachedActor->Destroy();

		UE_LOG(LogInventory, Log, TEXT("InventoryComponent: Removed hand item"));
	}
}

AActor* UInventoryComponent::SpawnActorFromItem(
	const FItem& Item,
	const FTransform& SpawnTransform,
	const ESpawnActorCollisionHandlingMethod CollisionHandling) const
{
	if (!OwnerCharacter || !GetWorld() || !InventoryComponentPrivate::IsValidItem(Item))
	{
		return nullptr;
	}

	const TSubclassOf<AActor> ActorClass = Item.ItemClass.LoadSynchronous();
	if (!ActorClass)
	{
		UE_LOG(LogInventory, Warning, TEXT("InventoryComponent: Failed to load actor class"));
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = CollisionHandling;

	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorClass, SpawnTransform, SpawnParams);
	if (!SpawnedActor)
	{
		UE_LOG(LogInventory, Warning, TEXT("InventoryComponent: Failed to spawn actor"));
		return nullptr;
	}

	if (SpawnedActor->Implements<UStoreable>() && Item.SavedTags.Num() > 0)
	{
		IStoreable::Execute_ApplyItemTags(SpawnedActor, Item.SavedTags);
	}

	return SpawnedActor;
}

void UInventoryComponent::UpdateHandItem(int OldItemIndex)
{
	if (!OwnerCharacter)
	{
		return;
	}

	if (OldItemIndex != CurrentActiveItemIndex)
	{
		if (UBasePlayerUI* PlayerUI = GetPlayerUI(); PlayerUI && PlayerUI->Hotbar)
		{
			PlayerUI->Hotbar->SetActiveHotbarSlot(OldItemIndex, CurrentActiveItemIndex);
		}
	}

	RemoveHandItemActors(OldItemIndex);

	if (!IsValidHotbarIndex(CurrentActiveItemIndex) ||
		!InventoryComponentPrivate::IsValidItem(Hotbar[CurrentActiveItemIndex]))
	{
		return;
	}

	FItem& NewItem = Hotbar[CurrentActiveItemIndex];
	const FTransform HandTransform = OwnerCharacter->HandSceneComponent->GetComponentTransform();
	AActor* SpawnedActor = SpawnActorFromItem(NewItem, HandTransform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	if (!SpawnedActor)
	{
		UE_LOG(LogInventory, Warning, TEXT("InventoryComponent: Failed to spawn hand item at index %d"),
		       CurrentActiveItemIndex);
		return;
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

	UE_LOG(LogInventory, Log, TEXT("InventoryComponent: Spawned and attached hand item at index %d"),
	       CurrentActiveItemIndex);
}

void UInventoryComponent::InteractWithItemInHand()
{
	if (!OwnerCharacter)
	{
		return;
	}

	AActor* ItemInHandActor = GetItemInHandActor();

	if (!ItemInHandActor)
	{
		UE_LOG(LogInventory, Warning, TEXT("InventoryComponent: no actor found in hand to interact with"));
		return;
	}

	if (!ItemInHandActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		UE_LOG(LogInventory, Warning, TEXT("InventoryComponent: actor in hand %s is not interactable"),
		       *ItemInHandActor->GetName());
		return;
	}

	UE_LOG(LogInventory, Log, TEXT("InventoryComponent: interacting with actor in hand %s"),
	       *ItemInHandActor->GetName());

	IInteractable::Execute_Interact(ItemInHandActor, OwnerCharacter);

	if (IsValidHotbarIndex(CurrentActiveItemIndex) &&
		InventoryComponentPrivate::IsValidItem(Hotbar[CurrentActiveItemIndex]) &&
		ItemInHandActor->Implements<UStoreable>())
	{
		Hotbar[CurrentActiveItemIndex].SavedTags = IStoreable::Execute_GetItemTags(ItemInHandActor);
	}
}

bool UInventoryComponent::HasItemInHand() const
{
	return GetItemInHandActor() != nullptr;
}

void UInventoryComponent::DropHotbarItem()
{
	if (!OwnerCharacter)
	{
		return;
	}

	if (!IsValidHotbarIndex(CurrentActiveItemIndex))
	{
		UE_LOG(LogInventory, Warning, TEXT("InventoryComponent: no item in current hotbar index %d"),
		       CurrentActiveItemIndex);
		return;
	}

	const FItem& Item = Hotbar[CurrentActiveItemIndex];
	if (!InventoryComponentPrivate::IsValidItem(Item))
	{
		UE_LOG(LogInventory, Warning, TEXT("InventoryComponent: no item to drop at hotbar index %d"),
		       CurrentActiveItemIndex);
		return;
	}

	const auto [CameraStart, CameraEnd] = OwnerCharacter->GetForwardVectorRelatedToCamera(
		InventoryComponentPrivate::DropSpawnDistance);
	const FVector CameraForward = (CameraEnd - CameraStart).GetSafeNormal();
	const FVector SpawnLocation = CameraStart + CameraForward * InventoryComponentPrivate::DropSpawnDistance;
	const FRotator SpawnRotation = CameraForward.Rotation();
	const FTransform SpawnTransform(SpawnRotation, SpawnLocation);

	AActor* SpawnedActor = SpawnActorFromItem(
		Item,
		SpawnTransform,
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if (!SpawnedActor)
	{
		UE_LOG(LogInventory, Warning, TEXT("InventoryComponent: Failed to spawn actor from hotbar index %d"),
	       CurrentActiveItemIndex);
		return;
	}

	Hotbar[CurrentActiveItemIndex] = FItem();
	UE_LOG(LogInventory, Log, TEXT("InventoryComponent: dropped item from hotbar index %d"), CurrentActiveItemIndex);

	UpdateHandItem(CurrentActiveItemIndex);
	UpdateEntireUI();
}

void UInventoryComponent::CollectItem()
{
	if (!OwnerCharacter)
	{
		UE_LOG(LogInventory, Error, TEXT("InventoryComponent: OwnerCharacter is null during CollectItem"));
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
		UE_LOG(LogInventory, Log, TEXT("InventoryComponent: hit actor is null or not storeable"));
		return;
	}

	UE_LOG(LogInventory, Log, TEXT("InventoryComponent executing on %s"), *HitActor->GetName());

	if (OwnerCharacter->GrabbingComponent->IsGrabbingObject)
	{
		OwnerCharacter->GrabbingComponent->ToggleGrab(false);
	}

	if (!IsHotbarFull())
	{
		if (auto [Actor, Item, Success] = GetActorAsItemResult(HitActor); Success)
		{
			UE_LOG(LogInventory, Log, TEXT("Found empty storage in hotbar, trying to store actor there"));
			Actor->Destroy();
			const int32 EmptyIndex = Hotbar.IndexOfByPredicate([](const FItem& I)
			{
				return I.ItemClass.IsNull();
			});

			if (IsValidHotbarIndex(EmptyIndex))
			{
				Hotbar[EmptyIndex] = Item;
				UpdateEntireUI();
				if (EmptyIndex == CurrentActiveItemIndex)
				{
					UpdateHandItem(CurrentActiveItemIndex);
				}
			}
		}
	}
	else
	{
		AddItem(HitActor);
		UpdateEntireUI();
	}
}

void UInventoryComponent::ScrollActiveItem(const float Delta)
{
	if (HotbarSize == 0)
	{
		CurrentActiveItemIndex = -1;
		return;
	}

	const int32 DeltaInt = FMath::RoundToInt(Delta * -1);
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
