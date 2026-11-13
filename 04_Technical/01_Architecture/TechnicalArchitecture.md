# Chronos 51 — Technical Architecture (UE5)

## 1. Project Setup & Configuration

### 1.1 UE5 Project Settings
```ini
; DefaultEngine.ini
[/Script/EngineSettings.GeneralProjectSettings]
ProjectName=Chronos51
ProjectVersion=1.0.0
CompanyName=Chronos51 Studios
CopyrightNotice=Chronos51 Studios 2026

[/Script/Engine.RendererSettings]
r.Nanite=1
r.Lumen=1
r.VirtualTextures=1
r.VirtualTextureReducedMemory=0
r.RayTracing=1
r.DynamicGlobalIllumination=1
r.Shadow.Virtual.Enable=1

[/Script/Engine.WorldPartitionSettings]
bEnableWorldPartition=True
DefaultGridCellSize=50000
DefaultLoadingRange=150000

[/Script/Engine.PhysicsSettings]
DefaultPhysicsScene=Enhanced
PhysicsTimestep=0.01666
bSubstepping=True
MaxSubstepDeltaTime=0.01666
MaxSubsteps=6

[/Script/Engine.InputSettings]
bEnableEnhancedInput=1
```

### 1.2 Plugin Requirements
| Plugin | Purpose | Status |
|--------|---------|--------|
| Enhanced Input | Input mapping contexts | Built-in |
| Gameplay Ability System (GAS) | Organ abilities, effects | Built-in |
| Niagara | VFX (Phase Shift, temporal, Astra) | Built-in |
| MetaSounds | Audio (adaptive music, spatial VO) | Built-in |
| World Partition | Streaming large world | Built-in |
| USD Support | Blender→UE5 pipeline | Built-in |
| Articy Draft 3 | Narrative design (optional) | Marketplace |
| Yarn Spinner UE5 | Dialogue runtime | Custom/Community |

### 1.3 C++ Module Structure
```
Chronos51/
├── Source/
│   ├── Chronos51/
│   │   ├── Chronos51.Build.cs
│   │   ├── Public/
│   │   │   ├── Chronos51GameModeBase.h
│   │   │   ├── Chronos51PlayerController.h
│   │   │   ├── Chronos51Character.h
│   │   │   ├── TimelineManager.h
│   │   │   ├── ZoneState.h
│   │   │   ├── PuzzleManager.h
│   │   │   ├── AstraCompanion.h
│   │   │   ├── VistaNode.h
│   │   │   ├── OrganAbility.h
│   │   │   ├── SaveSystem.h
│   │   │   └── DialogueRunnerSubsystem.h
│   │   └── Private/
│   │       ├── Chronos51GameModeBase.cpp
│   │       ├── Chronos51PlayerController.cpp
│   │       ├── Chronos51Character.cpp
│   │       ├── TimelineManager.cpp
│   │       ├── ZoneState.cpp
│   │       ├── PuzzleManager.cpp
│   │       ├── AstraCompanion.cpp
│   │       ├── VistaNode.cpp
│   │       ├── OrganAbility.cpp
│   │       ├── SaveSystem.cpp
│   │       └── DialogueRunnerSubsystem.cpp
│   └── Chronos51Editor/
│       └── (Editor utilities, custom asset types)
```

---

## 2. Core System Architecture

### 2.1 Timeline Manager (GameInstance Subsystem)
```cpp
// TimelineManager.h
UCLASS()
class UTimelineManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    // Timeline state
    UPROPERTY(BlueprintReadOnly) ETimelineState CurrentTimeline = ETimelineState::Present;
    UPROPERTY(BlueprintReadOnly) TMap<FName, FZoneState> ZoneStates;
    UPROPERTY(BlueprintReadOnly) TArray<FName> ActivatedVistaNodes;
    UPROPERTY(BlueprintReadOnly) TArray<FName> FastTravelUnlocked;

    // Phase Shift
    UFUNCTION(BlueprintCallable) void RequestPhaseShift(AVistaNode* TargetNode);
    UFUNCTION(BlueprintCallable) void ReturnToPresent();
    UFUNCTION(BlueprintImplementableEvent) void OnPhaseShiftBegin(AVistaNode* Node);
    UFUNCTION(BlueprintImplementableEvent) void OnPhaseShiftComplete(AVistaNode* Node);
    UFUNCTION(BlueprintImplementableEvent) void OnTimelineChanged(ETimelineState Old, ETimelineState New);

    // Zone State Application
    UFUNCTION(BlueprintCallable) void ApplyZoneState(FName ZoneID);
    UFUNCTION(BlueprintCallable) void RecordZoneChange(FName ZoneID, const FZoneStateChange& Change);

    // Save/Load Integration
    UFUNCTION(BlueprintCallable) FChronosSaveData CaptureTimelineState();
    UFUNCTION(BlueprintCallable) void RestoreTimelineState(const FChronosSaveData& Data);

private:
    TMap<FName, ULevelStreaming*> PastLevelStreaming;
    AVistaNode* CurrentVistaNode = nullptr;
    FTimelineDelegate PhaseShiftDelegate;
};
```

### 2.2 Zone State System
```cpp
// ZoneState.h
UENUM(BlueprintType)
enum class ETimelineState : uint8
{
    Present,
    Past
};

USTRUCT(BlueprintType)
struct FZoneStateChange
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere) FName ChangeID;
    UPROPERTY(EditAnywhere) EZoneChangeType Type; // WaterLevel, BridgeExtended, RadiationCleared, DoorUnlocked, MachineRepaired
    UPROPERTY(EditAnywhere) float TargetValue;
    UPROPERTY(EditAnywhere) AActor* TargetActor; // Optional direct reference
    UPROPERTY(EditAnywhere) float TransitionTime = 3.0f;
};

USTRUCT(BlueprintType)
struct FZoneState
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere) FName ZoneID;
    UPROPERTY(EditAnywhere) float WaterLevel = 1.0f;
    UPROPERTY(EditAnywhere) bool bBridgeExtended = false;
    UPROPERTY(EditAnywhere) bool bRadiationCleared = false;
    UPROPERTY(EditAnywhere) TMap<FName, bool> DoorStates;
    UPROPERTY(EditAnywhere) TMap<FName, bool> MachineStates;
    UPROPERTY(EditAnywhere) TArray<FZoneStateChange> PendingChanges;
    
    UFUNCTION(BlueprintCallable) void ApplyChange(const FZoneStateChange& Change);
    UFUNCTION(BlueprintCallable) void RevertChange(const FZoneStateChange& Change);
};
```

### 2.3 Puzzle Manager (ActorComponent)
```cpp
// PuzzleManager.h
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UPuzzleManager : public UActorComponent
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly) TMap<FName, FPuzzleInstance> ActivePuzzles;
    UPROPERTY(EditAnywhere) TObjectPtr<UDataAsset> PuzzleDefinitions; // DA_PuzzleDefinition array

    UFUNCTION(BlueprintCallable) bool StartPuzzle(FName PuzzleID);
    UFUNCTION(BlueprintCallable) void ProcessPuzzleAction(FName PuzzleID, FName StepID, const TMap<FName, FString>& ActionData);
    UFUNCTION(BlueprintCallable) bool CheckPuzzleSolved(FName PuzzleID);
    
    UFUNCTION(BlueprintImplementableEvent) void OnPuzzleStepComplete(FName PuzzleID, FName StepID);
    UFUNCTION(BlueprintImplementableEvent) void OnPuzzleSolved(FName PuzzleID, FName OrganUnlock);
    UFUNCTION(BlueprintImplementableEvent) void OnPuzzleFailed(FName PuzzleID);

    // Hint System
    UFUNCTION(BlueprintCallable) FText GetHint(FName PuzzleID, int32 Tier);
    UFUNCTION(BlueprintCallable) bool PurchaseHint(FName PuzzleID, int32 Tier);

private:
    TMap<FName, TObjectPtr<UDA_PuzzleDefinition>> PuzzleDefMap;
    UPuzzleManager() { PrimaryComponentTick.bCanEverTick = false; }
};
```

### 2.4 Organ Abilities (GAS Integration)
```cpp
// OrganAbility.h
UCLASS()
class UOrganAbility : public UGameplayAbility
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EOrganType OrganType;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FGameplayTagContainer RequiredTags;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FGameplayTagContainer BlockedTags;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Cooldown = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Cost = 0.0f; // Resonance cost if any

    // Organ-specific overrides
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
    virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
};

UENUM(BlueprintType)
enum class EOrganType : uint8
{
    None,
    ChronosCore,    // Time Dilation Bubble
    NeuralLoom,     // Past Vision Overlay
    VoidSail,       // Double Jump + Glide
    GravityAnchor,  // Wall Run / Ceiling Hang
    AstraCore       // Final Synthesis
};
```

### 2.5 Astra Companion (Behavior Tree + Gamespeak)
```cpp
// AstraCompanion.h
UCLASS()
class AAstraCompanion : public ACharacter
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TObjectPtr<UBehaviorTree> BehaviorTree;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TObjectPtr<UBlackboardData> BlackboardAsset;
    
    UPROPERTY(BlueprintReadOnly) EAstraState CurrentState = EAstraState::Idle;
    UPROPERTY(BlueprintReadOnly) TObjectPtr<AVistaNode> CurrentTargetNode;
    UPROPERTY(BlueprintReadOnly) FName CurrentPuzzleAssistID;

    // Gamespeak Commands
    UFUNCTION(BlueprintCallable, Category="Gamespeak") void Command_Scan();
    UFUNCTION(BlueprintCallable, Category="Gamespeak") void Command_Wait();
    UFUNCTION(BlueprintCallable, Category="Gamespeak") void Command_Follow(FName TargetNodeID);
    UFUNCTION(BlueprintCallable, Category="Gamespeak") void Command_Recall();
    UFUNCTION(BlueprintCallable, Category="Gamespeak") void Command_Hold(FName TargetID);

    // Puzzle Assist
    UFUNCTION(BlueprintCallable) void BeginPuzzleAssist(FName PuzzleID, FName TargetID);
    UFUNCTION(BlueprintCallable) void EndPuzzleAssist();

    // Visual/Audio
    UFUNCTION(BlueprintCallable) void SetTrustLevel(int32 Trust);
    UFUNCTION(BlueprintCallable) void PlayMemoryFragment(int32 FragmentID);
    UFUNCTION(BlueprintCallable) void SetVisualState(EAstraVisualState State);

protected:
    UPROPERTY() TObjectPtr<UBehaviorTreeComponent> BTComponent;
    UPROPERTY() TObjectPtr<UBlackboardComponent> BlackboardComponent;
};

UENUM(BlueprintType)
enum class EAstraState : uint8
{
    Idle,
    Following,
    WaitingAtNode,
    PuzzleAssist,
    Scanning,
    Recalled,
    MemoryFragment
};

UENUM(BlueprintType)
enum class EAstraVisualState : uint8
{
    Idle,
    Scanning,
    Memory,
    Distress,
    Unity
};
```

---

## 3. Phase Shift Implementation

### 3.1 Transition Sequence
```cpp
// TimelineManager.cpp - Phase Shift Flow
void UTimelineManager::RequestPhaseShift(AVistaNode* TargetNode)
{
    if (!TargetNode || !TargetNode->CanPhaseShift()) return;
    
    CurrentVistaNode = TargetNode;
    OnPhaseShiftBegin.Broadcast(TargetNode);
    
    // 1. Disable player input
    GetPlayerController()->SetInputMode(FInputModeGameOnly());
    GetPlayerController()->bShowMouseCursor = false;
    
    // 2. Start camera blend (Level Sequence)
    ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
        TargetNode->PhaseShiftSequence, FMovieSceneSequencePlaybackSettings(), World);
    SequencePlayer->Play();
    
    // 3. Async load past level
    FName PastLevelName = FName(*FString::Printf(TEXT("Past_%s"), *TargetNode->ZoneID.ToString()));
    FLatentActionInfo LatentInfo;
    LatentInfo.CallbackTarget = this;
    LatentInfo.ExecutionFunction = "OnPastLevelLoaded";
    LatentInfo.Linkage = 0;
    UGameplayStatics::LoadStreamLevel(World, PastLevelName, true, true, LatentInfo);
    
    // 4. Swap pawn to PastPawn (stripped traversal)
    // Handled in OnPastLevelLoaded
}

void UTimelineManager::OnPastLevelLoaded()
{
    // Spawn PastPawn at Vista Node anchor
    APawn* PastPawn = World->SpawnActor<APawn>(PastPawnClass, CurrentVistaNode->GetActorTransform());
    GetPlayerController()->Possess(PastPawn);
    
    // Blend camera to fixed cinematic
    CurrentVistaNode->ActivateCinematicCamera();
    
    CurrentTimeline = ETimelineState::Past;
    OnTimelineChanged.Broadcast(ETimelineState::Present, ETimelineState::Past);
    OnPhaseShiftComplete.Broadcast(CurrentVistaNode);
}
```

### 3.2 Camera Blend (Sequencer-Driven)
- **PhaseShift_MasterSequence**: Master sequence per Vista Node
- **Tracks:** Camera (shoulder → fixed), PostProcess (temporal distortion), Audio (temporal sweep), Visibility (present actors hidden, past actors shown)
- **Duration:** 3 seconds (1.5s blend in, 1.5s blend out)
- **Accessibility:** Instant cut version (0.1s) for reduced motion

---

## 4. Save System (Atomic, Versioned, Dual-Timeline)

### 4.1 Save Data Structure
```cpp
// SaveSystem.h
USTRUCT(BlueprintType)
struct FChronosSaveData
{
    GENERATED_BODY()
    
    UPROPERTY() int32 Version = 5;
    UPROPERTY() int32 Slot = 0;
    UPROPERTY() FDateTime Timestamp;
    UPROPERTY() float PlaytimeSeconds;
    
    UPROPERTY() ETimelineState CurrentTimeline;
    UPROPERTY() FName CurrentZone;
    UPROPERTY() FTransform PlayerTransform;
    
    UPROPERTY() TArray<FName> ActivatedVistaNodes;
    UPROPERTY() TArray<FName> FastTravelUnlocked;
    UPROPERTY() TArray<FName> OrgansRestored;
    UPROPERTY() TArray<FName> ActiveAbilities;
    
    UPROPERTY() TMap<FName, FPuzzleSaveState> PuzzleStates;
    UPROPERTY() TMap<FName, FZoneState> ZoneStates;
    
    UPROPERTY() int32 AstraTrust;
    UPROPERTY() TArray<int32> AstraMemoryFragments;
    UPROPERTY() int32 ResonanceCurrency;
    
    UPROPERTY() FGameSettings SaveSettings;
};

USTRUCT(BlueprintType)
struct FPuzzleSaveState
{
    GENERATED_BODY()
    UPROPERTY() int32 CurrentStep = 0;
    UPROPERTY() TMap<FName, FString> StepData;
    UPROPERTY() bool bSolved = false;
    UPROPERTY() float SolveTimestamp;
};
```

### 4.2 Atomic Write Pattern
```cpp
// SaveSystem.cpp
bool USaveSystem::SaveGame(int32 Slot, const FChronosSaveData& Data)
{
    FString SaveDir = FPaths::ProjectSavedDir() / TEXT("Saves");
    FString SlotName = FString::Printf(TEXT("Save_Slot_%d.json"), Slot);
    FString TempPath = SaveDir / (SlotName + TEXT(".tmp"));
    FString FinalPath = SaveDir / SlotName;
    FString BackupPath = SaveDir / (SlotName + TEXT(".bak"));
    
    // Serialize to JSON
    FString JsonString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
    FJsonSerializer::Serialize(Data.ToJsonObject().ToSharedRef(), Writer);
    
    // Write temp file
    if (!FFileHelper::SaveStringToFile(JsonString, *TempPath, FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM))
    {
        UE_LOG(LogChronos51, Error, TEXT("Failed to write temp save file"));
        return false;
    }
    
    // Atomic rename (keep backup)
    IFileManager::Get().Move(*FinalPath, *BackupPath, true, true);
    if (!IFileManager::Get().Move(*TempPath, *FinalPath, true, true))
    {
        UE_LOG(LogChronos51, Error, TEXT("Failed to rename save file"));
        return false;
    }
    
    return true;
}
```

### 4.3 Migration Chain
```cpp
// SaveSystem.cpp - Version Migration
FChronosSaveData USaveSystem::MigrateSaveData(FChronosSaveData Data)
{
    while (Data.Version < CURRENT_SAVE_VERSION)
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
```

---

## 5. Input System (Enhanced Input)

### 5.1 Input Mapping Contexts
```cpp
// InputConfig.h
USTRUCT(BlueprintType)
struct FChronosInputConfig
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere) TObjectPtr<UInputMappingContext> IMC_Traversal;
    UPROPERTY(EditAnywhere) TObjectPtr<UInputMappingContext> IMC_PuzzlePanel;
    UPROPERTY(EditAnywhere) TObjectPtr<UInputMappingContext> IMC_Journal;
    UPROPERTY(EditAnywhere) TObjectPtr<UInputMappingContext> IMC_PhotoMode;
    UPROPERTY(EditAnywhere) TObjectPtr<UInputMappingContext> IMC_Cinematic;
    UPROPERTY(EditAnywhere) TObjectPtr<UInputMappingContext> IMC_AstraCommand;
};
```

### 5.2 Input Actions (DataAssets)
| Action | Type | Default Keyboard | Default Gamepad | Contexts |
|--------|------|------------------|-----------------|----------|
| IA_Move | Axis2D | WASD | LStick | Traversal |
| IA_Look | Axis2D | Mouse | RStick | Traversal |
| IA_Jump | Digital | Space | A | Traversal |
| IA_Mantle | Digital | Space (context) | A (context) | Traversal |
| IA_Sprint | Digital | LShift | L3 | Traversal |
| IA_Crouch | Digital | LCtrl | B | Traversal |
| IA_Phase | Digital (Hold) | Q | L2/LT | Traversal (Vista Node) |
| IA_Organ_1..4 | Digital | 1/2/3/4 | D-Pad | Traversal |
| IA_Astra_Command | Digital | E | Y | AstraCommand |
| IA_Interact | Digital | F | X | Traversal |
| IA_Journal | Digital | J | View | Journal |
| IA_PhotoMode | Digital | F12 | — | PhotoMode |

---

## 6. Audio Architecture (MetaSounds + Audio Buses)

### 6.1 Bus Hierarchy
```
Master Bus
├── Music Bus (Adaptive: Base/Tension/Resolve layers)
├── Ambience Bus (Per-zone loops, crossfade)
├── SFX Bus (Interactions, mechanisms, abilities)
├── UI Bus (Menu, journal, hints)
└── VO Bus (Astra, NPC logs, Thorne echo)
```

### 6.2 Adaptive Music (MetaSound)
```metasound
// Music_MetaSound.uasset
Inputs:
  - ZoneID (Enum)
  - Intensity (Float 0-1) // Driven by puzzle proximity + unsolved count
  - Timeline (Enum: Present/Past)

Layers:
  - Base: Always playing, zone-specific ambient
  - Tension: Fades in at Intensity > 0.3
  - Resolve: Triggers on puzzle solve/organ restore (one-shot)

Transitions:
  - Crossfade 3s on Zone change
  - Past timeline: 1940s-60s orchestral/electronic palette
  - Present timeline: Ambient, minimal, bioluminescent tones
```

---

## 7. Performance Budgets & Optimization

### 7.1 Target Specs
| Platform | Resolution | Target FPS | GPU | CPU |
|----------|------------|------------|-----|-----|
| PC High | 1440p | 60 | RTX 3070 / RX 6700 XT | Ryzen 5 5600X / i5-12400 |
| PC Ultra | 4K | 60 | RTX 4080 / RX 7900 XTX | Ryzen 7 7800X3D / i7-13700K |
| Console | 4K | 30 | PS5 / Xbox Series X | Zen 2 / Zen 3 |

### 7.2 Frame Budget (16.67ms @ 60fps)
| System | Budget | Notes |
|--------|--------|-------|
| Base Pass (Nanite) | 4-6ms | Cluster culling, rasterization |
| Lumen GI | 3-5ms | Surface cache, screen probe gather |
| Lumen Reflections | 1-2ms | Glossy surfaces only |
| Virtual Shadow Maps | 2-3ms | Directional + local lights |
| Niagara VFX | 1-2ms | GPU compute, temporal AA |
| Gameplay/Logic | 1-2ms | GAS, Timeline, Puzzle, AI |
| UI/PostProcess | 1ms | Bloom, DOF, color grade |
| **Total** | **13-21ms** | Headroom for spikes |

### 7.3 Optimization Levers (CVars)
```ini
; Scalability settings
r.Lumen.ScreenProbeGather.IntegrateDownsampleFactor=2
r.Lumen.ScreenProbeGather.SpatialFilterNumPasses=1
r.Lumen.ScreenProbeGather.StochasticInterpolation=1
r.Lumen.Reflections.Quality=2
r.Nanite.MinPixelsPerEdgeHW=32
r.Shadow.Virtual.Cache=1
r.Streaming.PoolSize=2048
```

---

## 8. CI/CD Pipeline (GitHub Actions)

### 8.1 Build Pipeline
```yaml
# .github/workflows/ue5-build.yml
name: UE5 Build & Test
on: [push, pull_request]

jobs:
  build:
    runs-on: windows-latest
    steps:
      - uses: actions/checkout@v4
        with:
          lfs: true
      - name: Setup UE5
        uses: peterrdev/setup-unreal-engine@v1
        with:
          version: '5.3'
      - name: Generate Project Files
        run: |
          & "C:\Program Files\Epic Games\UE_5.3\Engine\Build\BatchFiles\Build.bat" Chronos51Editor Win64 Development -Project="E:/Chronos51/05_UE5_Project/Chronos51/Chronos51.uproject"
      - name: Build Editor
        run: |
          & "C:\Program Files\Epic Games\UE_5.3\Engine\Build\BatchFiles\Build.bat" Chronos51Editor Win64 Development -Project="E:/Chronos51/05_UE5_Project/Chronos51/Chronos51.uproject" -WaitMutex
      - name: Run Unit Tests
        run: |
          & "C:\Program Files\Epic Games\UE_5.3\Engine\Binaries\Win64\UnrealEditor-Cmd.exe" "E:/Chronos51/05_UE5_Project/Chronos51/Chronos51.uproject" -run=AutomationTest -TestNames="Chronos51.*" -unattended -nullrhi -nosound
      - name: Upload Build Artifacts
        uses: actions/upload-artifact@v4
        with:
          name: Chronos51-Editor
          path: E:/Chronos51/05_UE5_Project/Chronos51/Binaries/Win64/
```

### 8.2 Automated Playtest (Headless)
```yaml
  playtest:
    needs: build
    runs-on: windows-latest
    steps:
      - name: Run Gameplay Verification
        run: |
          & "C:\Program Files\Epic Games\UE_5.3\Engine\Binaries\Win64\UnrealEditor-Cmd.exe" "E:/Chronos51/05_UE5_Project/Chronos51/Chronos51.uproject" -game -ExecCmds="Chronos51.VerifyCriticalPath" -unattended -nullrhi -nosound -log
```