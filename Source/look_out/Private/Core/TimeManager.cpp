// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/TimeManager.h"

#include "Components/DirectionalLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/VolumetricCloudComponent.h"
#include "Data/Save/TimeManagerSaveData.h"
#include "Libraries/SaveSystemUtils.h"
#include "Misc/LogCategories.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/MemoryWriter.h"

namespace TimeManagerPrivate
{
	void SerializeSaveData(FArchive& Archive, FTimeManagerSaveData& SaveData)
	{
		Archive << SaveData.SaveId;
		Archive << SaveData.Day;
		Archive << SaveData.Time;
		Archive << SaveData.DayLength;
		Archive << SaveData.RealHoursPerDay;
		Archive << SaveData.TimeDilation;
		Archive << SaveData.bTimeStopped;
	}
}

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
	EnsureSaveId();
}

// Called when the game starts or when spawned
void ATimeManager::BeginPlay()
{
	Super::BeginPlay();
	EnsureSaveId();
	UE_LOG(LogLookOutGame, Log, TEXT("Time Manager has been started"));

	PerformTimeUpdate(0);
}

// Called every frame
void ATimeManager::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	PerformTimeUpdate(DeltaTime);
}

void ATimeManager::EnsureSaveId()
{
	if (!PersistentSaveId.IsValid() && !FSaveSystemUtils::IsLevelPlacedActor(this))
	{
		PersistentSaveId = FGuid::NewGuid();
	}
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

void ATimeManager::OnSave_Implementation(TArray<uint8>& OutData)
{
	EnsureSaveId();

	FTimeManagerSaveData SaveData;
	SaveData.SaveId = PersistentSaveId;
	SaveData.Day = Day;
	SaveData.Time = Time;
	SaveData.DayLength = DayLength;
	SaveData.RealHoursPerDay = RealHoursPerDay;
	SaveData.TimeDilation = TimeDilation;
	SaveData.bTimeStopped = bTimeStopped;

	FMemoryWriter Writer(OutData, true);
	TimeManagerPrivate::SerializeSaveData(Writer, SaveData);
}

void ATimeManager::OnLoad_Implementation(const TArray<uint8>& InData)
{
	if (InData.Num() == 0)
	{
		return;
	}

	FTimeManagerSaveData SaveData;
	FMemoryReader Reader(InData, true);
	TimeManagerPrivate::SerializeSaveData(Reader, SaveData);

	PersistentSaveId = SaveData.SaveId;
	Day = SaveData.Day;
	Time = SaveData.Time;
	DayLength = SaveData.DayLength;
	RealHoursPerDay = SaveData.RealHoursPerDay;
	TimeDilation = SaveData.TimeDilation;
	bTimeStopped = SaveData.bTimeStopped;
	LastMinute = FMath::FloorToInt(Time / 2.5f);
	PerformTimeUpdate(0.0f);
}

FString ATimeManager::GetSaveID_Implementation() const
{
	if (FSaveSystemUtils::IsLevelPlacedActor(this))
	{
		return FSaveSystemUtils::BuildStableLevelActorId(this);
	}

	return PersistentSaveId.IsValid() ? PersistentSaveId.ToString(EGuidFormats::DigitsWithHyphens) : FString();
}
