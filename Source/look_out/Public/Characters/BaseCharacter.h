// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GrabbingComponent.h"
#include "Components/MovementComponentExtended.h"
#include "Components/PlayerSideInteractionComponent.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

USTRUCT(BlueprintType)
struct FRelatedForwardVectorResult
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	FVector StartVector;
	
	UPROPERTY(BlueprintReadOnly)
	FVector EndVector;
};


UCLASS(Blueprintable, BlueprintType)
class LOOK_OUT_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	virtual void Zoom(bool State);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual FRelatedForwardVectorResult GetForwardVectorRelatedToCamera(float VectorLength);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USpringArmComponent* SpringArm;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCameraComponent* Camera;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPhysicsHandleComponent* PhysicsHandle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPhysicsConstraintComponent* PhysicsConstraint;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Base Values")
	float FieldOfView = 90;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMovementComponentExtended* MovementComponentExtended;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UGrabbingComponent* GrabbingComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPlayerSideInteractionComponent* PlayerSideInteractionComponent;
};
