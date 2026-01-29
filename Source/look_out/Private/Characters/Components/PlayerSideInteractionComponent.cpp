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
void UPlayerSideInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerSideInteractionComponent::Interact()
{
	FRelatedForwardVectorResult InteractVectors = OwnerCharacter->GetForwardVectorRelatedToCamera(InteractDistance);
	FHitResult HitResult;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwnerCharacter);

	const bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		InteractVectors.StartVector,
		InteractVectors.EndVector,
		ECC_Visibility,
		QueryParams
	);

	if (AActor* HitActor = HitResult.GetActor(); bHit && HitActor && HitActor->ActorHasTag("interactable"))
	{
		if (HitResult.GetActor()->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
		{
			IInteractable::Execute_Interact(HitActor, OwnerCharacter);
		}
	}
}
