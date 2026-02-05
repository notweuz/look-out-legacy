#include "Characters/Components/GrabbingComponent.h"

#include "Camera/CameraComponent.h"
#include "Characters/BaseCharacter.h"
#include "Characters/Components/MovementComponentExtended.h"
#include "Components/CapsuleComponent.h"
#include "Interfaces/Grabbable.h"
#include "Objects/BaseStaticObject.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

UGrabbingComponent::UGrabbingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
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

void UGrabbingComponent::LightweightObjectRotation(const float InputAxisX, const float InputAxisY)
{
	if (!IsGrabbingObject) return;

	constexpr float RotationSpeed = 1.5f;

	GrabRotation.Yaw += InputAxisX * RotationSpeed;
	GrabRotation.Pitch += InputAxisY * RotationSpeed;

	GrabRotation.Normalize();
}

EGrabbableObjectType UGrabbingComponent::GrabbedObjectType()
{
	EGrabbableObjectType Result = None;
	const UPrimitiveComponent* GrabbedComponent = nullptr;

	if (OwnerCharacter->PhysicsHandle && OwnerCharacter->PhysicsHandle->GetGrabbedComponent())
	{
		GrabbedComponent = OwnerCharacter->PhysicsHandle->GetGrabbedComponent();
	}

	if (StaticObject != nullptr) Result = Static;
	if (HeavyObject != nullptr) Result = Heavyweight;
	if (GrabbedComponent) Result = Lightweight;

	return Result;
}

void UGrabbingComponent::ToggleGrabComponent(const bool State)
{
	if (State)
	{
		GrabDistance = MaxGrabDistance;
		const auto [StartVector, EndVector] = OwnerCharacter->GetForwardVectorRelatedToCamera(GrabDistance);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(OwnerCharacter);

		if (FHitResult HitResult; GetWorld()->LineTraceSingleByChannel(HitResult, StartVector, EndVector,
		                                                               ECC_Visibility, QueryParams))
		{
			AActor* HitActor = HitResult.GetActor();
			UPrimitiveComponent* HitActorComponent = HitResult.GetComponent();

			if (HitActor && HitActor->GetClass()->ImplementsInterface(UGrabbable::StaticClass()))
			{
				if (IGrabbable* GrabbableInterface = Cast<IGrabbable>(HitActor))
				{
					if (const EGrabbableObjectType Type = GrabbableInterface->GetGrabbableType(); Type == Lightweight)
					{
						GrabRotation = FRotator::ZeroRotator;

						OwnerCharacter->PhysicsHandle->GrabComponentAtLocationWithRotation(
							HitActorComponent,
							NAME_None,
							HitActorComponent->GetComponentLocation(),
							HitActorComponent->GetComponentRotation()
						);

						HitActorComponent->SetEnableGravity(false);
						HitActorComponent->WakeAllRigidBodies();

						IsGrabbingObject = true;
					}
					else if (Type == Heavyweight)
					{
						HeavyObject = HitActorComponent;
						OwnerCharacter->PhysicsConstraint->SetConstrainedComponents(
							HitActorComponent, NAME_None,
							OwnerCharacter->GetCapsuleComponent(), NAME_None
						);
						OwnerCharacter->MovementComponentExtended->CanSprint = false;
						OwnerCharacter->MovementComponentExtended->ToggleSprint(false);
						OwnerCharacter->MovementComponentExtended->ChangeWalkSpeed(
							OwnerCharacter->MovementComponentExtended->DragSpeed);
						IsGrabbingObject = true;
					}
					else if (Type == Static)
					{
						StaticObject = HitActorComponent;
						IsGrabbingObject = true;
					}
				}
			}
		}
	}
	else
	{
		if (const EGrabbableObjectType ObjectType = GrabbedObjectType(); ObjectType == Lightweight)
		{
			UPrimitiveComponent* GrabbedComponent = OwnerCharacter->PhysicsHandle->GetGrabbedComponent();
			OwnerCharacter->PhysicsHandle->ReleaseComponent();
			if (GrabbedComponent)
			{
				GrabbedComponent->SetEnableGravity(true);
				GrabbedComponent->WakeAllRigidBodies();
			}
		}
		else if (ObjectType == Heavyweight)
		{
			HeavyObject = nullptr;
			OwnerCharacter->PhysicsConstraint->BreakConstraint();
			OwnerCharacter->MovementComponentExtended->CanSprint = true;
			OwnerCharacter->MovementComponentExtended->ChangeWalkSpeed(
				OwnerCharacter->MovementComponentExtended->WalkSpeed);
		}
		else if (ObjectType == Static)
		{
			StaticObject = nullptr;
		}
		IsGrabbingObject = false;
	}
}

void UGrabbingComponent::ProcessGrabbing(const float DeltaSeconds)
{
	if (const EGrabbableObjectType ObjectType = GrabbedObjectType(); ObjectType == Lightweight)
	{
		auto [StartVector, EndVector] = OwnerCharacter->GetForwardVectorRelatedToCamera(GrabDistance);

		OwnerCharacter->PhysicsHandle->SetTargetLocation(EndVector);

		if (UPrimitiveComponent* GrabbedComponent = OwnerCharacter->PhysicsHandle->GetGrabbedComponent())
		{
			const FRotator CameraRot = OwnerCharacter->Camera->GetComponentRotation();
			const FRotator BaseYaw(0.0f, CameraRot.Yaw, 0.0f);

			const FQuat TargetQuat = FQuat(BaseYaw) * FQuat(GrabRotation);

			const FRotator CurrentRot = GrabbedComponent->GetComponentRotation();
			const FRotator SmoothTargetRot = FMath::RInterpTo(CurrentRot, TargetQuat.Rotator(), DeltaSeconds, 50.0f);

			OwnerCharacter->PhysicsHandle->SetTargetRotation(SmoothTargetRot);

			GrabbedComponent->WakeAllRigidBodies();
		}
	}
	else if (ObjectType == Heavyweight)
	{
		UPrimitiveComponent* Component1 = nullptr;
		FName BoneName1;
		UPrimitiveComponent* Component2 = nullptr;
		FName BoneName2;

		OwnerCharacter->PhysicsConstraint->GetConstrainedComponents(Component1, BoneName1, Component2, BoneName2);

		if (Component1 && Component2)
		{
			if (const float Distance = FVector::Dist(Component1->GetComponentLocation(),
			                                         Component2->GetComponentLocation()); Distance >= GrabDistance *
				1.5f)
			{
				FVector Diff = Component1->GetComponentLocation() - Component2->GetComponentLocation();
				Diff.Normalize();
				OwnerCharacter->LaunchCharacter(Distance * Diff * Component1->GetComponentScale().X * 0.1f, false,
				                                false);
			}
		}
	}
	else if (ObjectType == Static)
	{
		if (const UCapsuleComponent* PlayerCapsule = OwnerCharacter->GetCapsuleComponent(); StaticObject &&
			PlayerCapsule)
		{
			if (const float Distance = FVector::Dist(StaticObject->GetComponentLocation(),
			                                         PlayerCapsule->GetComponentLocation()); Distance >= GrabDistance *
				1.5f)
			{
				FVector Diff = StaticObject->GetComponentLocation() - PlayerCapsule->GetComponentLocation();
				Diff.Normalize();
				OwnerCharacter->LaunchCharacter(Distance * Diff * StaticObject->GetComponentScale().X * 0.1f, false,
				                                false);
			}
		}
	}
}

void UGrabbingComponent::ThrowObject()
{
	if (const EGrabbableObjectType ObjectType = GrabbedObjectType(); ObjectType == Lightweight)
	{
		UPrimitiveComponent* GrabbedComponent = OwnerCharacter->PhysicsHandle->GetGrabbedComponent();
		const FVector LaunchImpulse = OwnerCharacter->Camera->GetForwardVector();

		const float Mass = FMath::Max(1.0f, GrabbedComponent->GetMass());
		const float FinalStrength = FMath::Clamp(Strength / Mass, 500.0f, Strength);

		ToggleGrabComponent(false);
		GrabbedComponent->AddImpulse(LaunchImpulse * FinalStrength, NAME_None, true);
	}
}

void UGrabbingComponent::ChangeDistance(const float DeltaVector)
{
	if (FMath::IsNearlyZero(DeltaVector)) return;

	const EGrabbableObjectType Type = GrabbedObjectType();

	UPrimitiveComponent* TargetComponent = nullptr;

	if (Type == Lightweight)
	{
		GrabDistance = FMath::Clamp(GrabDistance + DeltaVector * 5.0f, MinGrabDistance, MaxGrabDistance);
		return;
	}
	
	if (Type == Static)
	{
		TargetComponent = StaticObject;
	}
	else if (Type == Heavyweight)
	{
		TargetComponent = HeavyObject;
	}

	if (!TargetComponent) return;

	if (TargetComponent->GetClass()->ImplementsInterface(UGrabbable::StaticClass()))
	{
		IGrabbable::Execute_OnMouseScrollInput(TargetComponent, DeltaVector);
		return;
	}

	if (AActor* Owner = TargetComponent->GetOwner())
	{
		if (Owner->Implements<UGrabbable>())
		{
			IGrabbable::Execute_OnMouseScrollInput(Owner, DeltaVector);
		}
	}
}