// Copyright notice: Fill out in Project Settings.

#include "Characters/BaseCharacter.h"

#include "Camera/CameraComponent.h"
#include "Characters/Components/GrabbingComponent.h"
#include "Characters/Components/MovementComponentExtended.h"
#include "Characters/Components/PlayerObjectHintsComponent.h"
#include "Characters/Components/PlayerSideInteractionComponent.h"
#include "Components/CapsuleComponent.h"
#include "Enums/GrabbableObjectType.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Characters/Components/PlayerInputsComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Components initialization
	MovementComponentExtended = CreateDefaultSubobject<UMovementComponentExtended>(TEXT("MovementComponentExtended"));
	GrabbingComponent = CreateDefaultSubobject<UGrabbingComponent>(TEXT("GrabbingComponent"));
	PlayerInteractionComponent = CreateDefaultSubobject<UPlayerSideInteractionComponent>(
		TEXT("PlayerInteractionComponent"));
	PlayerObjectHintsComponent = CreateDefaultSubobject<
		UPlayerObjectHintsComponent>(TEXT("PlayerObjectHintsComponent"));
	PlayerInputsComponent = CreateDefaultSubobject<UPlayerInputsComponent>(TEXT("PlayerInputsComponent"));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraint"));
	HandSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HandSceneComponent"));

	SpringArm->SetupAttachment(GetMesh());
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	PhysicsConstraint->SetupAttachment(GetCapsuleComponent());
	HandSceneComponent->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	// Spring Arm configurationotb
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->TargetArmLength = 0.0f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraLagSpeed = 20.0f;
	SpringArm->CameraRotationLagSpeed = 20.0f;
	SpringArm->CameraLagMaxDistance = 10.0f;

	// Physics Handle configuration
	PhysicsHandle->InterpolationSpeed = 10.0f;

	// Physics Constraint configuration
	PhysicsConstraint->SetLinearXLimit(LCM_Limited, 1.0f);
	PhysicsConstraint->SetLinearYLimit(LCM_Limited, 1.0f);
	PhysicsConstraint->SetLinearZLimit(LCM_Limited, 1.0f);
	PhysicsConstraint->ConstraintInstance.ProfileInstance.LinearLimit.bSoftConstraint = true;
	PhysicsConstraint->ConstraintInstance.ProfileInstance.LinearLimit.Stiffness = 30.0f;
	PhysicsConstraint->ConstraintInstance.ProfileInstance.LinearLimit.Damping = 10.0f;
	PhysicsConstraint->ConstraintInstance.ProfileInstance.bLinearPlasticity = true;
	PhysicsConstraint->ConstraintInstance.ProfileInstance.LinearPlasticityType = CCPT_Grow;
	PhysicsConstraint->ConstraintInstance.ProfileInstance.LinearPlasticityThreshold = 0.5f;
	PhysicsConstraint->SetLinearDriveAccelerationMode(false);

	// Character Movement configuration
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	MovementComp->bOrientRotationToMovement = true;
	MovementComp->bUseControllerDesiredRotation = true;
	MovementComp->bIgnoreBaseRotation = true;
	MovementComp->Mass = 60.0f;
	MovementComp->bPushForceUsingZOffset = true;
	MovementComp->GetNavAgentPropertiesRef().bCanCrouch = true;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (SpringArm)
	{
		DefaultCameraLagSpeed = SpringArm->CameraLagSpeed;
		DefaultCameraRotationLagSpeed = SpringArm->CameraRotationLagSpeed;
		DefaultCameraLagMaxDistance = SpringArm->CameraLagMaxDistance;
	}
}

void ABaseCharacter::Zoom(bool bZoomIn)
{
	Camera->SetFieldOfView(bZoomIn ? 50.0f : FieldOfView);
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCameraDragResponse(DeltaTime);

	if (GrabbingComponent && GrabbingComponent->IsGrabbingObject)
	{
		GrabbingComponent->ProcessGrabbing(DeltaTime);
	}
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (PlayerInputsComponent)
	{
		PlayerInputsComponent->SetupPlayerInput(PlayerInputComponent);
	}
}

FCameraRelatedForwardVectorResult ABaseCharacter::GetForwardVectorRelatedToCamera(const float VectorLength) const
{
	FCameraRelatedForwardVectorResult Result;

	if (!Camera)
	{
		return Result;
	}

	const FVector CameraLocation = Camera->GetComponentLocation();
	FVector CameraForward = Camera->GetForwardVector();
	CameraForward *= VectorLength;

	Result.StartVector = CameraLocation;
	Result.EndVector = CameraLocation + CameraForward;

	return Result;
}

void ABaseCharacter::UpdateCameraDragResponse(const float DeltaTime)
{
	if (!SpringArm || !GrabbingComponent)
	{
		return;
	}

	const bool bIsDraggingHeavyObject =
		GrabbingComponent->IsGrabbingObject && GrabbingComponent->GetGrabbedObjectType() == Heavyweight;
	const float TargetCameraLagSpeed = bIsDraggingHeavyObject ? HeavyDragCameraLagSpeed : DefaultCameraLagSpeed;
	const float TargetCameraRotationLagSpeed = bIsDraggingHeavyObject
		                                           ? HeavyDragCameraRotationLagSpeed
		                                           : DefaultCameraRotationLagSpeed;
	const float TargetCameraLagMaxDistance = bIsDraggingHeavyObject
		                                         ? HeavyDragCameraLagMaxDistance
		                                         : DefaultCameraLagMaxDistance;

	SpringArm->CameraLagSpeed = FMath::FInterpTo(
		SpringArm->CameraLagSpeed, TargetCameraLagSpeed, DeltaTime, HeavyDragCameraResponseInterpSpeed);
	SpringArm->CameraRotationLagSpeed = FMath::FInterpTo(
		SpringArm->CameraRotationLagSpeed, TargetCameraRotationLagSpeed, DeltaTime,
		HeavyDragCameraResponseInterpSpeed);
	SpringArm->CameraLagMaxDistance = FMath::FInterpTo(
		SpringArm->CameraLagMaxDistance, TargetCameraLagMaxDistance, DeltaTime,
		HeavyDragCameraResponseInterpSpeed);
}
