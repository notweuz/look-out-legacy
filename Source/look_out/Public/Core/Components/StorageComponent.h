// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/Storage/Item.h"
#include "StorageComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOOK_OUT_API UStorageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStorageComponent();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Storage")
	TArray<FItem> Storage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Storage")
	int32 MaxStorageWeight;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Storage")
	int32 GetRemainingStorage();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Storage")
	bool TransferItem(UStorageComponent* OldStorage, FItem Item);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Storage")
	FItem RemoveItem(int32 Index);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Storage")
	void AddItem(AActor* Actor);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Storage")
	AActor* RetrieveItem(int32 Index, FTransform SpawnTransform);
	
	UFUNCTION(BlueprintCallable, Category="Storage")
	static UStorageComponent* GetStorageComponentFromActor(AActor* Actor);
	
	UFUNCTION(BlueprintCallable, Category="Storage")
	UStorageComponent* GetStorageLink();
};
