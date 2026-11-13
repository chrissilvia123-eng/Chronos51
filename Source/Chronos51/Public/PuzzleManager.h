// Copyright Chronos51 Studios 2026

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PuzzleManager.generated.h"

class UDA_PuzzleDefinition;

UENUM(BlueprintType)
enum class EPuzzleType : uint8
{
    PowerRouting,
    SequenceAlignment,
    TemporalSync,
    WaveformMatching,
    ArchiveDecryption
};

UENUM(BlueprintType)
enum class EPuzzleState : uint8
{
    Inactive,
    Started,
    InProgress,
    Solved,
    Failed
};

USTRUCT(BlueprintType)
struct FPuzzleStepData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName StepID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ClueSourceID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FName, FString> SolutionData;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TEnumAsByte<EAstraCommand> RequiredAstraCommand = EAstraCommand::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName RewardFragmentID;
};

UENUM(BlueprintType)
enum class EAstraCommand : uint8
{
    None,
    Scan,
    Wait,
    Follow,
    Recall,
    Hold
};

USTRUCT(BlueprintType)
struct FPuzzleHint
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Cost = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText HintText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<TSoftObjectPtr<AActor>> HighlightActors;
};

USTRUCT(BlueprintType)
struct FZoneStateChange
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ChangeID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TEnumAsByte<EZoneChangeType> Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TargetValue = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<AActor> TargetActor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TransitionTime = 3.0f;
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
struct FPuzzleInstance
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FName PuzzleID;

    UPROPERTY(BlueprintReadOnly)
    EPuzzleState State = EPuzzleState::Inactive;

    UPROPERTY(BlueprintReadOnly)
    int32 CurrentStepIndex = 0;

    UPROPERTY(BlueprintReadOnly)
    TMap<FName, FString> StepData;

    UPROPERTY(BlueprintReadOnly)
    float StartTime = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    float SolveTime = 0.0f;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CHRONOS51_API UPuzzleManager : public UActorComponent
{
    GENERATED_BODY()

public:
    UPuzzleManager();

    virtual void BeginPlay() override;

    // Puzzle management
    UFUNCTION(BlueprintCallable, Category = "Chronos51|Puzzles")
    bool StartPuzzle(FName PuzzleID);

    UFUNCTION(BlueprintCallable, Category = "Chronos51|Puzzles")
    void ProcessPuzzleAction(FName PuzzleID, FName StepID, const TMap<FName, FString>& ActionData);

    UFUNCTION(BlueprintCallable, Category = "Chronos51|Puzzles")
    bool CheckPuzzleSolved(FName PuzzleID) const;

    UFUNCTION(BlueprintCallable, Category = "Chronos51|Puzzles")
    FPuzzleInstance GetPuzzleInstance(FName PuzzleID) const;

    UFUNCTION(BlueprintCallable, Category = "Chronos51|Puzzles")
    TArray<FPuzzleInstance> GetActivePuzzles() const;

    // Hint system
    UFUNCTION(BlueprintCallable, Category = "Chronos51|Puzzles")
    FText GetHint(FName PuzzleID, int32 Tier) const;

    UFUNCTION(BlueprintCallable, Category = "Chronos51|Puzzles")
    bool PurchaseHint(FName PuzzleID, int32 Tier);

    // Definition access
    UFUNCTION(BlueprintCallable, Category = "Chronos51|Puzzles")
    UDA_PuzzleDefinition* GetPuzzleDefinition(FName PuzzleID) const;

    // Events
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPuzzleStepComplete, FName, PuzzleID, FName, StepID);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPuzzleSolved, FName, PuzzleID, FName, OrganUnlock);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPuzzleFailed, FName, PuzzleID);

    UPROPERTY(BlueprintAssignable, Category = "Chronos51|Puzzles")
    FOnPuzzleStepComplete OnPuzzleStepComplete;

    UPROPERTY(BlueprintAssignable, Category = "Chronos51|Puzzles")
    FOnPuzzleSolved OnPuzzleSolved;

    UPROPERTY(BlueprintAssignable, Category = "Chronos51|Puzzles")
    FOnPuzzleFailed OnPuzzleFailed;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chronos51|Puzzles")
    TArray<TObjectPtr<UDA_PuzzleDefinition>> PuzzleDefinitions;

    UPROPERTY()
    TMap<FName, TObjectPtr<UDA_PuzzleDefinition>> PuzzleDefMap;

    UPROPERTY()
    TMap<FName, FPuzzleInstance> ActivePuzzles;

    UPROPERTY()
    TMap<FName, TArray<int32>> PurchasedHints; // PuzzleID -> Tier indices

    void InitializePuzzleDefMap();
    bool EvaluateStepSolution(FName PuzzleID, FName StepID, const TMap<FName, FString>& ActionData) const;
    void ApplyPresentChanges(FName PuzzleID);
};