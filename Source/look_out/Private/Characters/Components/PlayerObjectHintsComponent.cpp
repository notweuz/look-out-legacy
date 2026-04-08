// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#include "Characters/Components/PlayerObjectHintsComponent.h"

#include "Characters/BaseCharacter.h"
#include "Characters/Components/PlayerSideInteractionComponent.h"
#include "Interfaces/Describable.h"
#include "Interfaces/Grabbable.h"
#include "Interfaces/Interactable.h"

UPlayerObjectHintsComponent::UPlayerObjectHintsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerObjectHintsComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle, this, &UPlayerObjectHintsComponent::ScanForObject, 0.1f, true);
	}
}

void UPlayerObjectHintsComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}

	DestroyHintsWidget();
	Super::EndPlay(EndPlayReason);
}

void UPlayerObjectHintsComponent::ScanForObject()
{
	if (!OwnerCharacter || !GetWorld() || !OwnerCharacter->PlayerInteractionComponent)
	{
		return;
	}

	const auto [Start, End] = OwnerCharacter->GetForwardVectorRelatedToCamera(
		OwnerCharacter->InteractionDistance);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerCharacter);

	if (!GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		ProcessNewComponent(nullptr);
		return;
	}

	if (UPrimitiveComponent* HitComponent = Hit.GetComponent(); HitComponent && ImplementsAnyHintInterface(
		HitComponent->GetClass()))
	{
		ProcessNewComponent(HitComponent);
	}
	else if (const AActor* HitActor = Hit.GetActor();
		HitActor && ImplementsAnyHintInterface(HitActor->GetClass()))
	{
		ProcessNewComponent(HitActor->GetRootComponent());
	}
	else
	{
		ProcessNewComponent(nullptr);
	}
}

USceneComponent* UPlayerObjectHintsComponent::ResolveAttachTarget(UActorComponent* Component) const
{
	if (USceneComponent* SceneComponent = Cast<USceneComponent>(Component))
	{
		return SceneComponent;
	}

	if (Component && Component->GetOwner())
	{
		return Component->GetOwner()->GetRootComponent();
	}

	return nullptr;
}

void UPlayerObjectHintsComponent::ProcessNewComponent(UActorComponent* Component)
{
	if (Component == CurrentHintsComponent)
	{
		UpdateWidgetHints(Component);
		return;
	}

	CurrentHintsComponent = Component;

	if (!Component || !HintsWidgetClass)
	{
		DestroyHintsWidget();
		return;
	}

	USceneComponent* AttachTarget = ResolveAttachTarget(Component);
	if (!AttachTarget)
	{
		DestroyHintsWidget();
		return;
	}

	if (!CurrentWidgetComponent)
	{
		CreateHintsWidget(AttachTarget);
	}
	else
	{
		CurrentWidgetComponent->AttachToComponent(AttachTarget,
		                                          FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}

	UpdateWidgetHints(Component);
}

void UPlayerObjectHintsComponent::CreateHintsWidget(USceneComponent* AttachTarget)
{
	if (!AttachTarget || !GetOwner())
	{
		return;
	}

	CurrentWidgetComponent = NewObject<UWidgetComponent>(GetOwner(), UWidgetComponent::StaticClass());
	CurrentWidgetComponent->SetupAttachment(AttachTarget);
	CurrentWidgetComponent->RegisterComponent();
	CurrentWidgetComponent->SetDrawAtDesiredSize(true);
	CurrentWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);

	UBaseObjectHintsWidget* Widget = CreateWidget<UBaseObjectHintsWidget>(GetWorld(), HintsWidgetClass);
	CurrentWidgetComponent->SetWidget(Widget);
}

void UPlayerObjectHintsComponent::DestroyHintsWidget()
{
	if (CurrentWidgetComponent)
	{
		CurrentWidgetComponent->DestroyComponent();
		CurrentWidgetComponent = nullptr;
	}
}

void UPlayerObjectHintsComponent::UpdateWidgetHints(UObject* Component) const
{
	if (!CurrentWidgetComponent || !Component)
	{
		return;
	}

	UBaseObjectHintsWidget* Widget = Cast<UBaseObjectHintsWidget>(CurrentWidgetComponent->GetWidget());
	if (!Widget)
	{
		return;
	}

	Widget->UpdateFromComponent(Cast<UActorComponent>(Component));
}

bool UPlayerObjectHintsComponent::ImplementsAnyHintInterface(const UClass* Class)
{
	if (!Class)
	{
		return false;
	}

	return Class->ImplementsInterface(UInteractable::StaticClass())
		|| Class->ImplementsInterface(UGrabbable::StaticClass())
		|| Class->ImplementsInterface(UDescribable::StaticClass());
}
