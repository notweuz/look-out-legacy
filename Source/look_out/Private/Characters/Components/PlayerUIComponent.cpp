// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Characters/Components/PlayerUIComponent.h"

#include "Characters/BaseCharacter.h"

// Sets default values for this component's properties
UPlayerUIComponent::UPlayerUIComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerUIComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
	
	if (APlayerController* PC = Cast<APlayerController>(OwnerCharacter->GetController()); PC && PlayerUIClass)
	{
		PlayerUI = CreateWidget<UBasePlayerUI>(PC, PlayerUIClass);
		if (PlayerUI)
		{
			PlayerUI->AddToViewport();
			UE_LOG(LogTemp, Warning, TEXT("Player UI created"));
		} else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to setup player UI"));
		}
	}
}


// Called every frame
void UPlayerUIComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

