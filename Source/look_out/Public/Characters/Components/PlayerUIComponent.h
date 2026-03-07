// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Widgets/Gameplay/BasePlayerUI.h"
#include "PlayerUIComponent.generated.h"


class ABaseCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOOK_OUT_API UPlayerUIComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerUIComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UBasePlayerUI> PlayerUIClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interface")
	UBasePlayerUI* PlayerUI;

private:
	UPROPERTY()
	ABaseCharacter* OwnerCharacter;
};
