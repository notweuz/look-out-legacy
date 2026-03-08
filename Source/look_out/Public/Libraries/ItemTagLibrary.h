// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "StructUtils/InstancedStruct.h"
#include "Data/Storage/ItemTag.h"
#include "ItemTagLibrary.generated.h"

UCLASS()
class LOOK_OUT_API UItemTagLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "ItemTag|Make", meta = (DisplayName = "Make ItemTag (Int)"))
	static FItemTag MakeItemTagInt(const FString& TagName, int32 Value);

	UFUNCTION(BlueprintPure, Category = "ItemTag|Make", meta = (DisplayName = "Make ItemTag (Float)"))
	static FItemTag MakeItemTagFloat(const FString& TagName, float Value);

	UFUNCTION(BlueprintPure, Category = "ItemTag|Make", meta = (DisplayName = "Make ItemTag (Bool)"))
	static FItemTag MakeItemTagBool(const FString& TagName, bool Value);

	UFUNCTION(BlueprintPure, Category = "ItemTag|Make", meta = (DisplayName = "Make ItemTag (String)"))
	static FItemTag MakeItemTagString(const FString& TagName, const FString& Value);

	UFUNCTION(BlueprintPure, Category = "ItemTag|Make", meta = (DisplayName = "Make ItemTag (Name)"))
	static FItemTag MakeItemTagName(const FString& TagName, FName Value);

	UFUNCTION(BlueprintPure, Category = "ItemTag|Make", meta = (DisplayName = "Make ItemTag (Struct)"))
	static FItemTag MakeItemTagStruct(const FString& TagName, const FInstancedStruct& Value);

	UFUNCTION(BlueprintPure, Category = "ItemTag|Get", meta = (DisplayName = "Get Int Value"))
	static int32 GetItemTagInt(const FItemTag& Tag);

	UFUNCTION(BlueprintPure, Category = "ItemTag|Get", meta = (DisplayName = "Get Float Value"))
	static float GetItemTagFloat(const FItemTag& Tag);

	UFUNCTION(BlueprintPure, Category = "ItemTag|Get", meta = (DisplayName = "Get Bool Value"))
	static bool GetItemTagBool(const FItemTag& Tag);

	UFUNCTION(BlueprintPure, Category = "ItemTag|Get", meta = (DisplayName = "Get String Value"))
	static FString GetItemTagString(const FItemTag& Tag);

	UFUNCTION(BlueprintPure, Category = "ItemTag|Get", meta = (DisplayName = "Get Name Value"))
	static FName GetItemTagName(const FItemTag& Tag);

	UFUNCTION(BlueprintPure, Category = "ItemTag|Get", meta = (DisplayName = "Get Struct Value"))
	static FInstancedStruct GetItemTagStruct(const FItemTag& Tag);

	UFUNCTION(BlueprintPure, Category = "ItemTag|Set", meta = (DisplayName = "Set Int Value"))
	static FItemTag SetItemTagInt(FItemTag Tag, int32 Value);

	UFUNCTION(BlueprintPure, Category = "ItemTag|Set", meta = (DisplayName = "Set Float Value"))
	static FItemTag SetItemTagFloat(FItemTag Tag, float Value);

	UFUNCTION(BlueprintPure, Category = "ItemTag|Set", meta = (DisplayName = "Set Bool Value"))
	static FItemTag SetItemTagBool(FItemTag Tag, bool Value);

	UFUNCTION(BlueprintPure, Category = "ItemTag|Set", meta = (DisplayName = "Set String Value"))
	static FItemTag SetItemTagString(FItemTag Tag, const FString& Value);

	UFUNCTION(BlueprintPure, Category = "ItemTag|Set", meta = (DisplayName = "Set Name Value"))
	static FItemTag SetItemTagName(FItemTag Tag, FName Value);

	UFUNCTION(BlueprintPure, Category = "ItemTag|Set", meta = (DisplayName = "Set Struct Value"))
	static FItemTag SetItemTagStruct(FItemTag Tag, const FInstancedStruct& Value);

	UFUNCTION(BlueprintPure, Category = "ItemTag|Utility", meta = (DisplayName = "To Display String"))
	static FString ItemTagToString(const FItemTag& Tag);

	UFUNCTION(BlueprintPure, Category = "ItemTag|Utility", meta = (DisplayName = "Has Tag Name"))
	static bool ItemTagHasName(const FItemTag& Tag, const FString& TagName);

	UFUNCTION(BlueprintPure, Category = "ItemTag|Utility", meta = (DisplayName = "Find Tag By Name"))
	static bool FindItemTagByName(const TArray<FItemTag>& Tags, const FString& TagName, FItemTag& OutTag);

	UFUNCTION(BlueprintPure, Category = "ItemTag|Utility", meta = (DisplayName = "Is Tag Type"))
	static bool IsItemTagType(const FItemTag& Tag, EStatValueType ExpectedType);
};