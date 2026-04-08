// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Enums/GrabbableObjectType.h"
#include "UObject/Interface.h"
#include "Grabbable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UGrabbable : public UInterface
{
	GENERATED_BODY()
};

class LOOK_OUT_API IGrabbable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Grabbing")
	EGrabbableObjectType GetGrabbableType();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Grabbing")
	FText GetGrabWidgetText();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Grabbing")
	void OnMouseScrollInput(float MouseInput);
};
