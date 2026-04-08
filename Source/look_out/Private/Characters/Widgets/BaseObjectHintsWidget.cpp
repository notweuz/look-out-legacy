// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#include "Characters/Widgets/BaseObjectHintsWidget.h"

#include "Interfaces/Describable.h"
#include "Interfaces/Grabbable.h"
#include "Interfaces/Interactable.h"

void UBaseObjectHintsWidget::ResetHints() const
{
	if (DisplayName)
	{
		DisplayName->SetText(FText::GetEmpty());
	}

	if (Description)
	{
		Description->SetText(FText::GetEmpty());
	}

	if (InteractText)
	{
		InteractText->SetText(FText::GetEmpty());
		InteractText->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (GrabText)
	{
		GrabText->SetText(FText::GetEmpty());
		GrabText->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (StoreText)
	{
		StoreText->SetText(FText::GetEmpty());
		StoreText->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UBaseObjectHintsWidget::UpdateFromComponent(UActorComponent* Component) const
{
	ResetHints();

	if (!Component)
	{
		return;
	}

	UObject* Target = Component;
	if (!Target->GetClass()->ImplementsInterface(UGrabbable::StaticClass()) &&
		!Target->GetClass()->ImplementsInterface(UInteractable::StaticClass()) &&
		!Target->GetClass()->ImplementsInterface(UDescribable::StaticClass()))
	{
		Target = Component->GetOwner();
	}

	if (!Target)
	{
		return;
	}

	if (DisplayName)
	{
		if (const AActor* TargetActor = Cast<AActor>(Target))
		{
			DisplayName->SetText(FText::FromString(TargetActor->GetActorNameOrLabel()));
		}
		else
		{
			DisplayName->SetText(FText::FromString(Target->GetName()));
		}
	}

	if (Target->GetClass()->ImplementsInterface(UGrabbable::StaticClass()))
	{
		if (GrabText)
		{
			const FText GrabTextValue = IGrabbable::Execute_GetGrabWidgetText(Target);
			GrabText->SetText(GrabTextValue.IsEmpty() ? FText::FromString(TEXT("LMB - Grab")) : GrabTextValue);
			GrabText->SetVisibility(ESlateVisibility::Visible);
		}
	}

	if (Target->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		if (InteractText)
		{
			const FText InteractHint = IInteractable::Execute_GetInteractWidgetText(Target);
			InteractText->SetText(InteractHint.IsEmpty() ? FText::FromString(TEXT("E - Interact")) : InteractHint);
			InteractText->SetVisibility(ESlateVisibility::Visible);
		}
	}

	if (Target->GetClass()->ImplementsInterface(UDescribable::StaticClass()) && Description)
	{
		if (AActor* OwnerActor = Cast<AActor>(Target))
		{
			Description->SetText(IDescribable::Execute_GetDescribeWidgetText(OwnerActor, OwnerActor));
		}
	}
}
