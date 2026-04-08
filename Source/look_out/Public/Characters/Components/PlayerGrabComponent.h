// Copyright notice: Fill out in Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/Enums/GrabbableObjectType.h"
#include "PlayerGrabComponent.generated.h"

class ABaseCharacter;
class UPrimitiveComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LOOK_OUT_API UPlayerGrabComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerGrabComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EGrabbableObjectType GetGrabbedObjectType() const;

	UFUNCTION(BlueprintCallable)
	void ToggleGrab(bool bGrab);

	UFUNCTION(BlueprintCallable)
	void ProcessGrabbing(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void ChangeGrabDistance(float Delta);

	UFUNCTION(BlueprintCallable)
	void ThrowObject();

	UFUNCTION(BlueprintCallable)
	void RotateLightweightObject(float AxisX, float AxisY);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grabbing")
	UPrimitiveComponent* HeavyObject = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grabbing")
	UPrimitiveComponent* StaticObject = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Grabbing")
	bool IsGrabbingObject = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Grabbing")
	FRotator GrabRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Base Values")
	float ThrowStrength = 1500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Base Values")
	float MinGrabDistance = 75.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Base Values")
	float GrabDistance = 200.0f;

private:
	UPROPERTY()
	ABaseCharacter* OwnerCharacter;

	bool HasRequiredComponents() const;
	AActor* GetTargetActorForScrollInput() const;
	void GrabObject();
	void ReleaseObject();

	void ProcessLightweightGrabbing(float DeltaTime) const;
	void ProcessHeavyweightGrabbing() const;
	void ProcessStaticGrabbing() const;
};
