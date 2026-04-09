// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#include "Widgets/Misc/BaseObjectHintsWidget.h"

#include "Interfaces/Describable.h"
#include "Interfaces/Grabbable.h"
#include "Interfaces/Interactable.h"
#include "Interfaces/Pickupable.h"
#include "World/Objects/BaseObject.h"

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
		!Target->GetClass()->ImplementsInterface(UDescribable::StaticClass()) &&
		!Target->GetClass()->ImplementsInterface(UPickupable::StaticClass()))
	{
		Target = Component->GetOwner();
	}

	if (!Target)
	{
		return;
	}

	if (DisplayName)
	{
		if (const ABaseObject* BaseObject = Cast<ABaseObject>(Target))
		{
			FText NameText = BaseObject->DisplayName;
			if (!NameText.IsEmptyOrWhitespace())
			{
				DisplayName->SetText(NameText);
			}
			else
			{
				DisplayName->SetText(FText::FromString(BaseObject->GetClass()->GetName()));
			}
		}
	}

	if (Target->GetClass()->ImplementsInterface(UGrabbable::StaticClass()))
	{
		if (GrabText)
		{
			FText GrabTextValue = IGrabbable::Execute_GetGrabWidgetText(Target);
			if (!GrabTextValue.IsEmptyOrWhitespace())
			{
				GrabText->SetText(GrabTextValue);
			}
			else
			{
				GrabText->SetText(FText::FromString(TEXT("LMB - Grab")));
			}
			GrabText->SetVisibility(ESlateVisibility::Visible);
		}
	}

	if (Target->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		if (InteractText)
		{
			FText InteractHint = IInteractable::Execute_GetInteractWidgetText(Target);
			if (!InteractHint.IsEmptyOrWhitespace())
			{
				InteractText->SetText(InteractHint);
			}
			else
			{
				InteractText->SetText(FText::FromString(TEXT("E - Interact")));
			}
			InteractText->SetVisibility(ESlateVisibility::Visible);
		}
	}

	if (Target->GetClass()->ImplementsInterface(UDescribable::StaticClass()) && Description)
	{
		if (AActor* OwnerActor = Cast<AActor>(Target))
		{
			FText DescText = IDescribable::Execute_GetDescribeWidgetText(OwnerActor, OwnerActor);
			if (!DescText.IsEmptyOrWhitespace())
			{
				Description->SetText(DescText);
			}
			else
			{
				Description->SetText(FText::GetEmpty());
			}
		}
	}
	
	if (Target->GetClass()->ImplementsInterface(UPickupable::StaticClass()))
	{
		if (AActor* OwnerActor = Cast<AActor>(Target))
		{
			StoreText->SetVisibility(ESlateVisibility::Visible);
			StoreText->SetText(FText::FromString("RMB - Collect"));
		}
	}
}
