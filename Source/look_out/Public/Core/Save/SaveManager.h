#pragma once
#include "CoreMinimal.h"
#include "SaveTypes.h"
#include "Characters/BaseCharacter.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveManager.generated.h"

class ABaseCharacter;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSaveSlotsChanged, const TArray<FSaveSlotMeta>&);

UCLASS()
class LOOK_OUT_API USaveManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    static const FString RegistrySlot;
    static const FString GlobalSlot;

    FOnSaveSlotsChanged OnSaveSlotsChanged;

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category="Save")
    void CreateSave(const FString& SlotName, ABaseCharacter* Player);

    UFUNCTION(BlueprintCallable, Category="Save")
    void OverwriteSave(const FString& SlotId, ABaseCharacter* Player);

    UFUNCTION(BlueprintCallable, Category="Save")
    void LoadSave(const FString& SlotId, ABaseCharacter* Player);

    UFUNCTION(BlueprintCallable, Category="Save")
    void DeleteSave(const FString& SlotId);

    UFUNCTION(BlueprintPure, Category="Save")
    TArray<FSaveSlotMeta> GetAllSlots() const { return Registry->Slots; }

    UFUNCTION(BlueprintCallable, Category="Save")
    void RegisterDestroyed(const FString& SlotId, FGuid SaveId);
    
    UFUNCTION(BlueprintCallable, Category="Save|WorldState")
    void SetFlag(const FString& SlotId, FName Key, bool Value);

    UFUNCTION(BlueprintCallable, Category="Save|WorldState")
    void IncrementCounter(const FString& SlotId, FName Key, int32 Amount = 1);

    UFUNCTION(BlueprintPure, Category="Save|WorldState")
    bool GetFlag(const FString& SlotId, FName Key, bool Default = false);

    UFUNCTION(BlueprintPure, Category="Save|WorldState")
    int32 GetCounter(const FString& SlotId, FName Key);

    UFUNCTION(BlueprintCallable, Category="Save|Global")
    void IncrementGlobalCounter(FName Key, int32 Amount = 1);

    UFUNCTION(BlueprintCallable, Category="Save|Global")
    void SetGlobalFlag(FName Key, bool Value);

    UFUNCTION(BlueprintPure, Category="Save|Global")
    int32 GetGlobalCounter(FName Key);

    UFUNCTION(BlueprintPure, Category="Save|Global")
    bool GetGlobalFlag(FName Key, bool Default = false);

    UFUNCTION(BlueprintPure, Category="Save|Global")
    UGlobalSaveGame* GetGlobalSave() const { return GlobalSave; }

private:
    UPROPERTY()
    TObjectPtr<USaveSlotRegistry> Registry;

    UPROPERTY()
    TObjectPtr<UGlobalSaveGame> GlobalSave;

    FDateTime SessionStartTime;

    void SaveRegistry();
    void LoadRegistry();
    void SaveGlobal();
    void LoadGlobal();

    void CollectWorldData(UGameSaveGame* SaveGame);
    void RestoreWorldData(UGameSaveGame* SaveGame);
    void CollectPlayerData(UGameSaveGame* SaveGame, ABaseCharacter* Player);
    void RestorePlayerData(UGameSaveGame* SaveGame, ABaseCharacter* Player);

    float CalcSessionTime() const;

    static FString GenerateSlotId();
};