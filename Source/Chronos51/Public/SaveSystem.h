// Copyright Chronos51 Studios 2026

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveSystem.generated.h"

UENUM(BlueprintType)
enum class ETimelineState : uint8
{
    Present,
    Past,
    Transitioning
};

USTRUCT(BlueprintType)
struct FPuzzleSaveState
{
    GENERATED_BODY()

    UPROPERTY()
    int32 CurrentStep = 0;

    UPROPERTY()
    TMap<FName, FString> StepData;

    UPROPERTY()
    bool bSolved = false;

    UPROPERTY()
    float SolveTimestamp = 0.0f;
};

USTRUCT(BlueprintType)
struct FZoneStateSaveData
{
    GENERATED_BODY()

    UPROPERTY()
    FName ZoneID;

    UPROPERTY()
    float WaterLevel = 1.0f;

    UPROPERTY()
    bool bBridgeExtended = false;

    UPROPERTY()
    bool bRadiationCleared = false;

    UPROPERTY()
    TMap<FName, bool> DoorStates;

    UPROPERTY()
    TMap<FName, bool> MachineStates;
};

USTRUCT(BlueprintType)
struct FChronosSaveData
{
    GENERATED_BODY()

    UPROPERTY()
    int32 Version = 5;

    UPROPERTY()
    int32 Slot = 0;

    UPROPERTY()
    FDateTime Timestamp;

    UPROPERTY()
    float PlaytimeSeconds = 0.0f;

    UPROPERTY()
    ETimelineState CurrentTimeline = ETimelineState::Present;

    UPROPERTY()
    FName CurrentZone;

    UPROPERTY()
    FTransform PlayerTransform;

    UPROPERTY()
    TArray<FName> ActivatedVistaNodes;

    UPROPERTY()
    TArray<FName> FastTravelUnlocked;

    UPROPERTY()
    TArray<FName> OrgansRestored;

    UPROPERTY()
    TArray<FName> ActiveAbilities;

    UPROPERTY()
    TMap<FName, FPuzzleSaveState> PuzzleStates;

    UPROPERTY()
    TMap<FName, FZoneStateSaveData> ZoneStates;

    UPROPERTY()
    int32 AstraTrust = 0;

    UPROPERTY()
    TArray<int32> AstraMemoryFragments;

    UPROPERTY()
    int32 ResonanceCurrency = 0;

    UPROPERTY()
    TMap<FName, FString> GameSettings;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveComplete, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoadComplete, bool, bSuccess);

UCLASS()
class CHRONOS51_API USaveSystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    // Save/Load
    UFUNCTION(BlueprintCallable, Category = "Chronos51|Save")
    void SaveGame(int32 SlotIndex);

    UFUNCTION(BlueprintCallable, Category = "Chronos51|Save")
    void LoadGame(int32 SlotIndex);

    UFUNCTION(BlueprintCallable, Category = "Chronos51|Save")
    void RequestAutosave();

    UFUNCTION(BlueprintCallable, Category = "Chronos51|Save")
    bool DoesSaveExist(int32 SlotIndex) const;

    UFUNCTION(BlueprintCallable, Category = "Chronos51|Save")
    TArray<int32> GetAvailableSlots() const;

    // State capture/restore
    UFUNCTION(BlueprintCallable, Category = "Chronos51|Save")
    FChronosSaveData CaptureCurrentState();

    UFUNCTION(BlueprintCallable, Category = "Chronos51|Save")
    void RestoreState(const FChronosSaveData& SaveData);

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Chronos51|Save")
    FOnSaveComplete OnSaveComplete;

    UPROPERTY(BlueprintAssignable, Category = "Chronos51|Save")
    FOnLoadComplete OnLoadComplete;

    // Settings
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chronos51|Save")
    int32 MaxSaveSlots = 5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chronos51|Save")
    float AutosaveInterval = 60.0f; // seconds

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chronos51|Save")
    float PlaytimeSeconds = 0.0f;

protected:
    // Internal
    FString GetSaveDirectory() const;
    FString GetSaveFilePath(int32 SlotIndex) const;
    FString GetTempFilePath(int32 SlotIndex) const;
    FString GetBackupFilePath(int32 SlotIndex) const;

    bool WriteSaveFile(int32 SlotIndex, const FChronosSaveData& Data);
    bool ReadSaveFile(int32 SlotIndex, FChronosSaveData& OutData);

    FChronosSaveData MigrateSaveData(FChronosSaveData Data);
    FChronosSaveData Migrate_v1_to_v2(FChronosSaveData Data);
    FChronosSaveData Migrate_v2_to_v3(FChronosSaveData Data);
    FChronosSaveData Migrate_v3_to_v4(FChronosSaveData Data);
    FChronosSaveData Migrate_v4_to_v5(FChronosSaveData Data);

    void ValidateSaveData(FChronosSaveData& Data);

    // Timer
    FTimerHandle AutosaveTimerHandle;
};