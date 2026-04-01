// Copyright notice: Fill out in Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerSideInteractionComponent.generated.h"

class ABaseCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LOOK_OUT_API UPlayerSideInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerSideInteractionComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Interact() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction")
	float InteractDistance = 200.0f;

private:
	UPROPERTY()
	ABaseCharacter* OwnerCharacter;

	UObject* ResolveInteractableTarget(const FHitResult& Hit) const;
};
