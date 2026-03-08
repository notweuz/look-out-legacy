// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Saveable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class USaveable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LOOK_OUT_API ISaveable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "SaveSystem")
	void OnSave(TArray<uint8>& OutData);

	UFUNCTION(BlueprintNativeEvent, Category = "SaveSystem")
	void OnLoad(const TArray<uint8>& InData);

	UFUNCTION(BlueprintNativeEvent, Category = "SaveSystem")
	FString GetSaveID() const;
};
