// Copyright notice: Fill out in Project Settings.

#include "Characters/Components/PlayerSideInteractionComponent.h"

#include "Characters/BaseCharacter.h"
#include "Interfaces/Interactable.h"

UPlayerSideInteractionComponent::UPlayerSideInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerSideInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
}

void UPlayerSideInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                    FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPlayerSideInteractionComponent::Interact() const
{
	if (!OwnerCharacter)
	{
		return;
	}

	const auto [Start, End] = OwnerCharacter->GetForwardVectorRelatedToCamera(InteractDistance);

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

	UPrimitiveComponent* HitComponent = Hit.GetComponent();
	AActor* HitActor = Hit.GetActor();

	UObject* InteractableTarget = nullptr;

	if (HitComponent && HitComponent->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		UE_LOG(LogTemp, Log, TEXT("PSIComponent found an interactable actor component %s"), *HitComponent->GetName())
		InteractableTarget = HitComponent;
	}
	else if (HitActor && HitActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		UE_LOG(LogTemp, Log, TEXT("PSIComponent found an interactable actor %s"), *HitActor->GetName())
		InteractableTarget = HitActor;
	}

#if WITH_EDITOR
	if (InteractableTarget)
	{
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 10.0f, 12, FColor::Blue, false, 2.0f);
		UE_LOG(LogTemp, Log, TEXT("PSIComponent hit interactable actor: %s"), *HitActor->GetName())
	}
	else
	{
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 10.0f, 12, FColor::Red, false, 2.0f);
		UE_LOG(LogTemp, Log, TEXT("PSIComponent hit non-interactable actor: %s"), *HitActor->GetName())
	}
#endif

	if (InteractableTarget)
	{
		UE_LOG(LogTemp, Log, TEXT("PSIComponent executing interaction on %s"), *InteractableTarget->GetName())
		IInteractable::Execute_Interact(InteractableTarget, OwnerCharacter);
	}
}
