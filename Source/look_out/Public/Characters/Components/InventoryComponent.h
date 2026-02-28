// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/Components/StorageComponent.h"
#include "InventoryComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LOOK_OUT_API UInventoryComponent : public UStorageComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrentActiveItemIndex = -1;
	
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY()
	class ABaseCharacter* OwnerCharacter;
};
