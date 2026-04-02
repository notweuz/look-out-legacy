// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Data/Save/SaveSlotMetadata.h"
#include "LookOutSaveSubsystem.generated.h"

class AActor;
class UActorComponent;
class ULookOutGlobalSaveGame;
class ULookOutSlotSaveGame;
struct FSavedActorData;

UCLASS()
class LOOK_OUT_API ULookOutSaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, Category="Save System")
	FString CreateSaveSlot(const FString& DisplayName);

	UFUNCTION(BlueprintCallable, Category="Save System")
	bool SaveSlot(const FString& SlotId);

	UFUNCTION(BlueprintCallable, Category="Save System")
	bool SaveCurrentSlot();

	UFUNCTION(BlueprintCallable, Category="Save System")
	bool LoadSlot(const FString& SlotId);

	UFUNCTION(BlueprintCallable, Category="Save System")
	bool DeleteSlot(const FString& SlotId);

	UFUNCTION(BlueprintCallable, Category="Save System")
	void SetCurrentSlotId(const FString& SlotId);

	UFUNCTION(BlueprintPure, Category="Save System")
	FString GetCurrentSlotId() const;

	UFUNCTION(BlueprintPure, Category="Save System")
	TArray<FSaveSlotMetadata> GetAllSaveSlots() const;

	UFUNCTION(BlueprintCallable, Category="Save System")
	ULookOutGlobalSaveGame* GetGlobalSave();

	UFUNCTION(BlueprintCallable, Category="Save System")
	bool SaveGlobalSave();

	UFUNCTION(BlueprintCallable, Category="Save System|Global Data")
	void SetGlobalInt(FName Key, int32 Value);

	UFUNCTION(BlueprintCallable, Category="Save System|Global Data")
	int32 GetGlobalInt(FName Key, int32 DefaultValue = 0) const;

	UFUNCTION(BlueprintCallable, Category="Save System|Global Data")
	void SetGlobalFloat(FName Key, float Value);

	UFUNCTION(BlueprintCallable, Category="Save System|Global Data")
	float GetGlobalFloat(FName Key, float DefaultValue = 0.0f) const;

	UFUNCTION(BlueprintCallable, Category="Save System|Global Data")
	void SetGlobalBool(FName Key, bool Value);

	UFUNCTION(BlueprintCallable, Category="Save System|Global Data")
	bool GetGlobalBool(FName Key, bool DefaultValue = false) const;

	UFUNCTION(BlueprintCallable, Category="Save System|Global Data")
	void SetGlobalString(FName Key, const FString& Value);

	UFUNCTION(BlueprintCallable, Category="Save System|Global Data")
	FString GetGlobalString(FName Key, const FString& DefaultValue = "") const;

	UFUNCTION(BlueprintCallable, Category="Save System|Global Data")
	void SetGlobalName(FName Key, FName Value);

	UFUNCTION(BlueprintCallable, Category="Save System|Global Data")
	FName GetGlobalName(FName Key, FName DefaultValue = NAME_None) const;

protected:
	UPROPERTY()
	ULookOutGlobalSaveGame* CachedGlobalSave = nullptr;

	UPROPERTY()
	FString CurrentSlotId;

private:
	static FString BuildSlotStorageName(const FString& SlotId);
	static FString SanitizeDisplayName(const FString& DisplayName);
	static bool CanSerializeComponent(const UActorComponent* Component);

	bool CaptureWorldState(ULookOutSlotSaveGame& SlotSave) const;
	bool ApplyWorldState(const ULookOutSlotSaveGame& SlotSave) const;
	bool SnapshotActor(AActor* Actor, FSavedActorData& OutActorData) const;
	bool RestoreActor(AActor* Actor, const FSavedActorData& SavedActorData) const;
	AActor* SpawnActorFromSaveData(const FSavedActorData& SavedActorData) const;
	FString ResolveActorId(const AActor* Actor) const;
	FString GetCurrentMapName() const;

	ULookOutSlotSaveGame* LoadSlotObject(const FString& SlotId) const;
	ULookOutGlobalSaveGame* LoadOrCreateGlobalSave();
	FSaveSlotMetadata* FindSlotMetadata(const FString& SlotId);
	const FSaveSlotMetadata* FindSlotMetadata(const FString& SlotId) const;
};
