// Copyright Chronos51 Studios 2026

#include "PuzzleManager.h"
#include "DA_PuzzleDefinition.h"
#include "TimelineManager.h"
#include "Kismet/GameplayStatics.h"

UPuzzleManager::UPuzzleManager()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UPuzzleManager::BeginPlay()
{
    Super::BeginPlay();
    InitializePuzzleDefMap();
}

void UPuzzleManager::InitializePuzzleDefMap()
{
    PuzzleDefMap.Empty();
    for (UDA_PuzzleDefinition* Def : PuzzleDefinitions)
    {
        if (Def)
        {
            PuzzleDefMap.Add(Def->GetPuzzleID(), Def);
        }
    }
}

bool UPuzzleManager::StartPuzzle(FName PuzzleID)
{
    if (!PuzzleDefMap.Contains(PuzzleID))
    {
        return false;
    }

    if (ActivePuzzles.Contains(PuzzleID))
    {
        FPuzzleInstance& Instance = ActivePuzzles[PuzzleID];
        if (Instance.State == EPuzzleState::Solved)
        {
            return false; // Already solved
        }
    }

    FPuzzleInstance Instance;
    Instance.PuzzleID = PuzzleID;
    Instance.State = EPuzzleState::Started;
    Instance.CurrentStepIndex = 0;
    Instance.StepData.Empty();
    Instance.StartTime = GetWorld()->GetTimeSeconds();
    Instance.SolveTime = 0.0f;

    ActivePuzzles.Add(PuzzleID, Instance);

    // Notify puzzle definition
    UDA_PuzzleDefinition* Def = PuzzleDefMap[PuzzleID];
    if (Def)
    {
        Def->OnPuzzleStarted.Broadcast(PuzzleID);
    }

    return true;
}

void UPuzzleManager::ProcessPuzzleAction(FName PuzzleID, FName StepID, const TMap<FName, FString>& ActionData)
{
    FPuzzleInstance* Instance = ActivePuzzles.Find(PuzzleID);
    if (!Instance || Instance->State == EPuzzleState::Solved)
    {
        return;
    }

    UDA_PuzzleDefinition* Def = PuzzleDefMap.FindRef(PuzzleID);
    if (!Def)
    {
        return;
    }

    // Check if this is the current expected step
    if (Def->Steps.IsValidIndex(Instance->CurrentStepIndex))
    {
        const FPuzzleStepData& CurrentStep = Def->Steps[Instance->CurrentStepIndex];
        if (CurrentStep.StepID == StepID)
        {
            // Evaluate solution
            if (EvaluateStepSolution(PuzzleID, StepID, ActionData))
            {
                // Step completed
                Instance->StepData.Add(StepID, ActionData);
                Instance->CurrentStepIndex++;

                OnPuzzleStepComplete.Broadcast(PuzzleID, StepID);

                // Check if puzzle is fully solved
                if (Instance->CurrentStepIndex >= Def->Steps.Num())
                {
                    Instance->State = EPuzzleState::Solved;
                    Instance->SolveTime = GetWorld()->GetTimeSeconds();

                    // Apply present changes
                    ApplyPresentChanges(PuzzleID);

                    // Grant organ if any
                    FName OrganUnlock = Def->OrganUnlock;
                    if (OrganUnlock != NAME_None)
                    {
                        OnPuzzleSolved.Broadcast(PuzzleID, OrganUnlock);
                    }
                    else
                    {
                        OnPuzzleSolved.Broadcast(PuzzleID, NAME_None);
                    }

                    Def->OnPuzzleSolved.Broadcast(PuzzleID);
                }
            }
            else
            {
                // Incorrect solution - could trigger failure state
                Instance->State = EPuzzleState::Failed;
                OnPuzzleFailed.Broadcast(PuzzleID);
            }
        }
    }
}

bool UPuzzleManager::CheckPuzzleSolved(FName PuzzleID) const
{
    const FPuzzleInstance* Instance = ActivePuzzles.Find(PuzzleID);
    return Instance && Instance->State == EPuzzleState::Solved;
}

FPuzzleInstance UPuzzleManager::GetPuzzleInstance(FName PuzzleID) const
{
    const FPuzzleInstance* Instance = ActivePuzzles.Find(PuzzleID);
    return Instance ? *Instance : FPuzzleInstance();
}

TArray<FPuzzleInstance> UPuzzleManager::GetActivePuzzles() const
{
    TArray<FPuzzleInstance> Result;
    Result.Reserve(ActivePuzzles.Num());
    for (auto& Pair : ActivePuzzles)
    {
        Result.Add(Pair.Value);
    }
    return Result;
}

FText UPuzzleManager::GetHint(FName PuzzleID, int32 Tier) const
{
    UDA_PuzzleDefinition* Def = PuzzleDefMap.FindRef(PuzzleID);
    if (Def && Def->HintTiers.IsValidIndex(Tier))
    {
        return Def->HintTiers[Tier].HintText;
    }
    return FText::GetEmpty();
}

bool UPuzzleManager::PurchaseHint(FName PuzzleID, int32 Tier)
{
    // Check if already purchased
    TArray<int32>& Purchased = PurchasedHints.FindOrAdd(PuzzleID);
    if (Purchased.Contains(Tier))
    {
        return false; // Already purchased
    }

    // Check resonance currency (would need access to player state)
    // For now, just grant
    Purchased.Add(Tier);
    return true;
}

UDA_PuzzleDefinition* UPuzzleManager::GetPuzzleDefinition(FName PuzzleID) const
{
    return PuzzleDefMap.FindRef(PuzzleID);
}

bool UPuzzleManager::EvaluateStepSolution(FName PuzzleID, FName StepID, const TMap<FName, FString>& ActionData) const
{
    UDA_PuzzleDefinition* Def = PuzzleDefMap.FindRef(PuzzleID);
    if (!Def) return false;

    for (const FPuzzleStepData& Step : Def->Steps)
    {
        if (Step.StepID == StepID)
        {
            // Compare ActionData with Step.SolutionData
            for (auto& Pair : Step.SolutionData)
            {
                const FString* ProvidedValue = ActionData.Find(Pair.Key);
                if (!ProvidedValue || *ProvidedValue != Pair.Value)
                {
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}

void UPuzzleManager::ApplyPresentChanges(FName PuzzleID)
{
    UDA_PuzzleDefinition* Def = PuzzleDefMap.FindRef(PuzzleID);
    if (!Def) return;

    // Notify TimelineManager to apply zone state changes
    if (UGameInstance* GI = GetWorld()->GetGameInstance())
    {
        UTimelineManager* TimelineManager = GI->GetSubsystem<UTimelineManager>();
        if (TimelineManager)
        {
            for (const FZoneStateChange& Change : Def->PresentChanges)
            {
                TimelineManager->RecordZoneChange(Def->ZoneID, Change);
            }
        }
    }
}