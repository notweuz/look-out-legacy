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
	UFUNCTION(BlueprintNativeEvent, Category="Saving") void OnSave(TArray<uint8>& OutBytes);
	UFUNCTION(BlueprintNativeEvent, Category="Saving") void OnLoad(const TArray<uint8>& InBytes);
	UFUNCTION(BlueprintNativeEvent, Category="Saving") void OnPostLoadFromSave();
	UFUNCTION(BlueprintNativeEvent, Category="Saving") void DestroyPermanently();
	
	static void DefaultSaveObject(UObject* Object, TArray<uint8>& OutBytes);
	static void DefaultLoadObject(UObject* Object, const TArray<uint8>& InBytes);
};
