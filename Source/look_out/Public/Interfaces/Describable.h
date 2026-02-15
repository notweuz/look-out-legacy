// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Describable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDescribable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LOOK_OUT_API IDescribable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Description")
	FText GetDescribeWidgetText(AActor* Caller);
};
