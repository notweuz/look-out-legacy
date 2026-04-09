// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Data/ItemDefinition.h"
#include "Core/Save/SaveTypes.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Grabbable.h"
#include "Interfaces/Saveable.h"
#include "BaseObject.generated.h"

UCLASS(Blueprintable, BlueprintType)
class LOOK_OUT_API ABaseObject : public AActor, public ISaveable, public IGrabbable
{
	GENERATED_BODY()

public:
	ABaseObject();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(SaveGame)
	FGuid SaveId = FGuid::NewGuid();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Save")
	EActorPersistence Persistence = EActorPersistence::Placed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Type")
	EObjectType ObjectType = EObjectType::Lightweight;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Inventory Definition") UItemDefinition* Definition;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Inventory Definition|Manual") float Weight;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Inventory Definition|Manual") TObjectPtr<UTexture2D> Icon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Inventory Definition|Manual") FText DisplayName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Inventory Definition|Manual") FText Description;
	
	virtual FGuid GetSaveId_Implementation() override { return SaveId; }
	virtual void OnSave_Implementation(TArray<uint8>& OutBytes) override;
	virtual void OnLoad_Implementation(const TArray<uint8>& InBytes) override;

	virtual EObjectType GetGrabbableType_Implementation() override;
	virtual FText GetGrabWidgetText_Implementation() override;
};
