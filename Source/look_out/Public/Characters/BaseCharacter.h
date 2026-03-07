// Copyright notice: Fill out in Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PlayerUIComponent.h"
#include "GameFramework/Character.h"
#include "Data/CameraRelatedForwardVectorResult.h"
#include "BaseCharacter.generated.h"

class UBasePlayerUI;
class UPlayerObjectHintsComponent;
class UInventoryComponent;
class USpringArmComponent;
class UCameraComponent;
class UPhysicsHandleComponent;
class UPhysicsConstraintComponent;
class UMovementComponentExtended;
class UGrabbingComponent;
class UPlayerSideInteractionComponent;

UCLASS(Blueprintable, BlueprintType)
class LOOK_OUT_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void Zoom(bool bZoomIn);

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FCameraRelatedForwardVectorResult GetForwardVectorRelatedToCamera(float VectorLength) const;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPhysicsHandleComponent* PhysicsHandle;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* HandSceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPhysicsConstraintComponent* PhysicsConstraint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Camera")
	float FieldOfView = 90.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UMovementComponentExtended* MovementComponentExtended;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UGrabbingComponent* GrabbingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UPlayerSideInteractionComponent* PlayerInteractionComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UPlayerObjectHintsComponent* PlayerObjectHintsComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UInventoryComponent* InventoryComponent;
	
	UPROPERTY(BlueprintReadOnly, Category="Components")
	UPlayerUIComponent* PlayerUIComponent;
};