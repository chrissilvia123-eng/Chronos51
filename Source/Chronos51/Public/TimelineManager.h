// Copyright Chronos51 Studios 2026

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TimelineManager.generated.h"

class AVistaNode;
class ULevelStreaming;
class ULevelSequence;

UENUM(BlueprintType)
enum class ETimelineState : uint8
{
    Present,
    Past,
    Transitioning
};

UENUM(BlueprintType)
enum class EZoneChangeType : uint8
{
    WaterLevel,
    BridgeExtended,
    RadiationCleared,
    DoorUnlocked,
    MachineRepaired,
    PathOpened,
    LightingChanged
};

USTRUCT(BlueprintType)
struct FZoneStateChange
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ChangeID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EZoneChangeType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TargetValue = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<AActor> TargetActor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TransitionTime = 3.0f;
};

USTRUCT(BlueprintType)
struct FZoneState
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ZoneID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float WaterLevel = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bBridgeExtended = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bRadiationCleared = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FName, bool> DoorStates;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FName, bool> MachineStates;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FZoneStateChange> PendingChanges;

    UFUNCTION(BlueprintCallable)
    void ApplyChange(const FZoneStateChange& Change);

    UFUNCTION(BlueprintCallable)
    void RevertChange(const FZoneStateChange& Change);
};

USTRUCT(BlueprintType)
struct FVistaNodeData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName NodeID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ZoneID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<ULevelSequence> PhaseShiftSequence;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FName> ConnectedNodes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FName> UnlocksPuzzles;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName RequiredOrgan = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PhaseShiftDuration = 3.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bInstantCutAccessible = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bReducedMotionAccessible = true;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPhaseShiftBegin, AVistaNode*, TargetNode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPhaseShiftComplete, AVistaNode*, TargetNode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTimelineChanged, ETimelineState, OldState, ETimelineState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnZoneStateChanged, FName, ZoneID);

UCLASS()
class CHRONOS51_API UTimelineManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // Current timeline state
    UPROPERTY(BlueprintReadOnly, Category = "Chronos51|Timeline")
    ETimelineState CurrentTimeline = ETimelineState::Present;

    UPROPERTY(BlueprintReadOnly, Category = "Chronos51|Timeline")
    FName CurrentZoneID;

    UPROPERTY(BlueprintReadOnly, Category = "Chronos51|Timeline")
    AVistaNode* CurrentVistaNode = nullptr;

    // Persistent state
    UPROPERTY(BlueprintReadOnly, Category = "Chronos51|State")
    TMap<FName, FZoneState> ZoneStates;

    UPROPERTY(BlueprintReadOnly, Category = "Chronos51|State")
    TArray<FName> ActivatedVistaNodes;

    UPROPERTY(BlueprintReadOnly, Category = "Chronos51|State")
    TArray<FName> FastTravelUnlocked;

    // Vista Node registry
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chronos51|Configuration")
    TMap<FName, FVistaNodeData> VistaNodeRegistry;

    // Phase Shift
    UFUNCTION(BlueprintCallable, Category = "Chronos51|PhaseShift")
    void RequestPhaseShift(AVistaNode* TargetNode);

    UFUNCTION(BlueprintCallable, Category = "Chronos51|PhaseShift")
    void RequestReturnToPresent();

    UFUNCTION(BlueprintCallable, Category = "Chronos51|PhaseShift")
    bool CanPhaseShift(AVistaNode* Node) const;

    // Zone state management
    UFUNCTION(BlueprintCallable, Category = "Chronos51|ZoneState")
    void RecordZoneChange(FName ZoneID, const FZoneStateChange& Change);

    UFUNCTION(BlueprintCallable, Category = "Chronos51|ZoneState")
    void ApplyZoneState(FName ZoneID);

    UFUNCTION(BlueprintCallable, Category = "Chronos51|ZoneState")
    FZoneState GetZoneState(FName ZoneID) const;

    // Fast travel
    UFUNCTION(BlueprintCallable, Category = "Chronos51|FastTravel")
    void UnlockFastTravel(FName ZoneID);

    UFUNCTION(BlueprintCallable, Category = "Chronos51|FastTravel")
    void RequestFastTravel(FName ZoneID);

    // Save/Load integration
    UFUNCTION(BlueprintCallable, Category = "Chronos51|Save")
    void CaptureStateForSave();

    UFUNCTION(BlueprintCallable, Category = "Chronos51|Save")
    void RestoreStateFromSave();

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Chronos51|Events")
    FOnPhaseShiftBegin OnPhaseShiftBegin;

    UPROPERTY(BlueprintAssignable, Category = "Chronos51|Events")
    FOnPhaseShiftComplete OnPhaseShiftComplete;

    UPROPERTY(BlueprintAssignable, Category = "Chronos51|Events")
    FOnTimelineChanged OnTimelineChanged;

    UPROPERTY(BlueprintAssignable, Category = "Chronos51|Events")
    FOnZoneStateChanged OnZoneStateChanged;

protected:
    // Past level streaming
    UPROPERTY()
    TMap<FName, TObjectPtr<ULevelStreaming>> PastLevelStreaming;

    // Transition state
    bool bIsTransitioning = false;
    FName PendingPastLevelName;
    TSoftObjectPtr<ULevelSequence> PendingSequence;

    // Callbacks
    UFUNCTION()
    void OnPastLevelLoaded();

    UFUNCTION()
    void OnPhaseShiftSequenceFinished();

    // Helper
    FName GetPastLevelName(FName ZoneID) const;
};