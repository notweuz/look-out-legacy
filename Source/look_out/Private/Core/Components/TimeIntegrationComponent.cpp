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

	if (!TimeManager)
	{
		TimeManager = Cast<ATimeManager>(
			UGameplayStatics::GetActorOfClass(GetWorld(), ATimeManager::StaticClass())
		);
	}

	if (TimeManager)
	{
		TimeManager->OnCallMinutePassed.AddDynamic(this, &UTimeIntegrationComponent::OnMinutePassedTriggered);
	}
}

void UTimeIntegrationComponent::TickComponent(const float DeltaTime, const ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
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

	if (!TimeManager)
	{
		return Result;
	}

	Result.Day = TimeManager->Day;

	const float CurrentTime = TimeManager->Time;
	const float DayLength = TimeManager->DayLength;

	const float NormalizedTime = CurrentTime / DayLength * 24.0f;

	Result.Hour = FMath::FloorToInt(NormalizedTime);
	Result.Minute = FMath::FloorToInt((NormalizedTime - Result.Hour) * 60.0f);
	Result.Second = FMath::FloorToInt(((NormalizedTime - Result.Hour) * 60.0f - Result.Minute) * 60.0f);

	return Result;
}
