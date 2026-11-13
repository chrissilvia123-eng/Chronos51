// Copyright Chronos51 Studios 2026

#include "Chronos51GameInstance.h"
#include "TimelineManager.h"
#include "PuzzleManager.h"
#include "SaveSystem.h"

UChronos51GameInstance::UChronos51GameInstance()
{
    CurrentPlaythroughID = 0;
}

void UChronos51GameInstance::Init()
{
    Super::Init();

    // Subsystems are auto-initialized by GameInstance
    // Access them via GetSubsystem<>() when needed

    UE_LOG(LogTemp, Log, TEXT("Chronos51 GameInstance initialized"));
}

void UChronos51GameInstance::Shutdown()
{
    Super::Shutdown();
    UE_LOG(LogTemp, Log, TEXT("Chronos51 GameInstance shutdown"));
}

class UTimelineManager* UChronos51GameInstance::GetTimelineManager() const
{
    return GetSubsystem<UTimelineManager>();
}

class UPuzzleManager* UChronos51GameInstance::GetPuzzleManager() const
{
    return GetSubsystem<UPuzzleManager>();
}

class USaveSystem* UChronos51GameInstance::GetSaveSystem() const
{
    return GetSubsystem<USaveSystem>();
}

void UChronos51GameInstance::StartNewPlaythrough()
{
    CurrentPlaythroughID = FMath::RandRange(10000, 99999);
    
    // Reset all subsystems
    if (UTimelineManager* TM = GetTimelineManager())
    {
        // TimelineManager state reset would happen here
    }
    if (UPuzzleManager* PM = GetPuzzleManager())
    {
        // PuzzleManager state reset
    }
    if (USaveSystem* SS = GetSaveSystem())
    {
        SS->PlaytimeSeconds = 0.0f;
    }
}

void UChronos51GameInstance::EndPlaythrough()
{
    // Final autosave
    if (USaveSystem* SS = GetSaveSystem())
    {
        SS->RequestAutosave();
    }
}