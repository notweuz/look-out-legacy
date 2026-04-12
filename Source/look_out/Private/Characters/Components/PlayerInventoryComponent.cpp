#include "Characters/Components/PlayerInventoryComponent.h"

#include "Characters/Components/PlayerGrabComponent.h"
#include "Interfaces/Interactable.h"
#include "Interfaces/Pickupable.h"
#include "Interfaces/Saveable.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/LogCategories.h"

void UPlayerInventoryComponent::BeginPlay()
{
    Super::BeginPlay();
    
	OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
}

bool UPlayerInventoryComponent::TryPickup(AActor* Actor)
{
    if (!Actor) return false;

    if (!Actor->Implements<UPickupable>()) return false;

    const UItemDefinition* Def = IPickupable::Execute_GetDefinition(Actor);
    if (!Def) return false;

    if (!CanFit(Def->Weight))
    {
        UNotificationSubsystem* Notification = OwnerCharacter->GetGameInstance()->GetSubsystem<UNotificationSubsystem>();
        Notification->SendNotification(FText::FromString("Not enough storage"), 3.0f, ENotificationType::Warning);
        return false;
    }

    FItemSaveRecord Record;
    Record.ItemClass = Actor->GetClass();
    ISaveable::Execute_OnSave(Actor, Record.Bytes);

    AddItem(Record);

    Actor->Destroy();
    return true;
}

void UPlayerInventoryComponent::Collect()
{
    if (!OwnerCharacter || !GetWorld())
    {
        return;
    }
    
    const auto [Start, End] = OwnerCharacter->GetForwardVectorRelatedToCamera(OwnerCharacter->InteractionDistance);
    
    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(OwnerCharacter);
    
#if WITH_EDITOR
    DrawDebugLine(OwnerCharacter->GetWorld(), Start, End, FColor::Cyan, false, 10.0f);
#endif
    
    if (!GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
    {
        return;
    }

    if (AActor* HitActor = Hit.GetActor(); TryPickup(HitActor))
    {
        UE_LOG(LogInventory, Log, TEXT("Player picked up item of type %s"), *HitActor->GetClass()->GetName());
        OwnerCharacter->PlayerGrabComponent->ToggleGrab(false);
    } else
    {
        UE_LOG(LogInventory, Log, TEXT("Player couldn't pick up item of type %s"), *HitActor->GetClass()->GetName());
    }
}

void UPlayerInventoryComponent::DropActiveItem()
{
    if (!Items.IsValidIndex(ActiveSlotIndex) || !OwnerCharacter || !GetWorld()) return;

    UnequipItem();

    FItemSaveRecord Record = Items[ActiveSlotIndex];
    UClass* Class = Record.ItemClass.LoadSynchronous();
    if (!Class) return;

    const auto [TraceStart, TraceEnd] =
        OwnerCharacter->GetForwardVectorRelatedToCamera(OwnerCharacter->InteractionDistance);

    FCollisionQueryParams Params(SCENE_QUERY_STAT(DropActiveItemTrace), false, OwnerCharacter);

    FHitResult Hit;
    const bool bHasSurfaceHit = GetWorld()->LineTraceSingleByChannel(
        Hit,
        TraceStart,
        TraceEnd,
        ECC_Visibility,
        Params
    );

    const FVector OwnerLocation = OwnerCharacter->GetActorLocation();

    const FVector DropLocation = bHasSurfaceHit
        ? Hit.ImpactPoint + Hit.ImpactNormal
        : OwnerLocation - FVector(0.0f, 0.0f, OwnerCharacter->GetDefaultHalfHeight());

    const FRotator DropRotation = FRotator(
        0.0f,
        OwnerCharacter->GetActorRotation().Yaw,
        0.0f
    );
    const FTransform DropTransform(DropRotation, DropLocation);

    AActor* DroppedActor = GetWorld()->SpawnActorDeferred<AActor>(Class, DropTransform);
    if (!DroppedActor) return;
    
    UGameplayStatics::FinishSpawningActor(DroppedActor, DropTransform);
    ISaveable::Execute_OnLoad(DroppedActor, Record.Bytes);

    RemoveItem(ActiveSlotIndex);
}

void UPlayerInventoryComponent::SetActiveSlot(int32 SlotIndex)
{
    if (Items.IsValidIndex(SlotIndex) && SlotIndex < HotbarSize && SlotIndex >= 0)
    if (SlotIndex == ActiveSlotIndex) return;
    UE_LOG(LogInventory, Log, TEXT("Selected slot: %d"), SlotIndex)

    ActiveSlotIndex = SlotIndex;
    OnActiveSlotChanged.Broadcast(ActiveSlotIndex);
    RefreshHandItem();
}

void UPlayerInventoryComponent::NextSlot()
{
    SetActiveSlot((ActiveSlotIndex + 1) % HotbarSize);
}

void UPlayerInventoryComponent::PrevSlot()
{
    int32 newSlot = (ActiveSlotIndex - 1 + HotbarSize) % HotbarSize;
    SetActiveSlot(newSlot == 0 ? HotbarSize - 1 : newSlot);
}

void UPlayerInventoryComponent::ScrollHotbar(int Delta)
{
    if (Delta > 0) NextSlot();
    else if (Delta < 0) PrevSlot();
}

bool UPlayerInventoryComponent::GetActiveItem(FItemSaveRecord& OutRecord) const
{
    if (!Items.IsValidIndex(ActiveSlotIndex)) return false;

    OutRecord = Items[ActiveSlotIndex];
    return true;
}

void UPlayerInventoryComponent::SaveToRecords(TArray<FItemSaveRecord>& OutRecords) const
{
    const_cast<UPlayerInventoryComponent*>(this)->SaveEquippedItemState();
    UStorageComponent::SaveToRecords(OutRecords);
}

void UPlayerInventoryComponent::RefreshHandItem()
{
    UnequipItem();

    FItemSaveRecord Record;
    if (GetActiveItem(Record))
        EquipActiveItem();
}

void UPlayerInventoryComponent::EquipActiveItem()
{
    UnequipItem();

    if (!Items.IsValidIndex(ActiveSlotIndex)) return;

    const FItemSaveRecord& Record = Items[ActiveSlotIndex];

    UClass* Class = Record.ItemClass.LoadSynchronous();
    if (!Class) return;

    AActor* Owner = GetOwner();

    FActorSpawnParameters Params;
    Params.Owner = Owner;

    ItemInHand = GetWorld()->SpawnActor<AActor>(Class, FTransform::Identity, Params);
    if (!ItemInHand) return;

    ISaveable::Execute_OnLoad(ItemInHand, Record.Bytes);

    ItemInHand->SetActorEnableCollision(false);
    OwnerCharacter->ConfigureEquippedItem(ItemInHand);
    ItemInHand->AttachToComponent(
        OwnerCharacter->HandSceneComponent,
        FAttachmentTransformRules::SnapToTargetNotIncludingScale
    );
    ItemInHand->SetActorRelativeRotation(OwnerCharacter->EquippedItemFacingOffset);
    EquippedItemIndex = ActiveSlotIndex;

    UE_LOG(LogInventory, Log, TEXT("Equipped item: %s"), *ItemInHand->GetName());
    OnItemEquipped.Broadcast(ItemInHand);
}

void UPlayerInventoryComponent::UnequipItem()
{
    if (!ItemInHand) return;

    SaveEquippedItemState();

    ItemInHand->Destroy();
    ItemInHand = nullptr;
    EquippedItemIndex = INDEX_NONE;

    OnItemUnequipped.Broadcast();
}

void UPlayerInventoryComponent::ToggleEquip()
{
    if (ItemInHand) UnequipItem();
    else EquipActiveItem();
}

void UPlayerInventoryComponent::InteractWithActiveItem()
{
    FItemSaveRecord Record;
    if (!GetActiveItem(Record)) return;
    
    if (ItemInHand.GetClass()->ImplementsInterface(UInteractable::StaticClass()))
    {
        UE_LOG(LogPlayer, Log, TEXT("Interacting with item: %s"), *ItemInHand->GetName())
        IInteractable::Execute_Interact(ItemInHand, GetOwner());
    }
}

void UPlayerInventoryComponent::OnItemMoved(int32 FromIndex, int32 ToIndex)
{
    if (EquippedItemIndex == FromIndex)
    {
        EquippedItemIndex = ToIndex;
        EquipActiveItem();
        OnActiveSlotChanged.Broadcast(ToIndex);
        return;
    }
}

void UPlayerInventoryComponent::SaveEquippedItemState()
{
    if (!ItemInHand || !Items.IsValidIndex(EquippedItemIndex))
    {
        return;
    }

    Items[EquippedItemIndex].Bytes.Empty();
    ISaveable::Execute_OnSave(ItemInHand, Items[EquippedItemIndex].Bytes);
}
