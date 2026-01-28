// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
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

	// Set mesh rotation & position
	// GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FQuat(FRotator(0.0f, -90.0f, 0.0f)));

	SpringArm->SetupAttachment(GetMesh());
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	// Settings defaults of the spring arm
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->TargetArmLength = 0;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraLagSpeed = 20;
	SpringArm->CameraRotationLagSpeed = 20;
	SpringArm->CameraLagMaxDistance = 10;
	
	PhysicsHandle->InterpolationSpeed = 10;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bIgnoreBaseRotation = true;

	GetCharacterMovement()->Mass = 60;
	GetCharacterMovement()->bPushForceUsingZOffset = true;
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

void ABaseCharacter::MoveForward(float AxisValue)
{
	const FRotator ControlRotation = GetControlRotation();

	const FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	AddMovementInput(ForwardDirection, AxisValue);
}

void ABaseCharacter::MoveRight(float AxisValue)
{
	const FRotator ControlRotation = GetControlRotation();

	const FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(RightDirection, AxisValue);
}

void ABaseCharacter::ToggleSprint(bool State)
{
	if (State && CanSprint)
	{
		ChangeWalkSpeed(SprintSpeed);
		IsSprinting = true;
	}
	else if (!State)
	{
		ChangeWalkSpeed(WalkSpeed);
		IsSprinting = false;
	}
}

void ABaseCharacter::JumpAction()
{
	Jump();
}

void ABaseCharacter::ChangeWalkSpeed(float _WalkSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = _WalkSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = _WalkSpeed / 2;
}

void ABaseCharacter::Zoom(bool State)
{
	Camera->SetFieldOfView(State ? 50 : FieldOfView);
}

void ABaseCharacter::Look(float InputAxisX, float InputAxisY, bool bHoldingRMB)
{
	bool HaveToRotateItem = IsGrabbingObject && bHoldingRMB && GrabbedObjectType().bIsNotHeavy;

	if (!HaveToRotateItem)
	{
		AddControllerYawInput(InputAxisX);
		AddControllerPitchInput(InputAxisY);
	}
	else
	{
		InputAxisX *= -1;
		InputAxisY *= -1;

		GrabRotation.Roll = GrabRotation.Roll - InputAxisY;
		GrabRotation.Yaw = GrabRotation.Yaw - InputAxisX;
	}
}

FWeightCheckResult ABaseCharacter::GrabbedObjectType()
{
	FWeightCheckResult Result;

	UPrimitiveComponent* GrabbedComponent = nullptr;

	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		GrabbedComponent = PhysicsHandle->GetGrabbedComponent();
	}

	if (GrabbedComponent && HeavyObject)
	{
		bool IsObjectHeavy = GrabbedComponent == HeavyObject;

		if (IsObjectHeavy)
		{
			Result.bIsHeavy = true;
			Result.bIsNotHeavy = false;
		}
		else
		{
			Result.bIsHeavy = false;
			Result.bIsNotHeavy = true;
		}
	}
	else
	{
		Result.bIsHeavy = false;
		Result.bIsNotHeavy = false;
	}

	return Result;
}
