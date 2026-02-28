// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "BaseObjectHintsWidget.generated.h"

UCLASS()
class LOOK_OUT_API UBaseObjectHintsWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DisplayName;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Description;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InteractText;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* GrabText;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* StoreText;

	void UpdateFromComponent(UActorComponent* Component) const;
};
