// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#include "Characters/Components/PlayerObjectHintsComponent.h"

#include "Characters/BaseCharacter.h"
#include "Characters/Components/PlayerSideInteractionComponent.h"
#include "Interfaces/Describable.h"
#include "Interfaces/Grabbable.h"
#include "Interfaces/Interactable.h"
#include "Interfaces/Storeable.h"

UPlayerObjectHintsComponent::UPlayerObjectHintsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerObjectHintsComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle, this, &UPlayerObjectHintsComponent::ScanForObject, 0.1f, true);
}

void UPlayerObjectHintsComponent::ScanForObject()
{
	if (!OwnerCharacter) return;

	const auto [Start, End] = OwnerCharacter->GetForwardVectorRelatedToCamera(
		OwnerCharacter->PlayerInteractionComponent->InteractDistance);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerCharacter);

#if WITH_EDITOR
	DrawDebugLine(OwnerCharacter->GetWorld(), Start, End, FColor::Magenta, false, 10.0f);
#endif

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

	USceneComponent* AttachTarget = Cast<USceneComponent>(Component);
	if (!AttachTarget)
	{
		AttachTarget = Component->GetOwner()->GetRootComponent();
	}
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
	CurrentWidgetComponent = NewObject<UWidgetComponent>(GetOwner());
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
	if (!CurrentWidgetComponent || !Component) return;

	const UBaseObjectHintsWidget* Widget = Cast<UBaseObjectHintsWidget>(CurrentWidgetComponent->GetWidget());
	if (!Widget) return;
	
	Widget->UpdateFromComponent(Cast<UActorComponent>(Component));
}

bool UPlayerObjectHintsComponent::ImplementsAnyHintInterface(const UClass* Class)
{
	return Class->ImplementsInterface(UInteractable::StaticClass())
		|| Class->ImplementsInterface(UGrabbable::StaticClass())
		|| Class->ImplementsInterface(UStoreable::StaticClass())
		|| Class->ImplementsInterface(UDescribable::StaticClass());
}
