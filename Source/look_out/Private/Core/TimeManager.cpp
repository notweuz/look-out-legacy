// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/TimeManager.h"

#include "Components/DirectionalLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/VolumetricCloudComponent.h"
#include "Misc/LogCategories.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/MemoryWriter.h"

// Sets default values
ATimeManager::ATimeManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DirectionalLight = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("DirectionalLight"));
	ExponentialHeightFog = CreateDefaultSubobject<UExponentialHeightFogComponent>(TEXT("ExponentialHeightFog"));
	SkyAtmosphere = CreateDefaultSubobject<USkyAtmosphereComponent>(TEXT("SkyAtmosphere"));
	SkyLight = CreateDefaultSubobject<USkyLightComponent>(TEXT("SkyLight"));
	VolumetricCloud = CreateDefaultSubobject<UVolumetricCloudComponent>(TEXT("VolumetricCloud"));

	SkyLight->Intensity = 0.025;
}

// Called when the game starts or when spawned
void ATimeManager::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogLookOutGame, Log, TEXT("Time Manager has been started"));

	PerformTimeUpdate(0);
}

// Called every frame
void ATimeManager::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	PerformTimeUpdate(DeltaTime);
}

void ATimeManager::PerformTimeUpdate(const float DeltaTime)
{
	if (!bTimeStopped)
	{
		const float DayMultiplier = TimeDilation * RealHoursPerDay;
		const float TimeDelta = DeltaTime / DayMultiplier;
		const float NewTime = Time + TimeDelta;
		Time = NewTime;
		if (const int CurrentMinute = FMath::Floor(Time / 2.5); LastMinute != CurrentMinute)
		{
			LastMinute = CurrentMinute;
			if (OnCallMinutePassed.IsBound())
			{
				TriggerMinutePassed(Day, Time);
			}
		}

		if (Time >= DayLength)
		{
			Time = 0;
			Day++;
		}
		else
		{
			const float DayPercent = Time / DayLength;
			const float SunYaw = DayPercent * 360;
			const float SunPitch = FMath::Cos(FMath::DegreesToRadians(SunYaw - 180)) * -55;
			const FRotator SunRotation = FRotator(SunPitch, SunYaw, 0.0f);

			DirectionalLight->SetRelativeRotation(SunRotation);
		}
	}
}

void ATimeManager::TriggerMinutePassed(const int _Day, const float _Time) const
{
	if (OnCallMinutePassed.IsBound())
	{
		OnCallMinutePassed.Broadcast(_Day, _Time);
		UE_LOG(LogLookOutGame, Verbose, TEXT("[Time Manager Side] Triggered Minute Passed Event"));
	}
}
