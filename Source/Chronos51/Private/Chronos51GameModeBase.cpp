// Copyright Chronos51 Studios 2026

#include "Chronos51GameModeBase.h"
#include "TimelineManager.h"
#include "PuzzleManager.h"
#include "SaveSystem.h"
#include "Engine/GameInstance.h"

AChronos51GameModeBase::AChronos51GameModeBase()
{
    // Set default pawn/controller classes (will be overridden by BP)
    DefaultPawnClass = nullptr; // Set in BP
    PlayerControllerClass = nullptr; // Set in BP
}

void AChronos51GameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);

    // Core systems are created via GameInstance Subsystem, not here
    // This ensures they persist across level transitions
}

void AChronos51GameModeBase::BeginPlay()
{
    Super::BeginPlay();

    // Systems are accessed via GameInstance Subsystem
    // This is a fallback for direct access if needed
    if (UGameInstance* GI = GetGameInstance())
    {
        TimelineManager = GI->GetSubsystem<UTimelineManager>();
        PuzzleManager = GI->GetSubsystem<UPuzzleManager>();
        SaveSystem = GI->GetSubsystem<USaveSystem>();
    }
}

UTimelineManager* AChronos51GameModeBase::GetTimelineManager() const
{
    return TimelineManager;
}

UPuzzleManager* AChronos51GameModeBase::GetPuzzleManager() const
{
    return PuzzleManager;
}

USaveSystem* AChronos51GameModeBase::GetSaveSystem() const
{
    return SaveSystem;
}

void AChronos51GameModeBase::RequestAutosave()
{
    if (SaveSystem)
    {
        SaveSystem->RequestAutosave();
    }
}

void AChronos51GameModeBase::LoadGame(int32 SlotIndex)
{
    if (SaveSystem)
    {
        SaveSystem->LoadGame(SlotIndex);
    }
}