// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimeManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FCallMinutePassedSignature,
	int32, Day,
	float, Time
);

UCLASS()
class LOOK_OUT_API ATimeManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATimeManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
	float DayLength = 3600;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
	float RealHoursPerDay = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
	float TimeDilation = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Temp")
	int LastMinute = 0;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(BlueprintAssignable, Category="Events")
	FCallMinutePassedSignature OnCallMinutePassed;
	
	UFUNCTION(BlueprintCallable, Category = "Events")
	void TriggerMinutePassed(int Day, float Time);
	
	void PerformTimeUpdate(float DeltaTime);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
	bool bTimeStopped = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Date & Time")
	int Day = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Date & Time")
	float Time = 0;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UDirectionalLightComponent* DirectionalLight;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UExponentialHeightFogComponent* ExponentialHeightFog;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USkyAtmosphereComponent* SkyAtmosphere;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USkyLightComponent* SkyLight;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UVolumetricCloudComponent* VolumetricCloud;
};
