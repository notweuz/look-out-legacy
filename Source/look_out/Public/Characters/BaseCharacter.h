// Copyright notice: Fill out in Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Data/CameraRelatedForwardVectorResult.h"
#include "BaseCharacter.generated.h"

class UPlayerInputsComponent;
class UPlayerObjectHintsComponent;
class USpringArmComponent;
class UCameraComponent;
class UPhysicsHandleComponent;
class UPhysicsConstraintComponent;
class UPlayerMovementComponent;
class UPlayerGrabComponent;
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

	void UpdateCameraDragResponse(float DeltaTime);

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera|Heavy Drag")
	float HeavyDragCameraLagSpeed = 6.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera|Heavy Drag")
	float HeavyDragCameraRotationLagSpeed = 7.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera|Heavy Drag")
	float HeavyDragCameraLagMaxDistance = 24.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera|Heavy Drag")
	float HeavyDragCameraResponseInterpSpeed = 4.0f;

	// Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UPlayerMovementComponent* PlayerMovementController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UPlayerGrabComponent* PlayerGrabComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UPlayerSideInteractionComponent* PlayerInteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UPlayerObjectHintsComponent* PlayerObjectHintsComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UPlayerInputsComponent* PlayerInputsComponent;

private:
	float DefaultCameraLagSpeed = 0.0f;
	float DefaultCameraRotationLagSpeed = 0.0f;
	float DefaultCameraLagMaxDistance = 0.0f;
};
