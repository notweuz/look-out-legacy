// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/BaseStaticObject.h"

#include "Components/PrimitiveComponent.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/MemoryWriter.h"

namespace BaseStaticObjectPrivate
{
	void SerializeSaveData(FArchive& Archive, FActorSaveData& SaveData)
	{
		Archive << SaveData.PersistentActorId;
		Archive << SaveData.Transform;
		Archive << SaveData.bHiddenInGame;
		Archive << SaveData.bCollisionEnabled;
	}
}

ABaseStaticObject::ABaseStaticObject()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABaseStaticObject::BeginPlay()
{
	Super::BeginPlay();
	EnsurePersistentActorId();
}

void ABaseStaticObject::EnsurePersistentActorId()
{
	if (bGeneratePersistentActorIdOnBeginPlay && !PersistentActorId.IsValid())
	{
		PersistentActorId = FGuid::NewGuid();
	}
}

EGrabbableObjectType ABaseStaticObject::GetGrabbableType_Implementation()
{
	return Static;
}

FText ABaseStaticObject::GetGrabWidgetText_Implementation()
{
	return FText::FromString(TEXT("LMB - Hold"));
}

void ABaseStaticObject::OnSave_Implementation(TArray<uint8>& OutData)
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
	BaseStaticObjectPrivate::SerializeSaveData(Writer, SaveData);
}

void ABaseStaticObject::OnLoad_Implementation(const TArray<uint8>& InData)
{
	if (InData.Num() == 0)
	{
		return;
	}

	FActorSaveData SaveData;
	FMemoryReader Reader(InData, true);
	BaseStaticObjectPrivate::SerializeSaveData(Reader, SaveData);

	PersistentActorId = SaveData.PersistentActorId;
	SetActorTransform(SaveData.Transform);
	SetActorHiddenInGame(SaveData.bHiddenInGame);

	if (UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(GetRootComponent()))
	{
		Primitive->SetCollisionEnabled(
			SaveData.bCollisionEnabled ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
	}
}

FString ABaseStaticObject::GetSaveID_Implementation() const
{
	return PersistentActorId.IsValid() ? PersistentActorId.ToString(EGuidFormats::DigitsWithHyphens) : FString();
}
