#include "Core/Components/TimeIntegrationComponent.h"
#include "Kismet/GameplayStatics.h"

UTimeIntegrationComponent::UTimeIntegrationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
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

void UTimeIntegrationComponent::OnMinutePassedTriggered(const int32 Day, const float Time)
{
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
	
	const int32 TotalSeconds = FMath::FloorToInt(CurrentTime);
	
	Result.Hour = TotalSeconds / 3600;
	Result.Minute = (TotalSeconds % 3600) / 60;
	Result.Second = TotalSeconds % 60;
	
	return Result;
}
