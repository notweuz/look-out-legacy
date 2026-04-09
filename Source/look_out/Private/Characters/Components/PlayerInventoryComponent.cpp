#include "Characters/Components/PlayerInventoryComponent.h"
#include "Interfaces/Pickupable.h"
#include "Objects/BaseLightweightObject.h"
#include "Kismet/GameplayStatics.h"

void UPlayerInventoryComponent::BeginPlay()
{
    Super::BeginPlay();
    HotbarSlots.Init(INDEX_NONE, HotbarSize);
}

bool UPlayerInventoryComponent::TryPickup(AActor* Actor)
{
    if (!Actor) return false;

    if (!Cast<ABaseLightweightObject>(Actor)) return false;

    if (!Actor->Implements<UPickupable>()) return false;

    UItemDefinition* Def = IPickupable::Execute_GetDefinition(Actor);
    if (!Def) return false;

    if (!CanFit(Def->Weight)) return false;

    FItemSaveRecord Record;
    Record.ItemClass = Actor->GetClass();
    Record.Weight    = Def->Weight;
    ISaveable::Execute_OnSave(Actor, Record.Bytes);

    int32 NewIndex = Items.Num();
    AddItem(Record);

    for (int32 i = 0; i < HotbarSlots.Num(); i++)
    {
        if (HotbarSlots[i] == INDEX_NONE)
        {
            AssignToHotbar(NewIndex, i);
            break;
        }
    }

    Actor->Destroy();
    return true;
}

void UPlayerInventoryComponent::DropActiveItem()
{
    int32 ItemIndex = GetActiveItemIndex();
    if (!Items.IsValidIndex(ItemIndex)) return;

    UnequipItem();

    FItemSaveRecord Record = Items[ItemIndex];
    UClass* Class = Record.ItemClass.LoadSynchronous();
    if (!Class) return;

    FVector DropLocation = GetOwner()->GetActorLocation()
        + GetOwner()->GetActorForwardVector() * 100.f;
    FTransform DropTransform(DropLocation);

    AActor* DroppedActor = GetWorld()->SpawnActorDeferred<AActor>(Class, DropTransform);
    ISaveable::Execute_OnLoad(DroppedActor, Record.Bytes);
    UGameplayStatics::FinishSpawningActor(DroppedActor, DropTransform);

    ClearHotbarSlot(ActiveSlotIndex);
    ShiftHotbarIndicesAfterRemoval(ItemIndex);
    RemoveItem(ItemIndex);
}

bool UPlayerInventoryComponent::AssignToHotbar(int32 ItemIndex, int32 HotbarSlot)
{
    if (!Items.IsValidIndex(ItemIndex)) return false;
    if (!HotbarSlots.IsValidIndex(HotbarSlot)) return false;

    for (int32 i = 0; i < HotbarSlots.Num(); i++)
    {
        if (HotbarSlots[i] == ItemIndex)
        {
            HotbarSlots[i] = INDEX_NONE;
            OnHotbarChanged.Broadcast(i);
        }
    }

    HotbarSlots[HotbarSlot] = ItemIndex;
    OnHotbarChanged.Broadcast(HotbarSlot);

    if (HotbarSlot == ActiveSlotIndex)
        RefreshHandItem();

    return true;
}

void UPlayerInventoryComponent::ClearHotbarSlot(int32 HotbarSlot)
{
    if (!HotbarSlots.IsValidIndex(HotbarSlot)) return;

    HotbarSlots[HotbarSlot] = INDEX_NONE;
    OnHotbarChanged.Broadcast(HotbarSlot);

    if (HotbarSlot == ActiveSlotIndex)
        RefreshHandItem();
}

void UPlayerInventoryComponent::SetActiveSlot(int32 SlotIndex)
{
    if (!HotbarSlots.IsValidIndex(SlotIndex)) return;
    if (SlotIndex == ActiveSlotIndex) return;

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
    SetActiveSlot((ActiveSlotIndex - 1 + HotbarSize) % HotbarSize);
}

bool UPlayerInventoryComponent::GetActiveItem(FItemSaveRecord& OutRecord) const
{
    int32 ItemIndex = GetActiveItemIndex();
    if (!Items.IsValidIndex(ItemIndex)) return false;

    OutRecord = Items[ItemIndex];
    return true;
}

int32 UPlayerInventoryComponent::GetActiveItemIndex() const
{
    if (!HotbarSlots.IsValidIndex(ActiveSlotIndex)) return INDEX_NONE;
    return HotbarSlots[ActiveSlotIndex];
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

    FItemSaveRecord Record;
    if (!GetActiveItem(Record)) return;

    UClass* Class = Record.ItemClass.LoadSynchronous();
    if (!Class) return;

    AActor* Owner = GetOwner();

    FActorSpawnParameters Params;
    Params.Owner = Owner;

    ItemInHand = GetWorld()->SpawnActor<AActor>(Class, FTransform::Identity, Params);
    if (!ItemInHand) return;

    ISaveable::Execute_OnLoad(ItemInHand, Record.Bytes);

    ItemInHand->AttachToActor(
        Owner,
        FAttachmentTransformRules::SnapToTargetIncludingScale,
        "hand_r"
    );

    OnItemEquipped.Broadcast(ItemInHand);
}

void UPlayerInventoryComponent::UnequipItem()
{
    if (!ItemInHand) return;

    int32 ItemIndex = GetActiveItemIndex();
    if (Items.IsValidIndex(ItemIndex))
    {
        Items[ItemIndex].Bytes.Empty();
        ISaveable::Execute_OnSave(ItemInHand, Items[ItemIndex].Bytes);
    }

    ItemInHand->Destroy();
    ItemInHand = nullptr;

    OnItemUnequipped.Broadcast();
}

void UPlayerInventoryComponent::ShiftHotbarIndicesAfterRemoval(int32 RemovedItemIndex)
{
    for (int32& SlotItemIndex : HotbarSlots)
    {
        if (SlotItemIndex > RemovedItemIndex)
            SlotItemIndex--;
    }
}