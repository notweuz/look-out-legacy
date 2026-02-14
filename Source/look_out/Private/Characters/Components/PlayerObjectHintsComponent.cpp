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

	if (UPrimitiveComponent* HitComponent = Hit.GetComponent(); HitComponent && ImplementsAnyHintInterface(HitComponent->GetClass()))
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

	DestroyHintsWidget();
	CurrentHintsComponent = Component;

	if (Component && HintsWidgetClass)
	{
		CreateHintsWidget(Component);
		UpdateWidgetHints(Component);
	}
}

void UPlayerObjectHintsComponent::CreateHintsWidget(UActorComponent* Component)
{
	USceneComponent* AttachTarget = Cast<USceneComponent>(Component);
	if (!AttachTarget)
	{
		AttachTarget = Component->GetOwner()->GetRootComponent();
	}
	if (!AttachTarget) return;

	CurrentWidgetComponent = NewObject<UWidgetComponent>(Component->GetOwner());
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

void UPlayerObjectHintsComponent::UpdateWidgetHints(const UActorComponent* Component) const
{
	if (!CurrentWidgetComponent || !Component) return;

	const UBaseObjectHintsWidget* Widget = Cast<UBaseObjectHintsWidget>(CurrentWidgetComponent->GetWidget());
	if (!Widget) return;

	const UClass* CompClass = Component->GetClass();
	const AActor* Owner = Component->GetOwner();
	const UClass* ActorClass = Owner ? Owner->GetClass() : nullptr;

	auto Implements = [&](const TSubclassOf<UInterface> Interface)
	{
		return CompClass->ImplementsInterface(Interface)
			|| (ActorClass && ActorClass->ImplementsInterface(Interface));
	};

	if (Implements(UGrabbable::StaticClass()))
	{
		Widget->GrabText->SetVisibility(ESlateVisibility::Visible);
		Widget->GrabText->SetText(FText::FromString(TEXT("ЛКМ - взять")));
	}
	if (Implements(UInteractable::StaticClass()))
	{
		Widget->InteractText->SetVisibility(ESlateVisibility::Visible);
		Widget->InteractText->SetText(FText::FromString(TEXT("E - взаимодействовать")));
	}
}

bool UPlayerObjectHintsComponent::ImplementsAnyHintInterface(const UClass* Class)
{
	return Class->ImplementsInterface(UInteractable::StaticClass())
		|| Class->ImplementsInterface(UGrabbable::StaticClass())
		|| Class->ImplementsInterface(UDescribable::StaticClass());
}
