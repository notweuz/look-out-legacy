// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "AdvancedGameUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class LOOK_OUT_API UAdvancedGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Settings")
	static UAdvancedGameUserSettings* GetAdvancedGameUserSettings();

	UPROPERTY(Config, BlueprintReadWrite, Category = "Controls")
	float MouseSensitivity = 1.0f;
	
	virtual void ApplySettings(bool bCheckForCommandLineOverrides) override;
};
