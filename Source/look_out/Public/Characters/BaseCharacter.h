// Copyright notice: Fill out in Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Core/Data/CameraRelatedForwardVectorResult.h"
#include "Widgets/Screen/BaseGameplayScreenWidget.h"
#include "BaseCharacter.generated.h"

class UPlayerInventoryComponent;
class UPlayerInputsComponent;
class UPlayerObjectHintsComponent;
class USpringArmComponent;
class UCameraComponent;
class UPhysicsHandleComponent;
class UPhysicsConstraintComponent;
class UPlayerMovementComponent;
class UPlayerGrabComponent;
class UPlayerSideInteractionComponent;
class USkeletalMeshComponent;

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
	void UpdateHandItemTransform(float DeltaTime);

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	void ConfigureEquippedItem(AActor* EquippedItem) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FCameraRelatedForwardVectorResult GetForwardVectorRelatedToCamera(float VectorLength) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interface")
	TSubclassOf<UBaseGameplayScreenWidget> GameplayScreenWidgetClass;
	
	UPROPERTY(BlueprintReadOnly, Category="Interface")
	UBaseGameplayScreenWidget* GameplayScreenWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPhysicsHandleComponent* PhysicsHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* HandSceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* HandSwaySourceComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hand")
	FVector HandItemOffset = FVector(60.0f, 30.0f, -20.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hand")
	FRotator HandItemRotation = FRotator(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hand")
	bool bFollowCameraPitchWithHandItem = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hand") float HandItemPitchMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hand") FVector HandSwayLocationMultiplier = FVector(0.5f, 0.5f, 0.5f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hand") FRotator HandSwayRotationMultiplier = FRotator(1.0f, 1.0f, 1.0f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hand") FRotator EquippedItemFacingOffset = FRotator::ZeroRotator;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hand") float HandItemLocationInterpSpeed = 18.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hand") float HandItemRotationInterpSpeed = 18.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPhysicsConstraintComponent* PhysicsConstraint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Camera")
	float FieldOfView = 90.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interactions")
	float InteractionDistance = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera|Heavy Drag")
	float HeavyDragCameraLagSpeed = 6.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera|Heavy Drag")
	float HeavyDragCameraRotationLagSpeed = 7.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera|Heavy Drag")
	float HeavyDragCameraLagMaxDistance = 24.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera|Heavy Drag")
	float HeavyDragCameraResponseInterpSpeed = 4.0f;

	// Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components") UPlayerMovementComponent* PlayerMovementController;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components") UPlayerGrabComponent* PlayerGrabComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components") UPlayerSideInteractionComponent* PlayerInteractionComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components") UPlayerObjectHintsComponent* PlayerObjectHintsComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components") UPlayerInputsComponent* PlayerInputsComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components") UPlayerInventoryComponent* PlayerInventoryComponent;

private:
	float DefaultCameraLagSpeed = 0.0f;
	float DefaultCameraRotationLagSpeed = 0.0f;
	float DefaultCameraLagMaxDistance = 0.0f;
	FVector InitialHandSwayRelativeLocation = FVector::ZeroVector;
	FRotator InitialHandSwayRelativeRotation = FRotator::ZeroRotator;
	FVector CurrentHandWorldLocation = FVector::ZeroVector;
	FRotator CurrentHandWorldRotation = FRotator::ZeroRotator;
};
