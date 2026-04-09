// Copyright notice: Fill out in Project Settings.

#include "Characters/BaseCharacter.h"

#include "Camera/CameraComponent.h"
#include "Characters/Components/PlayerGrabComponent.h"
#include "Characters/Components/PlayerMovementComponent.h"
#include "Characters/Components/PlayerObjectHintsComponent.h"
#include "Characters/Components/PlayerSideInteractionComponent.h"
#include "Components/CapsuleComponent.h"
#include "Core/Enums/GrabbableObjectType.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Characters/Components/PlayerInputsComponent.h"
#include "Characters/Components/PlayerInventoryComponent.h"
#include "Components/PrimitiveComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Components initialization
	PlayerMovementController = CreateDefaultSubobject<UPlayerMovementComponent>(TEXT("PlayerMovementController"));
	PlayerGrabComponent = CreateDefaultSubobject<UPlayerGrabComponent>(TEXT("PlayerGrabComponent"));
	PlayerInteractionComponent = CreateDefaultSubobject<UPlayerSideInteractionComponent>(
		TEXT("PlayerInteractionComponent"));
	PlayerObjectHintsComponent = CreateDefaultSubobject<
		UPlayerObjectHintsComponent>(TEXT("PlayerObjectHintsComponent"));
	PlayerInputsComponent = CreateDefaultSubobject<UPlayerInputsComponent>(TEXT("PlayerInputsComponent"));
	PlayerInventoryComponent = CreateDefaultSubobject<UPlayerInventoryComponent>(TEXT("PlayerInventoryComponent"));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraint"));
	HandSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HandSceneComponent"));
	HandSwaySourceComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HandSwaySourceComponent"));

	SpringArm->SetupAttachment(GetMesh());
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	PhysicsConstraint->SetupAttachment(GetCapsuleComponent());
	HandSceneComponent->SetupAttachment(RootComponent);
	HandSwaySourceComponent->SetupAttachment(GetMesh());
	HandSwaySourceComponent->SetRelativeLocation(HandItemOffset);
	HandSwaySourceComponent->SetRelativeRotation(HandItemRotation);

	// Spring Arm configuration
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

	if (HandSwaySourceComponent)
	{
		InitialHandSwayRelativeLocation = HandSwaySourceComponent->GetRelativeLocation();
		InitialHandSwayRelativeRotation = HandSwaySourceComponent->GetRelativeRotation();
	}

	UpdateHandItemTransform(0.0f);

	if (SpringArm)
	{
		DefaultCameraLagSpeed = SpringArm->CameraLagSpeed;
		DefaultCameraRotationLagSpeed = SpringArm->CameraRotationLagSpeed;
		DefaultCameraLagMaxDistance = SpringArm->CameraLagMaxDistance;
	}
	
	if (GameplayScreenWidgetClass)
	{
		GameplayScreenWidget = CreateWidget<UBaseGameplayScreenWidget>(GetWorld(), GameplayScreenWidgetClass);
		if (GameplayScreenWidget)
		{
			GameplayScreenWidget->AddToViewport();
		}
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
	UpdateHandItemTransform(DeltaTime);

	if (PlayerGrabComponent && PlayerGrabComponent->IsGrabbingObject)
	{
		PlayerGrabComponent->ProcessGrabbing(DeltaTime);
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

void ABaseCharacter::ConfigureEquippedItem(AActor* EquippedItem) const
{
	if (!EquippedItem)
	{
		return;
	}

	TInlineComponentArray<UPrimitiveComponent*> PrimitiveComponents(EquippedItem);
	for (UPrimitiveComponent* PrimitiveComponent : PrimitiveComponents)
	{
		if (!PrimitiveComponent)
		{
			continue;
		}

		PrimitiveComponent->SetEnableGravity(false);
		PrimitiveComponent->SetSimulatePhysics(false);
		PrimitiveComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ABaseCharacter::UpdateHandItemTransform(const float DeltaTime)
{
	if (!HandSceneComponent || !Camera)
	{
		return;
	}

	const FVector BaseWorldLocation =
		Camera->GetComponentLocation()
		+ Camera->GetForwardVector() * HandItemOffset.X
		+ Camera->GetRightVector() * HandItemOffset.Y
		+ Camera->GetUpVector() * HandItemOffset.Z;

	FRotator TargetRotation = Camera->GetComponentRotation() + HandItemRotation;
	if (bFollowCameraPitchWithHandItem && Controller)
	{
		const float ControlPitch = FRotator::NormalizeAxis(Controller->GetControlRotation().Pitch);
		TargetRotation.Pitch += ControlPitch * (HandItemPitchMultiplier - 1.0f);
	}

	FVector SwayWorldOffset = FVector::ZeroVector;
	FRotator SwayRotationOffset = FRotator::ZeroRotator;

	if (HandSwaySourceComponent && GetMesh())
	{
		const FVector RelativeSwayLocation =
			HandSwaySourceComponent->GetRelativeLocation() - InitialHandSwayRelativeLocation;
		const FRotator RelativeSwayRotation =
			HandSwaySourceComponent->GetRelativeRotation() - InitialHandSwayRelativeRotation;

		const FVector ScaledRelativeSwayLocation = FVector(
			RelativeSwayLocation.X * HandSwayLocationMultiplier.X,
			RelativeSwayLocation.Y * HandSwayLocationMultiplier.Y,
			RelativeSwayLocation.Z * HandSwayLocationMultiplier.Z
		);

		SwayWorldOffset = GetMesh()->GetComponentTransform().TransformVectorNoScale(ScaledRelativeSwayLocation);
		SwayRotationOffset = FRotator(
			RelativeSwayRotation.Pitch * HandSwayRotationMultiplier.Pitch,
			RelativeSwayRotation.Yaw * HandSwayRotationMultiplier.Yaw,
			RelativeSwayRotation.Roll * HandSwayRotationMultiplier.Roll
		);
	}

	const FVector TargetWorldLocation = BaseWorldLocation + SwayWorldOffset;
	const FRotator TargetWorldRotation = TargetRotation + SwayRotationOffset;

	if (DeltaTime <= 0.0f || CurrentHandWorldLocation.IsZero() && CurrentHandWorldRotation.IsZero())
	{
		CurrentHandWorldLocation = TargetWorldLocation;
		CurrentHandWorldRotation = TargetWorldRotation;
	}
	else
	{
		CurrentHandWorldLocation = FMath::VInterpTo(
			CurrentHandWorldLocation,
			TargetWorldLocation,
			DeltaTime,
			HandItemLocationInterpSpeed
		);
		CurrentHandWorldRotation = FMath::RInterpTo(
			CurrentHandWorldRotation,
			TargetWorldRotation,
			DeltaTime,
			HandItemRotationInterpSpeed
		);
	}

	HandSceneComponent->SetWorldLocation(CurrentHandWorldLocation);
	HandSceneComponent->SetWorldRotation(CurrentHandWorldRotation);
}

void ABaseCharacter::UpdateCameraDragResponse(const float DeltaTime)
{
	if (!SpringArm || !PlayerGrabComponent)
	{
		return;
	}

	const bool bIsDraggingHeavyObject =
		PlayerGrabComponent->IsGrabbingObject && PlayerGrabComponent->GetGrabbedObjectType() == Heavyweight;
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
