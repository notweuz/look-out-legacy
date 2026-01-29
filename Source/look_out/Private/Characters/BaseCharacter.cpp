// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"

#include "Camera/CameraComponent.h"
#include "Characters/Components/GrabbingComponent.h"
#include "Characters/Components/MovementComponentExtended.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Creating camera component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraint"));

	// Set mesh rotation & position
	// GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FQuat(FRotator(0.0f, -90.0f, 0.0f)));

	SpringArm->SetupAttachment(GetMesh());
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	PhysicsConstraint->SetupAttachment(GetCapsuleComponent());
	
	// Spring Arm Defaults
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->TargetArmLength = 0;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraLagSpeed = 20;
	SpringArm->CameraRotationLagSpeed = 20;
	SpringArm->CameraLagMaxDistance = 10;
	
	// Physics Handle Defaults
	PhysicsHandle->InterpolationSpeed = 10;

	// Physics Constraint Defaults
	PhysicsConstraint->SetLinearXLimit(LCM_Limited, 1.0f);
	PhysicsConstraint->SetLinearYLimit(LCM_Limited, 1.0f);
	PhysicsConstraint->SetLinearZLimit(LCM_Limited, 1.0f);
	PhysicsConstraint->ConstraintInstance.ProfileInstance.LinearLimit.bSoftConstraint = true;
	PhysicsConstraint->ConstraintInstance.ProfileInstance.LinearLimit.Stiffness = 30;
	PhysicsConstraint->ConstraintInstance.ProfileInstance.LinearLimit.Damping = 10;
	PhysicsConstraint->ConstraintInstance.ProfileInstance.bLinearPlasticity = true;
	PhysicsConstraint->ConstraintInstance.ProfileInstance.LinearPlasticityType = CCPT_Grow;
	PhysicsConstraint->ConstraintInstance.ProfileInstance.LinearPlasticityThreshold = 0.5;
	PhysicsConstraint->SetLinearDriveAccelerationMode(false);
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bIgnoreBaseRotation = true;

	GetCharacterMovement()->Mass = 60;
	GetCharacterMovement()->bPushForceUsingZOffset = true;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	
	// Components
	
	MovementComponentExtended = CreateDefaultSubobject<UMovementComponentExtended>(TEXT("MovementComponentExtended"));
	GrabbingComponent = CreateDefaultSubobject<UGrabbingComponent>(TEXT("GrabbingComponent"));
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseCharacter::Zoom(bool State)
{
	Camera->SetFieldOfView(State ? 50 : FieldOfView);
}

FRelatedForwardVectorResult ABaseCharacter::GetForwardVectorRelatedToCamera(float VectorLength)
{
	FRelatedForwardVectorResult Result;
	
	const FVector CameraLocation = Camera->GetComponentLocation();
	FVector CameraForwardVector = Camera->GetForwardVector();
	
	CameraForwardVector *= VectorLength;
	Result.StartVector = CameraLocation;
	Result.EndVector = CameraLocation + CameraForwardVector;
	
	return Result;
}
