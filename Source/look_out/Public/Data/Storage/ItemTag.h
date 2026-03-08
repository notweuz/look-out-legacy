// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StructUtils/InstancedStruct.h"
#include "ItemTag.generated.h"

UENUM(BlueprintType)
enum class EStatValueType : uint8
{
	None,
	Int,
	Float,
	Bool,
	String,
	Name,
	Struct,
};

USTRUCT(BlueprintType)
struct LOOK_OUT_API FItemTag
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString TagName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	EStatValueType Type = EStatValueType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 IntValue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	float FloatValue = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	bool BoolValue = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString StringValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FName NameValue = NAME_None;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FInstancedStruct StructValue;
};
