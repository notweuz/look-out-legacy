// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/WidgetComponent.h"
#include "Widgets/BaseObjectHintsWidget.h"
#include "PlayerObjectHintsComponent.generated.h"

class ABaseCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LOOK_OUT_API UPlayerObjectHintsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerObjectHintsComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UBaseObjectHintsWidget> HintsWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	UActorComponent* CurrentHintsComponent;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	ABaseCharacter* OwnerCharacter;

	UPROPERTY()
	UWidgetComponent* CurrentWidgetComponent;

	FTimerHandle TimerHandle;

	void ScanForObject();
	void ProcessNewComponent(UActorComponent* Component);
	void UpdateWidgetHints(const UActorComponent* Component) const;
	void CreateHintsWidget(USceneComponent* AttachTarget);
	void DestroyHintsWidget();

	static bool ImplementsAnyHintInterface(const UClass* Class);
};
