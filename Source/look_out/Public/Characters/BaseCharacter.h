// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

USTRUCT(BlueprintType)
struct FWeightCheckResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	bool bIsHeavy = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsNotHeavy = false;
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
	virtual FWeightCheckResult GrabbedObjectType();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UMovementComponentExtended* MovementComponentExtended;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USpringArmComponent* SpringArm;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCameraComponent* Camera;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPhysicsHandleComponent* PhysicsHandle;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Base Values")
	float InteractDistance = 200;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Base Values")
	float GrabDistance = 200;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Base Values")
	float FieldOfView = 90;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Base Values")
	float Strength = 1500;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grabbing")
	UPrimitiveComponent* HeavyObject = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Grabbing")
	bool IsGrabbingObject = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Grabbing")
	FRotator GrabRotation = FRotator::ZeroRotator;
};
