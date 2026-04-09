// Copyright notice: Fill out in Project Settings.

#include "Characters/Components/PlayerGrabComponent.h"

#include "Camera/CameraComponent.h"
#include "Characters/BaseCharacter.h"
#include "Characters/Components/PlayerMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Interfaces/Grabbable.h"
#include "Misc/LogCategories.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

UPlayerGrabComponent::UPlayerGrabComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerGrabComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
}

void UPlayerGrabComponent::TickComponent(const float DeltaTime, const ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UPlayerGrabComponent::HasRequiredComponents() const
{
	return OwnerCharacter &&
		OwnerCharacter->Camera &&
		OwnerCharacter->PhysicsHandle &&
		OwnerCharacter->PhysicsConstraint &&
		OwnerCharacter->PlayerMovementController;
}

AActor* UPlayerGrabComponent::GetTargetActorForScrollInput() const
{
	const UPrimitiveComponent* TargetComponent = StaticObject ? StaticObject : HeavyObject;
	if (TargetComponent && TargetComponent->GetOwner())
	{
		return TargetComponent->GetOwner();
	}

	return nullptr;
}

EObjectType UPlayerGrabComponent::GetGrabbedObjectType() const
{
	if (StaticObject)
	{
		return EObjectType::Static;
	}
	if (HeavyObject)
	{
		return EObjectType::Heavyweight;
	}

	if (OwnerCharacter && OwnerCharacter->PhysicsHandle && OwnerCharacter->PhysicsHandle->GetGrabbedComponent())
	{
		return EObjectType::Lightweight;
	}

	return EObjectType::None;
}

void UPlayerGrabComponent::ToggleGrab(bool bGrab)
{
	if (!HasRequiredComponents())
	{
		return;
	}

	UE_LOG(LogPlayer, Log, TEXT("Player toggled grabbing to: %s"), bGrab ? TEXT("true") : TEXT("false"));
	if (bGrab)
	{
		GrabObject();
	}
	else
	{
		ReleaseObject();
	}
}

void UPlayerGrabComponent::GrabObject()
{
	if (!HasRequiredComponents() || !GetWorld())
	{
		return;
	}

	GrabDistance = OwnerCharacter->InteractionDistance;
	const auto [Start, End] = OwnerCharacter->GetForwardVectorRelatedToCamera(GrabDistance);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerCharacter);

#if WITH_EDITOR
	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 2.0f);
#endif

	FHitResult Hit;
	if (!GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		return;
	}

	AActor* HitActor = Hit.GetActor();
	UPrimitiveComponent* HitComponent = Hit.GetComponent();

	if (!HitActor || !HitComponent || !HitActor->GetClass()->ImplementsInterface(UGrabbable::StaticClass()))
	{
#if WITH_EDITOR
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 10.0f, 12, FColor::Red, false, 2.0f);
		const FString HitName = HitActor ? HitActor->GetName() : TEXT("None");
		UE_LOG(LogPlayer, Log, TEXT("GrabbingComponent hit non-grabbable actor: %s"), *HitName);
#endif
		return;
	}

#if WITH_EDITOR
	DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 10.0f, 12, FColor::Blue, false, 2.0f);
	UE_LOG(LogPlayer, Log, TEXT("GrabbingComponent hit grabbable actor: %s"), *HitActor->GetName());
#endif

	if (const EObjectType Type = IGrabbable::Execute_GetGrabbableType(HitActor); Type == EObjectType::Lightweight)
	{
		GrabRotation = FRotator::ZeroRotator;
		OwnerCharacter->PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitComponent, NAME_None, HitComponent->GetComponentLocation(), HitComponent->GetComponentRotation());

		HitComponent->SetEnableGravity(false);
		HitComponent->WakeAllRigidBodies();
		IsGrabbingObject = true;
		UE_LOG(LogPlayer, Log, TEXT("Player grabbed a Lightweight %s (Actor: %s)"), *HitComponent->GetName(),
		       *HitActor->GetName());
	}
	else if (Type == EObjectType::Heavyweight)
	{
		HeavyObject = HitComponent;
		OwnerCharacter->PhysicsConstraint->SetConstrainedComponents(
			HitComponent, NAME_None, OwnerCharacter->GetCapsuleComponent(), NAME_None);

		OwnerCharacter->PlayerMovementController->CanSprint = false;
		OwnerCharacter->PlayerMovementController->ToggleSprint(false);
		OwnerCharacter->PlayerMovementController->
		                ChangeWalkSpeed(OwnerCharacter->PlayerMovementController->DragSpeed);
		IsGrabbingObject = true;
		UE_LOG(LogPlayer, Log, TEXT("Player grabbed a EObjectType::Heavyweight %s (Actor: %s)"), *HitComponent->GetName(),
		       *HitActor->GetName());
	}
	else if (Type == EObjectType::Static)
	{
		StaticObject = HitComponent;
		OwnerCharacter->PlayerMovementController->CanSprint = false;
		OwnerCharacter->PlayerMovementController->ToggleSprint(false);
		OwnerCharacter->PlayerMovementController->
		                ChangeWalkSpeed(OwnerCharacter->PlayerMovementController->DragSpeed);
		IsGrabbingObject = true;
		UE_LOG(LogPlayer, Log, TEXT("Player grabbed a Static %s (Actor: %s)"), *HitComponent->GetName(),
		       *HitActor->GetName());
	}
}

void UPlayerGrabComponent::ReleaseObject()
{
	if (!HasRequiredComponents())
	{
		IsGrabbingObject = false;
		return;
	}

	if (const EObjectType Type = GetGrabbedObjectType(); Type == EObjectType::Lightweight)
	{
		UPrimitiveComponent* GrabbedComponent = OwnerCharacter->PhysicsHandle->GetGrabbedComponent();
		OwnerCharacter->PhysicsHandle->ReleaseComponent();

		if (GrabbedComponent)
		{
			GrabbedComponent->SetEnableGravity(true);
			GrabbedComponent->WakeAllRigidBodies();
			UE_LOG(LogPlayer, Log, TEXT("Player released a Lightweight %s"), *GrabbedComponent->GetName());
		}
	}
	else if (Type == EObjectType::Heavyweight)
	{
		if (HeavyObject)
		{
			UE_LOG(LogPlayer, Log, TEXT("Player released a EObjectType::Heavyweight %s"), *HeavyObject->GetName());
		}

		HeavyObject = nullptr;
		OwnerCharacter->PhysicsConstraint->BreakConstraint();
		OwnerCharacter->PlayerMovementController->CanSprint = true;
		OwnerCharacter->PlayerMovementController->
		                ChangeWalkSpeed(OwnerCharacter->PlayerMovementController->WalkSpeed);
	}
	else if (Type == EObjectType::Static)
	{
		if (StaticObject)
		{
			UE_LOG(LogPlayer, Log, TEXT("Player released a Static %s"), *StaticObject->GetName());
		}

		StaticObject = nullptr;
		OwnerCharacter->PlayerMovementController->CanSprint = true;
		OwnerCharacter->PlayerMovementController->
		                ChangeWalkSpeed(OwnerCharacter->PlayerMovementController->WalkSpeed);
	}

	IsGrabbingObject = false;
}

void UPlayerGrabComponent::ProcessGrabbing(const float DeltaTime)
{
	if (!HasRequiredComponents())
	{
		return;
	}

	if (const EObjectType Type = GetGrabbedObjectType(); Type == EObjectType::Lightweight)
	{
		ProcessLightweightGrabbing(DeltaTime);
	}
	else if (Type == EObjectType::Heavyweight)
	{
		ProcessHeavyweightGrabbing();
	}
	else if (Type == EObjectType::Static)
	{
		ProcessStaticGrabbing();
	}
}

void UPlayerGrabComponent::ProcessLightweightGrabbing(float DeltaTime) const
{
	if (!HasRequiredComponents())
	{
		return;
	}

	const auto [Start, End] = OwnerCharacter->GetForwardVectorRelatedToCamera(GrabDistance);
	OwnerCharacter->PhysicsHandle->SetTargetLocation(End);

	if (UPrimitiveComponent* GrabbedComponent = OwnerCharacter->PhysicsHandle->GetGrabbedComponent())
	{
		const FRotator CameraRot = OwnerCharacter->Camera->GetComponentRotation();
		const FRotator BaseYaw(0.0f, CameraRot.Yaw, 0.0f);
		const FQuat TargetQuat = FQuat(BaseYaw) * FQuat(GrabRotation);

		const FRotator CurrentRot = GrabbedComponent->GetComponentRotation();
		const FRotator SmoothRot = FMath::RInterpTo(CurrentRot, TargetQuat.Rotator(), DeltaTime, 50.0f);

		OwnerCharacter->PhysicsHandle->SetTargetRotation(SmoothRot);
		GrabbedComponent->WakeAllRigidBodies();
	}
}

void UPlayerGrabComponent::ProcessHeavyweightGrabbing() const
{
	if (!HasRequiredComponents())
	{
		return;
	}

	UPrimitiveComponent* Component1 = nullptr;
	FName BoneName1;
	UPrimitiveComponent* Component2 = nullptr;
	FName BoneName2;

	OwnerCharacter->PhysicsConstraint->GetConstrainedComponents(Component1, BoneName1, Component2, BoneName2);

	if (Component1 && Component2)
	{
		const float Distance = FVector::Dist(Component1->GetComponentLocation(), Component2->GetComponentLocation());
		if (Distance >= GrabDistance * 1.5f)
		{
			FVector Diff = Component1->GetComponentLocation() - Component2->GetComponentLocation();
			Diff.Normalize();
			OwnerCharacter->LaunchCharacter(Distance * Diff * Component1->GetComponentScale().X * 0.1f, false, false);
		}
	}
}

void UPlayerGrabComponent::ProcessStaticGrabbing() const
{
	if (!OwnerCharacter)
	{
		return;
	}

	if (const UCapsuleComponent* PlayerCapsule = OwnerCharacter->GetCapsuleComponent(); StaticObject && PlayerCapsule)
	{
		if (const float Distance = FVector::Dist(StaticObject->GetComponentLocation(),
		                                         PlayerCapsule->GetComponentLocation()); Distance >= GrabDistance *
			1.5f)
		{
			FVector Diff = StaticObject->GetComponentLocation() - PlayerCapsule->GetComponentLocation();
			Diff.Normalize();
			OwnerCharacter->LaunchCharacter(Distance * Diff * StaticObject->GetComponentScale().X * 0.1f, false, false);
		}
	}
}

void UPlayerGrabComponent::ThrowObject()
{
	const EObjectType Type = GetGrabbedObjectType();
	if (Type != EObjectType::Lightweight || !HasRequiredComponents())
	{
		return;
	}

	UPrimitiveComponent* GrabbedComponent = OwnerCharacter->PhysicsHandle->GetGrabbedComponent();
	if (!GrabbedComponent)
	{
		return;
	}

	const FVector ImpulseDirection = OwnerCharacter->Camera->GetForwardVector();

	const float Mass = FMath::Max(1.0f, GrabbedComponent->GetMass());
	const float FinalStrength = FMath::Clamp(ThrowStrength / Mass, 500.0f, ThrowStrength);

	ToggleGrab(false);
	GrabbedComponent->AddImpulse(ImpulseDirection * FinalStrength, NAME_None, true);
	UE_LOG(LogPlayer, Log, TEXT("Player threw an %s item"), *GrabbedComponent->GetName());
}

void UPlayerGrabComponent::ChangeGrabDistance(const float Delta)
{
	if (FMath::IsNearlyZero(Delta))
	{
		return;
	}

	const EObjectType Type = GetGrabbedObjectType();

	if (Type == EObjectType::Lightweight)
	{
		GrabDistance = FMath::Clamp(GrabDistance + Delta * 5.0f, MinGrabDistance,  OwnerCharacter->InteractionDistance);
		return;
	}

	if (AActor* TargetActor = GetTargetActorForScrollInput();
		TargetActor && TargetActor->GetClass()->ImplementsInterface(UGrabbable::StaticClass()))
	{
		IGrabbable::Execute_OnMouseScrollInput(TargetActor, Delta);
	}
}

void UPlayerGrabComponent::RotateLightweightObject(const float AxisX, const float AxisY)
{
	if (!IsGrabbingObject)
	{
		return;
	}

	constexpr float RotationSpeed = 1.5f;
	GrabRotation.Yaw += AxisX * RotationSpeed;
	GrabRotation.Pitch += AxisY * RotationSpeed;
	GrabRotation.Normalize();
}
