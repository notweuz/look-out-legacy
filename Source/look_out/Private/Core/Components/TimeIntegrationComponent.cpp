// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Components/TimeIntegrationComponent.h"

#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UTimeIntegrationComponent::UTimeIntegrationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
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


// Called every frame
void UTimeIntegrationComponent::TickComponent(const float DeltaTime, const ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTimeIntegrationComponent::OnMinutePassedTriggered(const int Day, const float Time)
{
	OnMinutePassedReceived(Day, Time);
}
