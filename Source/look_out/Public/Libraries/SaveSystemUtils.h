// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class AActor;

class LOOK_OUT_API FSaveSystemUtils
{
public:
	static bool IsLevelPlacedActor(const AActor* Actor);
	static FString BuildStableLevelActorId(const AActor* Actor);
	static FString NormalizeObjectPath(const FString& ObjectPath);
};
