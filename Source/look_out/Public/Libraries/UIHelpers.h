// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UIHelpers.generated.h"

/**
 * 
 */
UCLASS()
class LOOK_OUT_API UUIHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category="UI|Player")
	static ABaseCharacter* GetBasePlayerFromWidget(const UUserWidget* Widget);
};
