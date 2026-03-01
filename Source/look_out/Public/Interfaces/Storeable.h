// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/Storage/ItemTag.h"
#include "UObject/Interface.h"
#include "Storeable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UStoreable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LOOK_OUT_API IStoreable
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Storing")
	FText GetStoreWidgetText();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Storing")
	int32 GetItemWeight();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Storing")
	TArray<FItemTag> GetItemTags();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Storing")
    void ApplyItemTags(const TArray<FItemTag>& Tags);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Storing")
	UTexture2D* GetItemIcon();
};
