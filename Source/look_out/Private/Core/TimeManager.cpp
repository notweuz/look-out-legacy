// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/TimeManager.h"

// Sets default values
ATimeManager::ATimeManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATimeManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATimeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bTimeStopped)
	{
		float DayMultiplier = TimeDilation * RealHoursPerDay;
		float TimeDelta = DeltaTime / DayMultiplier;
		float NewTime = Time + TimeDelta;
		Time = NewTime;
		int CurrentMinute = FMath::Floor(Time / 2.5);
		if (LastMinute != CurrentMinute)
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
	}
}
