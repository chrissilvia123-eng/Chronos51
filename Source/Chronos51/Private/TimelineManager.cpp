// Copyright Chronos51 Studios 2026

#include "TimelineManager.h"
#include "VistaNode.h"
#include "Engine/LevelStreaming.h"
#include "Engine/LevelSequence.h"
#include "Engine/LevelSequencePlayer.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void UTimelineManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    // Initialize default zone states
    TArray<FName> ZoneIDs = { 
        FName("EntryHangar"), 
        FName("ReactorCore"), 
        FName("BioContainment"), 
        FName("CommsArray"), 
        FName("Hangar18"), 
        FName("Administration"), 
        FName("SubLevel0") 
    };

    for (FName ZoneID : ZoneIDs)
    {
        FZoneState State;
        State.ZoneID = ZoneID;
        ZoneStates.Add(ZoneID, State);
    }

    CurrentTimeline = ETimelineState::Present;
    bIsTransitioning = false;
}

void UTimelineManager::Deinitialize()
{
    // Unload any past levels
    for (auto& Pair : PastLevelStreaming)
    {
        if (Pair.Value && Pair.Value->IsLevelLoaded())
        {
            FLatentActionInfo LatentInfo;
            UGameplayStatics::UnloadStreamLevel(GetWorld(), Pair.Key, LatentInfo, true);
        }
    }
    PastLevelStreaming.Empty();

    Super::Deinitialize();
}

void UTimelineManager::RequestPhaseShift(AVistaNode* TargetNode)
{
    if (!TargetNode || bIsTransitioning || !CanPhaseShift(TargetNode))
    {
        return;
    }

    bIsTransitioning = true;
    CurrentTimeline = ETimelineState::Transitioning;
    CurrentVistaNode = TargetNode;

    // Broadcast begin event
    OnPhaseShiftBegin.Broadcast(TargetNode);

    // Get phase shift sequence
    FVistaNodeData* NodeData = VistaNodeRegistry.Find(TargetNode->GetNodeID());
    if (NodeData && NodeData->PhaseShiftSequence.IsValid())
    {
        PendingSequence = NodeData->PhaseShiftSequence;
    }

    // Disable player input
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        PC->SetIgnoreMoveInput(true);
        PC->SetIgnoreLookInput(true);
    }

    // Play camera blend sequence
    if (PendingSequence.IsValid())
    {
        ULevelSequence* Sequence = PendingSequence.LoadSynchronous();
        if (Sequence)
        {
            ALevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
                Sequence, FMovieSceneSequencePlaybackSettings(), GetWorld());
            if (SequencePlayer)
            {
                SequencePlayer->Play();
                
                // Bind to sequence finished
                FScriptDelegate Delegate;
                Delegate.BindUFunction(this, "OnPhaseShiftSequenceFinished");
                SequencePlayer->OnFinished.AddUnique(Delegate);
            }
        }
    }

    // Async load past level
    PendingPastLevelName = GetPastLevelName(TargetNode->GetZoneID());
    FLatentActionInfo LatentInfo;
    LatentInfo.CallbackTarget = this;
    LatentInfo.ExecutionFunction = "OnPastLevelLoaded";
    LatentInfo.Linkage = 0;
    LatentInfo.UUID = 0;
    
    UGameplayStatics::LoadStreamLevel(GetWorld(), PendingPastLevelName, true, true, LatentInfo);
}

void UTimelineManager::RequestReturnToPresent()
{
    if (bIsTransitioning || CurrentTimeline != ETimelineState::Past)
    {
        return;
    }

    bIsTransitioning = true;
    CurrentTimeline = ETimelineState::Transitioning;

    // Play reverse sequence if available
    if (PendingSequence.IsValid())
    {
        ULevelSequence* Sequence = PendingSequence.LoadSynchronous();
        if (Sequence)
        {
            ALevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
                Sequence, FMovieSceneSequencePlaybackSettings(), GetWorld());
            if (SequencePlayer)
            {
                SequencePlayer->PlayReverse();
                
                FScriptDelegate Delegate;
                Delegate.BindUFunction(this, "OnPhaseShiftSequenceFinished");
                SequencePlayer->OnFinished.AddUnique(Delegate);
            }
        }
    }

    // Unload past level
    if (PastLevelStreaming.Contains(PendingPastLevelName))
    {
        FLatentActionInfo LatentInfo;
        LatentInfo.CallbackTarget = this;
        LatentInfo.ExecutionFunction = "OnPastLevelUnloaded";
        LatentInfo.Linkage = 0;
        UGameplayStatics::UnloadStreamLevel(GetWorld(), PendingPastLevelName, LatentInfo, true);
    }
}

bool UTimelineManager::CanPhaseShift(AVistaNode* Node) const
{
    if (!Node) return false;
    
    FVistaNodeData* NodeData = VistaNodeRegistry.Find(Node->GetNodeID());
    if (!NodeData) return false;

    // Check required organ
    if (NodeData->RequiredOrgan != NAME_None)
    {
        // Would check against player's owned organs
        // For now, allow all
    }

    return true;
}

void UTimelineManager::OnPastLevelLoaded()
{
    // Mark past level as loaded
    ULevelStreaming* StreamingLevel = PastLevelStreaming.FindRef(PendingPastLevelName);
    if (!StreamingLevel)
    {
        StreamingLevel = UGameplayStatics::GetStreamingLevel(GetWorld(), PendingPastLevelName);
        if (StreamingLevel)
        {
            PastLevelStreaming.Add(PendingPastLevelName, StreamingLevel);
        }
    }

    // Spawn PastPawn at Vista Node anchor
    if (CurrentVistaNode)
    {
        // In a full implementation, we'd swap the player pawn here
        // For now, just activate the cinematic camera
        CurrentVistaNode->ActivateCinematicCamera();
    }

    CurrentTimeline = ETimelineState::Past;
    OnTimelineChanged.Broadcast(ETimelineState::Present, ETimelineState::Past);

    // Re-enable input for past interaction (limited)
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        PC->SetIgnoreMoveInput(false);
        PC->SetIgnoreLookInput(false);
    }

    OnPhaseShiftComplete.Broadcast(CurrentVistaNode);
    bIsTransitioning = false;
}

void UTimelineManager::OnPhaseShiftSequenceFinished()
{
    // Sequence finished, transition state handled in OnPastLevelLoaded/Unloaded
}

void UTimelineManager::RecordZoneChange(FName ZoneID, const FZoneStateChange& Change)
{
    FZoneState* State = ZoneStates.Find(ZoneID);
    if (State)
    {
        State->PendingChanges.Add(Change);
        State->ApplyChange(Change);
        OnZoneStateChanged.Broadcast(ZoneID);
    }
}

void UTimelineManager::ApplyZoneState(FName ZoneID)
{
    FZoneState* State = ZoneStates.Find(ZoneID);
    if (State)
    {
        // Apply all pending changes to the present world
        // This would trigger level streaming updates, actor visibility, etc.
        for (const FZoneStateChange& Change : State->PendingChanges)
        {
            // Implementation depends on change type
            // e.g., for BridgeExtended: show/hide bridge actors, update navigation
        }
    }
}

FZoneState UTimelineManager::GetZoneState(FName ZoneID) const
{
    const FZoneState* State = ZoneStates.Find(ZoneID);
    return State ? *State : FZoneState();
}

void UTimelineManager::UnlockFastTravel(FName ZoneID)
{
    if (!FastTravelUnlocked.Contains(ZoneID))
    {
        FastTravelUnlocked.Add(ZoneID);
    }
}

void UTimelineManager::RequestFastTravel(FName ZoneID)
{
    // Find a Vista Node in the target zone and teleport
    // Implementation depends on level structure
}

void UTimelineManager::CaptureStateForSave()
{
    // Called by SaveSystem to capture current timeline state
    // State is already in ZoneStates, ActivatedVistaNodes, FastTravelUnlocked
}

void UTimelineManager::RestoreStateFromSave()
{
    // Called by SaveSystem after loading
    // Apply all zone states to present world
    for (auto& Pair : ZoneStates)
    {
        ApplyZoneState(Pair.Key);
    }
}

FName UTimelineManager::GetPastLevelName(FName ZoneID) const
{
    return FName(*FString::Printf(TEXT("Past_%s"), *ZoneID.ToString()));
}