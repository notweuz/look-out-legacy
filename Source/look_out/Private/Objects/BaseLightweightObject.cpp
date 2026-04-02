// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/BaseLightweightObject.h"

#include "Components/PrimitiveComponent.h"
#include "Libraries/SaveSystemUtils.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/MemoryWriter.h"

namespace BaseLightweightObjectPrivate
{
	void SerializeSaveData(FArchive& Archive, FActorSaveData& SaveData)
	{
		Archive << SaveData.PersistentActorId;
		Archive << SaveData.Transform;
		Archive << SaveData.bHiddenInGame;
		Archive << SaveData.bCollisionEnabled;
	}
}

ABaseLightweightObject::ABaseLightweightObject()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABaseLightweightObject::BeginPlay()
{
	Super::BeginPlay();
	EnsurePersistentActorId();
}

void ABaseLightweightObject::EnsurePersistentActorId()
{
	if (bGeneratePersistentActorIdOnBeginPlay && !PersistentActorId.IsValid() && !FSaveSystemUtils::IsLevelPlacedActor(this))
	{
		PersistentActorId = FGuid::NewGuid();
	}
}

EGrabbableObjectType ABaseLightweightObject::GetGrabbableType_Implementation()
{
	return Lightweight;
}

FText ABaseLightweightObject::GetGrabWidgetText_Implementation()
{
	return FText::FromString(TEXT("LMB - Grab"));
}

void ABaseLightweightObject::OnSave_Implementation(TArray<uint8>& OutData)
{
	EnsurePersistentActorId();

	FActorSaveData SaveData;
	SaveData.PersistentActorId = PersistentActorId;
	SaveData.Transform = GetActorTransform();
	SaveData.bHiddenInGame = IsHidden();

	if (const UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(GetRootComponent()))
	{
		SaveData.bCollisionEnabled = Primitive->GetCollisionEnabled() != ECollisionEnabled::NoCollision;
	}

	FMemoryWriter Writer(OutData, true);
	BaseLightweightObjectPrivate::SerializeSaveData(Writer, SaveData);
}

void ABaseLightweightObject::OnLoad_Implementation(const TArray<uint8>& InData)
{
	if (InData.Num() == 0)
	{
		return;
	}

	FActorSaveData SaveData;
	FMemoryReader Reader(InData, true);
	BaseLightweightObjectPrivate::SerializeSaveData(Reader, SaveData);

	PersistentActorId = SaveData.PersistentActorId;
	SetActorTransform(SaveData.Transform);
	SetActorHiddenInGame(SaveData.bHiddenInGame);

	if (UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(GetRootComponent()))
	{
		Primitive->SetCollisionEnabled(
			SaveData.bCollisionEnabled ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
	}
}

FString ABaseLightweightObject::GetSaveID_Implementation() const
{
	if (FSaveSystemUtils::IsLevelPlacedActor(this))
	{
		return FSaveSystemUtils::BuildStableLevelActorId(this);
	}

	return PersistentActorId.IsValid() ? PersistentActorId.ToString(EGuidFormats::DigitsWithHyphens) : FString();
}
