// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Saveable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class USaveable : public UInterface { GENERATED_BODY() };

class LOOK_OUT_API ISaveable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent) FGuid GetSaveId();
	UFUNCTION(BlueprintNativeEvent) void OnSave(TArray<uint8>& OutBytes);
	UFUNCTION(BlueprintNativeEvent) void OnLoad(const TArray<uint8>& InBytes);
};
