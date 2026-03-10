// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/ActorAsItemResult.h"
#include "Data/Storage/Item.h"
#include "StorageComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LOOK_OUT_API UStorageComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStorageComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Storage", SaveGame)
	TArray<FItem> Storage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Storage")
	int32 MaxStorageWeight;

	UFUNCTION(BlueprintCallable, Category="Storage")
	int64 GetCurrentWeight() const;

	UFUNCTION(BlueprintCallable, Category="Storage")
	bool CanAddItem(const FItem& Item) const;

	UFUNCTION(BlueprintCallable, Category="Storage")
	bool CanSwapItems(const FItem& Incoming, const FItem& Outgoing) const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Storage")
	FItem RemoveItem(int32 Index);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Storage")
	void AddItem(AActor* Actor);
	
	UFUNCTION(BlueprintCallable, Category="Storage")
	FActorAsItemResult GetActorAsItemResult(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category="Storage")
	static UStorageComponent* GetStorageComponentFromActor(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category="Storage")
	UStorageComponent* GetStorageLink();

protected:
	static int64 GetItemTotalWeight(const FItem& Item);
};
