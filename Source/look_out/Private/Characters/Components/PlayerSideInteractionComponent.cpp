// Copyright notice: Fill out in Project Settings.

#include "Characters/Components/PlayerSideInteractionComponent.h"

#include "Characters/BaseCharacter.h"
#include "Interfaces/Interactable.h"
#include "Misc/LogCategories.h"

UPlayerSideInteractionComponent::UPlayerSideInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
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

UObject* UPlayerSideInteractionComponent::ResolveInteractableTarget(const FHitResult& Hit) const
{
	if (UPrimitiveComponent* HitComponent = Hit.GetComponent();
		HitComponent && HitComponent->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		return HitComponent;
	}

	if (AActor* HitActor = Hit.GetActor(); HitActor && HitActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		return HitActor;
	}

	return nullptr;
}

void UPlayerSideInteractionComponent::Interact() const
{
	if (!OwnerCharacter || !GetWorld())
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

	AActor* HitActor = Hit.GetActor();
	UObject* InteractableTarget = ResolveInteractableTarget(Hit);

#if WITH_EDITOR
	if (InteractableTarget)
	{
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 10.0f, 12, FColor::Blue, false, 2.0f);
		const FString HitName = HitActor ? HitActor->GetName() : InteractableTarget->GetName();
		UE_LOG(LogPlayer, Log, TEXT("PSIComponent hit interactable actor: %s"), *HitName);
	}
	else
	{
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 10.0f, 12, FColor::Red, false, 2.0f);
		UE_LOG(LogPlayer, Log, TEXT("PSIComponent hit non-interactable actor: %s"),
		       HitActor ? *HitActor->GetName() : TEXT("None"));
	}
#endif

	if (InteractableTarget)
	{
		UE_LOG(LogPlayer, Log, TEXT("PSIComponent executing interaction on %s"), *InteractableTarget->GetName());
		IInteractable::Execute_Interact(InteractableTarget, OwnerCharacter);
	}
}
