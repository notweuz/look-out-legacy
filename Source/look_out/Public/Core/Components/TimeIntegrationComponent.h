#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/TimeManager.h"
#include "Data/TimeFormattedResult.h"
#include "TimeIntegrationComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMinutePassedEvent, int32, Day, float, Time);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LOOK_OUT_API UTimeIntegrationComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTimeIntegrationComponent();

	UPROPERTY(BlueprintAssignable, Category = "Time Events")
	FOnMinutePassedEvent OnMinutePassed;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Time")
	ATimeManager* TimeManager;

	UFUNCTION(BlueprintPure, Category = "Time Formatting")
	FTimeFormattedResult GetTimeFormatted() const;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UFUNCTION()
	void OnMinutePassedTriggered(int32 Day, float Time);
};
