// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerMovementComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LOOK_OUT_API UPlayerMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerMovementComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual void MoveForward(float InputAxis);

	UFUNCTION(BlueprintCallable)
	virtual void MoveRight(float InputAxis);

	UFUNCTION(BlueprintCallable)
	virtual void Look(float InputAxisX, float InputAxisY);

	UFUNCTION(BlueprintCallable)
	virtual void JumpAction();

	UFUNCTION(BlueprintCallable)
	virtual void DoCrouch(bool State);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	virtual void ToggleSprint(bool State);

	UFUNCTION(BlueprintCallable)
	virtual void ChangeWalkSpeed(float WalkSpeed);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Base Values")
	float WalkSpeed = 400;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Base Values")
	float SprintSpeed = 600;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Base Values")
	float DragSpeed = 90;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera|Heavy Drag")
	float HeavyDragLookSensitivityMultiplier = 0.45f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="States")
	bool CanSprint = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="States")
	bool IsSprinting = false;

protected:
	UPROPERTY()
	class ABaseCharacter* OwnerCharacter;

private:
	bool HasMovementOwner() const;
	bool IsDraggingHeavyObject() const;
	class UCharacterMovementComponent* GetCharacterMovementComponent() const;
	float GetMouseSensitivity() const;
};
