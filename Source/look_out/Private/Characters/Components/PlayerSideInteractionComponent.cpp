// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/PlayerSideInteractionComponent.h"

#include "Characters/BaseCharacter.h"
#include "Interfaces/Interactable.h"

// Sets default values for this component's properties
UPlayerSideInteractionComponent::UPlayerSideInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerSideInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
}


// Called every frame
void UPlayerSideInteractionComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerSideInteractionComponent::Interact()
{
    if (!OwnerCharacter) return;

    auto [Start, End] = OwnerCharacter->GetForwardVectorRelatedToCamera(InteractDistance);

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(OwnerCharacter);

    if (!GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
    {
        return;
    }

    UPrimitiveComponent* HitComp = Hit.GetComponent();
    AActor* HitActor = Hit.GetActor();

    UObject* Target = nullptr;

    if (HitComp && HitComp->Implements<UInteractable>())
    {
        Target = HitComp;
    }
    else if (HitActor && HitActor->Implements<UInteractable>())
    {
        Target = HitActor;
    }

    if (Target)
    {
        IInteractable::Execute_Interact(Target, OwnerCharacter);
    }
}
