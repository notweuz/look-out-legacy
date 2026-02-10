// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseObjectHintsWidget.generated.h"

/**
 * 
 */
UCLASS()
class LOOK_OUT_API UBaseObjectHintsWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DisplayName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Description;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText StatusText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText InteractionHintText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText GrabHintText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText StoreHinText;
};
