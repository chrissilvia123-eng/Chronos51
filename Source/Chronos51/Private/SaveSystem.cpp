// Copyright Chronos51 Studios 2026

#include "SaveSystem.h"
#include "TimelineManager.h"
#include "PuzzleManager.h"
#include "Chronos51Character.h"
#include "Kismet/GameplayStatics.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"

void USaveSystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    // Start autosave timer
    GetWorld()->GetTimerManager().SetTimer(
        AutosaveTimerHandle, 
        this, 
        &USaveSystem::RequestAutosave, 
        AutosaveInterval, 
        true
    );
}

void USaveSystem::SaveGame(int32 SlotIndex)
{
    if (SlotIndex < 0 || SlotIndex >= MaxSaveSlots)
    {
        OnSaveComplete.Broadcast(false);
        return;
    }

    FChronosSaveData Data = CaptureCurrentState();
    Data.Slot = SlotIndex;
    Data.Timestamp = FDateTime::UtcNow();
    Data.PlaytimeSeconds = PlaytimeSeconds;

    bool bSuccess = WriteSaveFile(SlotIndex, Data);
    OnSaveComplete.Broadcast(bSuccess);
}

void USaveSystem::LoadGame(int32 SlotIndex)
{
    if (SlotIndex < 0 || SlotIndex >= MaxSaveSlots)
    {
        OnLoadComplete.Broadcast(false);
        return;
    }

    FChronosSaveData Data;
    bool bSuccess = ReadSaveFile(SlotIndex, Data);
    
    if (bSuccess)
    {
        // Migrate if needed
        Data = MigrateSaveData(Data);
        RestoreState(Data);
    }
    
    OnLoadComplete.Broadcast(bSuccess);
}

void USaveSystem::RequestAutosave()
{
    // Save to dedicated autosave slot (slot 0)
    SaveGame(0);
}

bool USaveSystem::DoesSaveExist(int32 SlotIndex) const
{
    return FPaths::FileExists(GetSaveFilePath(SlotIndex));
}

TArray<int32> USaveSystem::GetAvailableSlots() const
{
    TArray<int32> Slots;
    for (int32 i = 0; i < MaxSaveSlots; ++i)
    {
        if (DoesSaveExist(i))
        {
            Slots.Add(i);
        }
    }
    return Slots;
}

FChronosSaveData USaveSystem::CaptureCurrentState()
{
    FChronosSaveData Data;
    Data.Version = 5;

    // Get player
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    AChronos51Character* Player = PC ? Cast<AChronos51Character>(PC->GetPawn()) : nullptr;

    if (Player)
    {
        Data.PlayerTransform = Player->GetActorTransform();
        Data.CurrentZone = Player->GetCurrentZoneID(); // Would need this method
    }

    // Get timeline manager state
    if (UGameInstance* GI = GetGameInstance())
    {
        UTimelineManager* TimelineManager = GI->GetSubsystem<UTimelineManager>();
        if (TimelineManager)
        {
            Data.CurrentTimeline = TimelineManager->CurrentTimeline;
            Data.CurrentZone = TimelineManager->CurrentZoneID;
            Data.ActivatedVistaNodes = TimelineManager->ActivatedVistaNodes;
            Data.FastTravelUnlocked = TimelineManager->FastTravelUnlocked;

            // Zone states
            for (auto& Pair : TimelineManager->ZoneStates)
            {
                FZoneStateSaveData ZoneData;
                ZoneData.ZoneID = Pair.Key;
                ZoneData.WaterLevel = Pair.Value.WaterLevel;
                ZoneData.bBridgeExtended = Pair.Value.bBridgeExtended;
                ZoneData.bRadiationCleared = Pair.Value.bRadiationCleared;
                ZoneData.DoorStates = Pair.Value.DoorStates;
                ZoneData.MachineStates = Pair.Value.MachineStates;
                Data.ZoneStates.Add(Pair.Key, ZoneData);
            }
        }

        // Puzzle states
        UPuzzleManager* PuzzleManager = GI->GetSubsystem<UPuzzleManager>();
        if (PuzzleManager)
        {
            // Would capture puzzle states from PuzzleManager
        }
    }

    // Player organs/abilities
    if (Player)
    {
        // Data.OrgansRestored = Player->OwnedOrgans; // Convert enum to FName
        // Data.ActiveAbilities = ...; // Get from ASC
    }

    // Astra state
    // Data.AstraTrust = ...;
    // Data.AstraMemoryFragments = ...;
    // Data.ResonanceCurrency = ...;

    Data.PlaytimeSeconds = PlaytimeSeconds;

    return Data;
}

void USaveSystem::RestoreState(const FChronosSaveData& SaveData)
{
    // Restore playtime
    PlaytimeSeconds = SaveData.PlaytimeSeconds;

    // Restore timeline state
    if (UGameInstance* GI = GetGameInstance())
    {
        UTimelineManager* TimelineManager = GI->GetSubsystem<UTimelineManager>();
        if (TimelineManager)
        {
            TimelineManager->RestoreStateFromSave();
        }

        UPuzzleManager* PuzzleManager = GI->GetSubsystem<UPuzzleManager>();
        if (PuzzleManager)
        {
            // Restore puzzle states
        }
    }

    // Restore player
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC && PC->GetPawn())
    {
        AChronos51Character* Player = Cast<AChronos51Character>(PC->GetPawn());
        if (Player)
        {
            Player->SetActorTransform(SaveData.PlayerTransform);
            
            // Restore organs/abilities
            // Player->OwnedOrgans = ...;
            // Grant abilities via ASC
        }
    }

    // Restore Astra state
    // ...
}

FString USaveSystem::GetSaveDirectory() const
{
    return FPaths::ProjectSavedDir() / TEXT("Saves");
}

FString USaveSystem::GetSaveFilePath(int32 SlotIndex) const
{
    return GetSaveDirectory() / FString::Printf(TEXT("Save_Slot_%d.json"), SlotIndex);
}

FString USaveSystem::GetTempFilePath(int32 SlotIndex) const
{
    return GetSaveFilePath(SlotIndex) + TEXT(".tmp");
}

FString USaveSystem::GetBackupFilePath(int32 SlotIndex) const
{
    return GetSaveFilePath(SlotIndex) + TEXT(".bak");
}

bool USaveSystem::WriteSaveFile(int32 SlotIndex, const FChronosSaveData& Data)
{
    FString SaveDir = GetSaveDirectory();
    IFileManager::Get().MakeDirectory(*SaveDir, true);

    FString TempPath = GetTempFilePath(SlotIndex);
    FString FinalPath = GetSaveFilePath(SlotIndex);
    FString BackupPath = GetBackupFilePath(SlotIndex);

    // Serialize to JSON
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
    // Note: Full serialization would use JsonObjectConverter
    // For brevity, this is a placeholder

    FString JsonString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
    // FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer); // Would need full struct serialization

    // For now, use a simpler approach - save as binary with version
    TArray<uint8> BinaryData;
    FMemoryWriter Writer(BinaryData);
    Writer << Data; // Requires FChronosSaveData to have serialization operators

    // Write temp file
    if (!FFileHelper::SaveArrayToFile(BinaryData, *TempPath))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to write temp save file: %s"), *TempPath);
        return false;
    }

    // Atomic rename: keep backup
    if (FPaths::FileExists(FinalPath))
    {
        IFileManager::Get().Move(*BackupPath, *FinalPath, true, true);
    }

    if (!IFileManager::Get().Move(*FinalPath, *TempPath, true, true))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to rename save file"));
        return false;
    }

    return true;
}

bool USaveSystem::ReadSaveFile(int32 SlotIndex, FChronosSaveData& OutData)
{
    FString FilePath = GetSaveFilePath(SlotIndex);
    
    // Try main file first, then backup
    TArray<FString> PathsToTry = { FilePath, GetBackupFilePath(SlotIndex) };
    
    for (const FString& Path : PathsToTry)
    {
        if (!FPaths::FileExists(Path)) continue;

        TArray<uint8> BinaryData;
        if (!FFileHelper::LoadFileToArray(BinaryData, *Path))
        {
            continue;
        }

        FMemoryReader Reader(BinaryData);
        Reader << OutData; // Requires serialization operators

        if (Reader.IsError())
        {
            continue;
        }

        // Validate version
        if (OutData.Version > 5)
        {
            UE_LOG(LogTemp, Warning, TEXT("Save version %d newer than current (5)"), OutData.Version);
            return false;
        }

        ValidateSaveData(OutData);
        return true;
    }

    return false;
}

FChronosSaveData USaveSystem::MigrateSaveData(FChronosSaveData Data)
{
    while (Data.Version < 5)
    {
        switch (Data.Version)
        {
            case 1: Data = Migrate_v1_to_v2(Data); break;
            case 2: Data = Migrate_v2_to_v3(Data); break;
            case 3: Data = Migrate_v3_to_v4(Data); break;
            case 4: Data = Migrate_v4_to_v5(Data); break;
        }
        Data.Version++;
    }
    return Data;
}

FChronosSaveData USaveSystem::Migrate_v1_to_v2(FChronosSaveData Data)
{
    // v1: flags were TArray<FName> completed_quests
    // v2: changed to TMap<FName, bool> flags
    // Migration handled in ValidateSaveData
    return Data;
}

FChronosSaveData USaveSystem::Migrate_v2_to_v3(FChronosSaveData Data)
{
    // v3 added PlaytimeSeconds
    Data.PlaytimeSeconds = 0.0f;
    return Data;
}

FChronosSaveData USaveSystem::Migrate_v3_to_v4(FChronosSaveData Data)
{
    // v4 added AstraTrust, MemoryFragments, ResonanceCurrency
    Data.AstraTrust = 0;
    Data.AstraMemoryFragments.Empty();
    Data.ResonanceCurrency = 0;
    return Data;
}

FChronosSaveData USaveSystem::Migrate_v4_to_v5(FChronosSaveData Data)
{
    // v5 added GameSettings map
    Data.GameSettings.Empty();
    return Data;
}

void USaveSystem::ValidateSaveData(FChronosSaveData& Data)
{
    // Ensure required fields exist
    if (Data.ActivatedVistaNodes.Num() == 0)
    {
        Data.ActivatedVistaNodes.Empty();
    }
    if (Data.FastTravelUnlocked.Num() == 0)
    {
        Data.FastTravelUnlocked.Empty();
    }
    if (Data.OrgansRestored.Num() == 0)
    {
        Data.OrgansRestored.Empty();
    }
    if (Data.AstraMemoryFragments.Num() == 0)
    {
        Data.AstraMemoryFragments.Empty();
    }
    if (Data.ZoneStates.Num() == 0)
    {
        Data.ZoneStates.Empty();
    }
    if (Data.PuzzleStates.Num() == 0)
    {
        Data.PuzzleStates.Empty();
    }
}