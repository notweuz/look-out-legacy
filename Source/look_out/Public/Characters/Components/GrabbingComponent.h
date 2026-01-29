// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GrabbingComponent.generated.h"

USTRUCT(BlueprintType)
struct FWeightCheckResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	bool bIsHeavy = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsNotHeavy = false;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOOK_OUT_API UGrabbingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabbingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual FWeightCheckResult GrabbedObjectType();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grabbing")
	UPrimitiveComponent* HeavyObject = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Grabbing")
	bool IsGrabbingObject = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Grabbing")
	FRotator GrabRotation = FRotator::ZeroRotator;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Base Values")
	float Strength = 1500;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Base Values")
	float GrabDistance = 200;
	
	virtual void LightweightObjectRotation(float InputAxisX, float InputAxisY);
	
protected:
	UPROPERTY()
	class ABaseCharacter* OwnerCharacter;
};
