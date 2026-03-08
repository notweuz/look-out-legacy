// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#include "Widgets/BaseObjectHintsWidget.h"

#include "Interfaces/Describable.h"
#include "Interfaces/Grabbable.h"
#include "Interfaces/Interactable.h"
#include "Interfaces/Storeable.h"

void UBaseObjectHintsWidget::UpdateFromComponent(UActorComponent* Component) const
{
	if (!Component)
	{
		return;
	}

	UObject* Target = Component;
	if (!Target->GetClass()->ImplementsInterface(UGrabbable::StaticClass()) &&
		!Target->GetClass()->ImplementsInterface(UInteractable::StaticClass()) &&
		!Target->GetClass()->ImplementsInterface(UDescribable::StaticClass()) &&
		!Target->GetClass()->ImplementsInterface(UStoreable::StaticClass()))
	{
		Target = Component->GetOwner();
	}

	if (!Target)
	{
		return;
	}

	if (Target->GetClass()->ImplementsInterface(UGrabbable::StaticClass()))
	{
		FText GrabTextValue = IGrabbable::Execute_GetGrabWidgetText(Target);
		GrabText->SetText(GrabTextValue.IsEmpty() ? FText::FromString(TEXT("LMB - Grab")) : GrabTextValue);
		GrabText->SetVisibility(ESlateVisibility::Visible);
	}

	if (Target->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		InteractText->SetText(FText::FromString(TEXT("E - Interact")));
		InteractText->SetVisibility(ESlateVisibility::Visible);
	}

	if (Target->GetClass()->ImplementsInterface(UStoreable::StaticClass()))
	{
		StoreText->SetText(FText::FromString(TEXT("RMB - Collect")));
		StoreText->SetVisibility(ESlateVisibility::Visible);
	}

	if (Target->GetClass()->ImplementsInterface(UDescribable::StaticClass()))
	{
		if (AActor* OwnerActor = Cast<AActor>(Target))
		{
			Description->SetText(IDescribable::Execute_GetDescribeWidgetText(OwnerActor, OwnerActor));
		}
	}
}
