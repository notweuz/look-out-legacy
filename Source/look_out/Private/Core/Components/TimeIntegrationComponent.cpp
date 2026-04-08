#include "Core/Components/TimeIntegrationComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/LogCategories.h"

UTimeIntegrationComponent::UTimeIntegrationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTimeIntegrationComponent::BeginPlay()
{
	Super::BeginPlay();
	BindToTimeManager();
}

void UTimeIntegrationComponent::BindToTimeManager()
{
	if (TimeManager)
	{
		TimeManager->OnCallMinutePassed.RemoveDynamic(this, &UTimeIntegrationComponent::OnMinutePassedTriggered);
	}

	if (!GetWorld())
	{
		TimeManager = nullptr;
		return;
	}

	TimeManager = TimeManager ? TimeManager : Cast<ATimeManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ATimeManager::StaticClass()));

	if (TimeManager)
	{
		TimeManager->OnCallMinutePassed.AddDynamic(this, &UTimeIntegrationComponent::OnMinutePassedTriggered);
	}
}

void UTimeIntegrationComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (TimeManager)
	{
		TimeManager->OnCallMinutePassed.RemoveDynamic(this, &UTimeIntegrationComponent::OnMinutePassedTriggered);
	}

	Super::EndPlay(EndPlayReason);
}

void UTimeIntegrationComponent::OnMinutePassedTriggered(const int32 Day, const float Time)
{
	UE_LOG(LogLookOutGame, Verbose, TEXT("[Time Integration Component Side] Triggered Minute Passed Event"));
	OnMinutePassed.Broadcast(Day, Time);
}

FTimeFormattedResult UTimeIntegrationComponent::GetTimeFormatted() const
{
	FTimeFormattedResult Result;

	if (!TimeManager || TimeManager->DayLength <= 0.0f)
	{
		return Result;
	}

	Result.Day = TimeManager->Day;

	const float NormalizedHours = (TimeManager->Time / TimeManager->DayLength) * 24.0f;
	const float TotalMinutes = NormalizedHours * 60.0f;
	const float TotalSeconds = TotalMinutes * 60.0f;

	Result.Hour = FMath::FloorToInt(NormalizedHours);
	Result.Minute = FMath::FloorToInt(TotalMinutes) % 60;
	Result.Second = FMath::FloorToInt(TotalSeconds) % 60;

	return Result;
}
