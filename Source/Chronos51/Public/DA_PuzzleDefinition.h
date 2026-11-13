// Copyright Chronos51 Studios 2026

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PuzzleManager.h"
#include "DA_PuzzleDefinition.generated.h"

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

UCLASS(BlueprintType)
class CHRONOS51_API UDA_PuzzleDefinition : public UDataAsset
{
    GENERATED_BODY()

public:
    UDA_PuzzleDefinition()
    {
        PuzzleID = NAME_None;
        ZoneID = NAME_None;
        Type = EPuzzleType::PowerRouting;
        OrganUnlock = NAME_None;
        EstimatedSolveTimeMinutes = 5.0f;
    }

    // Identity
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Identity")
    FName PuzzleID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Identity")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Identity")
    EPuzzleType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Identity")
    FName ZoneID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Identity")
    TArray<FName> RequiredVistaNodes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Identity")
    TArray<FName> PrerequisitePuzzles;

    // Steps
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steps")
    TArray<FPuzzleStepData> Steps;

    // Rewards
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rewards")
    FName OrganUnlock;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rewards")
    TArray<FZoneStateChange> PresentChanges;

    // Hints
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hints")
    TArray<FPuzzleHint> HintTiers;

    // Meta
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Meta")
    float EstimatedSolveTimeMinutes;

    // Events
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPuzzleStarted, FName, PuzzleID);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPuzzleSolved, FName, PuzzleID);

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnPuzzleStarted OnPuzzleStarted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnPuzzleSolved OnPuzzleSolved;
};