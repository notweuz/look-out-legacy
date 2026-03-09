// Copyright notice: Fill out in Project Settings.

#include "Characters/Components/GrabbingComponent.h"

#include "Camera/CameraComponent.h"
#include "Characters/BaseCharacter.h"
#include "Characters/Components/MovementComponentExtended.h"
#include "Components/CapsuleComponent.h"
#include "Interfaces/Grabbable.h"
#include "Misc/LogCategories.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

UGrabbingComponent::UGrabbingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGrabbingComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
}

void UGrabbingComponent::TickComponent(const float DeltaTime, const ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

EGrabbableObjectType UGrabbingComponent::GetGrabbedObjectType() const
{
	if (StaticObject)
	{
		return Static;
	}
	if (HeavyObject)
	{
		return Heavyweight;
	}

	if (OwnerCharacter && OwnerCharacter->PhysicsHandle && OwnerCharacter->PhysicsHandle->GetGrabbedComponent())
	{
		return Lightweight;
	}

	return None;
}

void UGrabbingComponent::ToggleGrab(bool bGrab)
{
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

void UGrabbingComponent::GrabObject()
{
	GrabDistance = MaxGrabDistance;
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

	if (!HitActor || !HitActor->GetClass()->ImplementsInterface(UGrabbable::StaticClass()))
	{
#if WITH_EDITOR
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 10.0f, 12, FColor::Red, false, 2.0f);
		UE_LOG(LogPlayer, Log, TEXT("GrabbingComponent hit non-grabbable actor: %s"), *HitActor->GetName())
#endif
		return;
	}

#if WITH_EDITOR
	DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 10.0f, 12, FColor::Blue, false, 2.0f);
	UE_LOG(LogPlayer, Log, TEXT("GrabbingComponent hit grabbable actor: %s"), *HitActor->GetName())
#endif

	IGrabbable* Grabbable = Cast<IGrabbable>(HitActor);

	if (const EGrabbableObjectType Type = IGrabbable::Execute_GetGrabbableType(HitActor); Type == Lightweight)
	{
		GrabRotation = FRotator::ZeroRotator;
		OwnerCharacter->PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitComponent, NAME_None, HitComponent->GetComponentLocation(), HitComponent->GetComponentRotation());

		HitComponent->SetEnableGravity(false);
		HitComponent->WakeAllRigidBodies();
		IsGrabbingObject = true;
		UE_LOG(LogPlayer, Log, TEXT("Player grabbed a Lightweight %s (Actor: %s)"), *HitComponent->GetName(),
		       *HitActor->GetName())
	}
	else if (Type == Heavyweight)
	{
		HeavyObject = HitComponent;
		OwnerCharacter->PhysicsConstraint->SetConstrainedComponents(
			HitComponent, NAME_None, OwnerCharacter->GetCapsuleComponent(), NAME_None);

		OwnerCharacter->MovementComponentExtended->CanSprint = false;
		OwnerCharacter->MovementComponentExtended->ToggleSprint(false);
		OwnerCharacter->MovementComponentExtended->
		                ChangeWalkSpeed(OwnerCharacter->MovementComponentExtended->DragSpeed);
		IsGrabbingObject = true;
		UE_LOG(LogPlayer, Log, TEXT("Player grabbed a Heavyweight %s (Actor: %s)"), *HitComponent->GetName(),
		       *HitActor->GetName())
	}
	else if (Type == Static)
	{
		StaticObject = HitComponent;
		IsGrabbingObject = true;
		UE_LOG(LogPlayer, Log, TEXT("Player grabbed a Static %s (Actor: %s)"), *HitComponent->GetName(),
		       *HitActor->GetName())
	}
}

void UGrabbingComponent::ReleaseObject()
{
	if (const EGrabbableObjectType Type = GetGrabbedObjectType(); Type == Lightweight)
	{
		UPrimitiveComponent* GrabbedComponent = OwnerCharacter->PhysicsHandle->GetGrabbedComponent();
		OwnerCharacter->PhysicsHandle->ReleaseComponent();

		if (GrabbedComponent)
		{
			GrabbedComponent->SetEnableGravity(true);
			GrabbedComponent->WakeAllRigidBodies();
		}
		UE_LOG(LogPlayer, Log, TEXT("Player released a Lightweight %s"), *GrabbedComponent->GetName())
	}
	else if (Type == Heavyweight)
	{
		UE_LOG(LogPlayer, Log, TEXT("Player released a Heavyweight %s"), *HeavyObject->GetName())
		HeavyObject = nullptr;
		OwnerCharacter->PhysicsConstraint->BreakConstraint();
		OwnerCharacter->MovementComponentExtended->CanSprint = true;
		OwnerCharacter->MovementComponentExtended->
		                ChangeWalkSpeed(OwnerCharacter->MovementComponentExtended->WalkSpeed);
	}
	else if (Type == Static)
	{
		UE_LOG(LogPlayer, Log, TEXT("Player released a Static %s"), *StaticObject->GetName())
		StaticObject = nullptr;
	}

	IsGrabbingObject = false;
}

void UGrabbingComponent::ProcessGrabbing(const float DeltaTime)
{
	if (const EGrabbableObjectType Type = GetGrabbedObjectType(); Type == Lightweight)
	{
		ProcessLightweightGrabbing(DeltaTime);
	}
	else if (Type == Heavyweight)
	{
		ProcessHeavyweightGrabbing();
	}
	else if (Type == Static)
	{
		ProcessStaticGrabbing();
	}
}

void UGrabbingComponent::ProcessLightweightGrabbing(float DeltaTime) const
{
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

void UGrabbingComponent::ProcessHeavyweightGrabbing() const
{
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

void UGrabbingComponent::ProcessStaticGrabbing() const
{
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

void UGrabbingComponent::ThrowObject()
{
	const EGrabbableObjectType Type = GetGrabbedObjectType();
	if (Type != Lightweight)
	{
		return;
	}

	UPrimitiveComponent* GrabbedComponent = OwnerCharacter->PhysicsHandle->GetGrabbedComponent();
	const FVector ImpulseDirection = OwnerCharacter->Camera->GetForwardVector();

	const float Mass = FMath::Max(1.0f, GrabbedComponent->GetMass());
	const float FinalStrength = FMath::Clamp(ThrowStrength / Mass, 500.0f, ThrowStrength);

	ToggleGrab(false);
	GrabbedComponent->AddImpulse(ImpulseDirection * FinalStrength, NAME_None, true);
	UE_LOG(LogPlayer, Log, TEXT("Player threw an %s item"), *GrabbedComponent->GetName())
}

void UGrabbingComponent::ChangeGrabDistance(const float Delta)
{
	if (FMath::IsNearlyZero(Delta))
	{
		return;
	}

	const EGrabbableObjectType Type = GetGrabbedObjectType();

	if (Type == Lightweight)
	{
		GrabDistance = FMath::Clamp(GrabDistance + Delta * 5.0f, MinGrabDistance, MaxGrabDistance);
		return;
	}

	UPrimitiveComponent* Target = Type == Static ? StaticObject : HeavyObject;
	if (!Target)
	{
		return;
	}

	if (Target->GetClass()->ImplementsInterface(UGrabbable::StaticClass()))
	{
		IGrabbable::Execute_OnMouseScrollInput(Target, Delta);
		return;
	}

	if (AActor* TargetOwner = Target->GetOwner(); TargetOwner && TargetOwner->GetClass()->ImplementsInterface(
		UGrabbable::StaticClass()))
	{
		IGrabbable::Execute_OnMouseScrollInput(TargetOwner, Delta);
	}
}

void UGrabbingComponent::RotateLightweightObject(const float AxisX, const float AxisY)
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
