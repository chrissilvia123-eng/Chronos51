# Chronos 51 — Blueprint Setup Guide

## Overview
This guide walks through creating the Blueprint classes that sit on top of the C++ core systems. All gameplay logic lives in Blueprints; C++ provides the framework.

---

## 1. Core Blueprint Classes to Create

### 1.1 BP_Chronos51GameMode (extends AChronos51GameModeBase)
```
Class Settings:
- Default Pawn Class: BP_Xylos
- Player Controller Class: BP_Chronos51PlayerController
- Game State Class: BP_Chronos51GameState (optional)
- HUD Class: BP_Chronos51HUD

BeginPlay:
1. Get TimelineManager from GameInstance
2. Bind to OnPhaseShiftBegin, OnPhaseShiftComplete, OnTimelineChanged, OnZoneStateChanged
3. Get PuzzleManager from GameInstance
4. Bind to OnPuzzleStepComplete, OnPuzzleSolved, OnPuzzleFailed
5. Get SaveSystem from GameInstance
6. Bind to OnSaveComplete, OnLoadComplete
7. Initialize UI widgets (HUD, Journal, PhotoMode)
```

### 1.2 BP_Chronos51PlayerController (extends APlayerController)
```
Components:
- EnhancedInputComponent (auto)

BeginPlay:
1. Get InputConfig DataAsset (created in editor)
2. AddMappingContext(IMC_Traversal, 0)
3. SetInputMode(GameOnly)
4. bShowMouseCursor = false

Input Bindings (in Enhanced Input):
- IA_Move → MoveAction (Axis2D)
- IA_Look → LookAction (Axis2D)
- IA_Jump → JumpAction (Triggered)
- IA_Mantle → MantleAction (Triggered)
- IA_Sprint → SprintAction (Started/Completed)
- IA_Crouch → CrouchAction (Triggered)
- IA_Phase → PhaseAction (Started/Completed)
- IA_Organ_1..4 → OrganActions (Triggered)
- IA_Astra_Command → AstraCommandAction (Triggered)
- IA_Interact → InteractAction (Triggered)
- IA_Journal → ToggleJournal (Triggered)
- IA_PhotoMode → TogglePhotoMode (Triggered)
- IA_Pause → TogglePause (Triggered)

Functions:
- MoveAction(value): Call Pawn.MoveInput(value)
- LookAction(value): Call Pawn.LookInput(value)
- JumpAction(): Call Pawn.RequestJump()
- SprintAction(started): Pawn.SetSprinting(true)
- SprintAction(completed): Pawn.SetSprinting(false)
- CrouchAction(): Pawn.ToggleCrouch()
- PhaseAction(started): Pawn.RequestPhaseShift()
- PhaseAction(completed): Pawn.CancelPhaseShift()
- OrganAction(index): Pawn.ActivateOrganAbility(index)
- AstraCommandAction(): Open Astra Radial Menu
- InteractAction(): Pawn.Interact()
```

### 1.3 BP_Xylos (extends AChronos51Character)
```
Components (inherited from C++):
- AbilitySystemComponent
- AttributeSet
- CameraComponent (ShoulderCam)
- SpringArmComponent (with deadzone/lag settings)

SpringArm Settings:
- TargetArmLength: 300
- bUsePawnControlRotation: true
- bEnableCameraLag: true
- CameraLagSpeed: 8.0
- bEnableCameraRotationLag: true
- CameraRotationLagSpeed: 10.0
- ProbeSize: 12 (collision probe for wall avoidance)

Camera Settings:
- FieldOfView: 90
- bUsePawnControlRotation: false

CharacterMovement Settings:
- MaxWalkSpeed: 180 (walk)
- MaxWalkSpeedCrouched: 90
- MaxWalkSpeedSprinting: 450 (set via Sprint ability)
- JumpZVelocity: Calculated in C++ from JumpHeight/TimeToApex
- AirControl: 0.35
- bOrientRotationToMovement: true
- RotationRate: (0, 540, 0)
- BrakingDecelerationWalking: 2000
- GroundFriction: 3.0
- NavAgentProps: Radius 42, Height 192

Capsule:
- HalfHeight: 96
- Radius: 42

Mesh (Skeletal):
- SkeletalMesh: SK_Xylos
- AnimClass: ABP_Xylos
- RelativeLocation: (0, 0, -96)
- RelativeRotation: (0, -90, 0)

Variables (Exposed to BP):
- JumpHeight: 220 (cm)
- TimeToApex: 0.35
- FallGravityMultiplier: 1.8
- CoyoteTime: 0.1
- JumpBufferTime: 0.12
- VariableJumpMultiplier: 0.45
- MantleHeight: 150

Custom Events (from C++):
- OnPhaseShiftBegin() → Disable input, play transition VFX
- OnPhaseShiftEnd() → Re-enable input, clear VFX

Functions to Implement in BP:
- MoveInput(value): AddMovementInput
- LookInput(value): AddControllerYaw/PitchInput
- RequestJump(): Set bWantsToJump = true
- SetSprinting(bool): Set MaxWalkSpeed (180/450)
- ToggleCrouch(): Crouch/UnCrouch
- RequestPhaseShift(): Check current Vista Node → Call TimelineManager.RequestPhaseShift
- CancelPhaseShift(): Clear phase shift charge
- ActivateOrganAbility(index): Call ASC.TryActivateAbilityByTag
- Interact(): Line trace → Call BPI_Interactable.Interact
- GetCurrentZoneID(): Return current zone (from overlap volumes)
- SetCurrentVistaNode(node): Store reference for Phase Shift prompt
```

### 1.4 BP_VistaNode (extends AVistaNode)
```
Components (inherited):
- CinematicCamera (CineCameraComponent)
- TriggerVolume (BoxComponent)
- CameraPath (SplineComponent, optional)

CinematicCamera Settings:
- FieldOfView: 90 (adjust per composition)
- CurrentFocalLength: 35 (adjust: 24 wide, 50 standard, 85 detail)
- FocusSettings.ManualFocusDistance: Set to focal subject
- FocusSettings.bEnableAutofocus: false
- CurrentFStop: 2.8
- bEnableDepthOfField: true

TriggerVolume:
- BoxExtent: (200, 200, 200) — adjust per node
- Collision Profile: Trigger
- Generate Overlap Events: true

Variables (Exposed):
- NodeID: Unique name (e.g., "ReactorCore_A")
- ZoneID: Zone name (e.g., "ReactorCore")
- DisplayName: "Reactor Control Room Alpha"
- PhaseShiftSequence: LevelSequence reference
- PhaseShiftDuration: 3.0
- ConnectedNodes: Array of NodeIDs
- UnlocksPuzzles: Array of PuzzleIDs
- RequiredOrgan: OrganType (None if no gate)
- bInstantCutAccessible: true
- bReducedMotionAccessible: true

Construction Script:
1. Set CinematicCamera to inactive
2. Position TriggerVolume at player interaction point
3. If CameraPath exists, setup spline points for multi-angle

BeginPlay:
1. Bind OnComponentBeginOverlap → OnPlayerEnter
2. Bind OnComponentEndOverlap → OnPlayerExit
3. Register with TimelineManager (via GameInstance)

OnPlayerEnter(OtherActor):
1. Cast to BP_Xylos
2. If valid: Set bPlayerInRange = true
3. Player.SetCurrentVistaNode(self)
4. Show Phase Shift prompt (UI)

OnPlayerExit(OtherActor):
1. Cast to BP_Xylos
2. If valid: Set bPlayerInRange = false
3. Player.SetCurrentVistaNode(null)
4. Hide Phase Shift prompt

ActivateCinematicCamera():
1. CinematicCamera.SetActive(true, true)

DeactivateCinematicCamera():
1. CinematicCamera.SetActive(false)

GetPhaseAnchorTransform():
1. Return GetActorTransform() (or child SceneComponent)
```

### 1.5 BP_AstraCompanion (extends AAstraCompanion)
```
Components:
- AudioComponent (spatial, attenuation)
- NiagaraComponent (constellation VFX)
- BehaviorTreeComponent
- BlackboardComponent

Behavior Tree:
- BT_Astra (see Section 3)

Blackboard Keys:
- TargetNode (Object)
- CurrentState (Enum: Idle/Following/Waiting/PuzzleAssist/Scanning/Recalled/Memory)
- PlayerRef (Object)
- TrustLevel (Int)
- MemoryFragmentsCollected (Int)

Variables:
- TrustLevel: 0-10
- MemoryFragments: Array of IDs
- VisualState: Enum (Idle/Scanning/Memory/Distress/Unity)

Functions (Gamespeak Commands):
- Command_Scan(): Play Scan VFX → Call TimelineManager.ScanAtLocation
- Command_Wait(): MoveTo Blackboard TargetNode → Set State Waiting
- Command_Follow(NodeID): Find VistaNode → MoveTo → Set State Following
- Command_Recall(): MoveTo Player → Set State Recalled
- Command_Hold(TargetID): Interact with Target → Set State PuzzleAssist

Events:
- OnTrustChanged(newTrust): Update Blackboard, change dialogue pool
- OnMemoryFragmentCollected(fragmentID): Add to array, play VFX
- SetVisualState(state): Change Niagara parameters
```

---

## 2. DataAssets to Create

### 2.1 DA_InputConfig (UInputConfig)
```
Create in: /Game/Data/DA_InputConfig

Input Actions (create each as InputAction asset):
- IA_Move (Axis2D, Swallowed: false)
- IA_Look (Axis2D, Swallowed: false)
- IA_Jump (Digital, Trigger: Pressed)
- IA_Mantle (Digital, Trigger: Pressed)
- IA_Sprint (Digital, Trigger: Down/Up)
- IA_Crouch (Digital, Trigger: Pressed)
- IA_Phase (Digital, Trigger: Down/Up)
- IA_Organ_1 (Digital, Trigger: Pressed)
- IA_Organ_2 (Digital, Trigger: Pressed)
- IA_Organ_3 (Digital, Trigger: Pressed)
- IA_Organ_4 (Digital, Trigger: Pressed)
- IA_Astra_Command (Digital, Trigger: Pressed)
- IA_Interact (Digital, Trigger: Pressed)
- IA_Journal (Digital, Trigger: Pressed)
- IA_PhotoMode (Digital, Trigger: Pressed)
- IA_Pause (Digital, Trigger: Pressed)

Mapping Contexts (create each as InputMappingContext asset):
- IMC_Traversal (Priority 0, Auto Activate)
- IMC_PuzzlePanel (Priority 10, Manual)
- IMC_Journal (Priority 20, Manual)
- IMC_PhotoMode (Priority 30, Manual)
- IMC_Cinematic (Priority 100, Manual)
- IMC_AstraCommand (Priority 5, Manual)

Default Bindings: Set in DefaultInput.ini (already configured)
```

### 2.2 DA_PuzzleDefinition (Create one per puzzle)
```
Example: DA_Puzzle_Reactor_PowerRouting

PuzzleID: "Reactor_PowerRouting"
DisplayName: "Reactor Stabilization"
Type: PowerRouting
ZoneID: "ReactorCore"
RequiredVistaNodes: ["ReactorCore_A", "ReactorCore_B"]
PrerequisitePuzzles: []

Steps:
1. StepID: "GeneratorPhasing"
   Description: "Phase generators in sequence"
   ClueSourceID: "DLG_CLUE_SPARKS_GENERATOR_ORDER"
   SolutionData: { "Generator1": "Alpha", "Generator2": "Gamma", "Generator3": "Beta" }
   RequiredAstraCommand: Hold
   RewardFragmentID: "Fragment_2"

2. StepID: "DampenerSync"
   Description: "Synchronize dampeners at Control A and B"
   ClueSourceID: "DLG_CLUE_CORWIN_DAMPENER_SYNC"
   SolutionData: { "NodeA": "true", "NodeB": "true", "Window": "0.5" }
   RequiredAstraCommand: Hold
   RewardFragmentID: "Fragment_3"

3. StepID: "Calibration"
   Description: "Calibrate reactor frequency to 7.83 Hz"
   ClueSourceID: "DLG_CLUE_OSCILLOSCOPE_TARGET"
   SolutionData: { "Frequency": "7.83" }
   RequiredAstraCommand: Hold
   RewardFragmentID: ""

OrganUnlock: "ChronosCore"
PresentChanges:
- ChangeID: "Reactor_WaterDrain", Type: WaterLevel, TargetValue: 0.0, TransitionTime: 5.0
- ChangeID: "Reactor_BridgeExtend", Type: BridgeExtended, TargetValue: 1.0, TransitionTime: 3.0
- ChangeID: "Reactor_RadiationClear", Type: RadiationCleared, TargetValue: 1.0, TransitionTime: 2.0

HintTiers:
- Cost: 10, HintText: "DLG_HINT_REACTOR_1"
- Cost: 25, HintText: "DLG_HINT_REACTOR_2", HighlightActors: [GeneratorPanels]
- Cost: 50, HintText: "DLG_HINT_REACTOR_3"

EstimatedSolveTimeMinutes: 8
```

### 2.3 Organ Ability DataAssets (for GAS)
```
Create GameplayAbility Blueprints for each organ:

GA_ChronosCore_TimeDilation:
- AbilityTag: Ability.Organ.Chronos
- InputTag: Input.Organ.1
- Cooldown: 30s
- Cost: 50 Resonance (GameplayEffect)
- Activation: Spawn TimeDilationBubble actor at player location
- Effect: Custom TimeDilation (0.3x) in 500cm radius for 3s

GA_NeuralLoom_PastVision:
- AbilityTag: Ability.Organ.Neural
- InputTag: Input.Organ.2 (Toggle)
- Cooldown: 0s
- Cost: 10 Resonance/sec (GameplayEffect, duration)
- Activation: Enable PastVisionOverlay post-process material

GA_VoidSail_DoubleJumpGlide:
- AbilityTag: Ability.Organ.Void
- InputTag: Input.Organ.3 (Jump while airborne)
- Cooldown: 0s (charges based on air time)
- Activation: Grant double jump + glide movement mode

GA_GravityAnchor_WallRun:
- AbilityTag: Ability.Organ.Gravity
- InputTag: Input.Organ.4 (Auto on wall contact)
- Cooldown: 0s
- Activation: Enable wall movement mode when touching wall

GA_AstraCore_Synthesis:
- AbilityTag: Ability.Organ.Astra
- InputTag: None (cinematic only)
- Activation: Final sequence at Sub-Level 0
```

---

## 3. Astra Behavior Tree (BT_Astra)

### 3.1 Blackboard Keys
| Key | Type | Description |
|-----|------|-------------|
| TargetNode | Object | VistaNode to move to |
| CurrentState | Enum | Idle/Following/Waiting/PuzzleAssist/Scanning/Recalled/Memory |
| PlayerRef | Object | BP_Xylos reference |
| TrustLevel | Int | 0-10 |
| PuzzleAssistTarget | Object | Actor to Hold |

### 3.2 Behavior Tree Structure
```
Root (Selector)
├── Sequence: [HasPuzzleAssistTask?, MoveToTarget, ExecuteHold]
│   ├── Decorator: Blackboard (HasPuzzleAssistTask = true)
│   ├── Task: MoveTo (Target: PuzzleAssistTarget, AcceptanceRadius: 50)
│   └── Task: ExecuteHold (Interact with target, wait for completion)
├── Sequence: [PlayerIssuedFollow?, MoveToVistaNode, WaitAtNode]
│   ├── Decorator: Blackboard (CurrentState = Following)
│   ├── Task: MoveTo (Target: TargetNode, AcceptanceRadius: 100)
│   └── Task: WaitAtNode (Set State = WaitingAtNode, loop)
├── Sequence: [PlayerIssuedScan?, PlayScanVFX, RevealClue]
│   ├── Decorator: Blackboard (CurrentState = Scanning)
│   ├── Task: PlayScanVFX (Niagara at location)
│   └── Task: RevealClue (Call TimelineManager.RevealClueAtLocation)
├── Sequence: [PlayerIssuedWait?, IdleAtLocation]
│   ├── Decorator: Blackboard (CurrentState = Waiting)
│   └── Task: Wait (Set State = WaitingAtNode, loop)
└── Sequence: [PlayerIssuedRecall?, MoveToPlayer, IdleNearPlayer]
    ├── Decorator: Blackboard (CurrentState = Recalled)
    ├── Task: MoveTo (Target: PlayerRef, AcceptanceRadius: 50)
    └── Task: IdleNearPlayer (Set State = Idle, follow player)
```

### 3.3 Tasks (Blueprint)
```
Task_MoveToTarget:
- Get Blackboard TargetNode
- AI MoveTo (AcceptanceRadius: 50)
- OnSuccess: FinishExecute(true)
- OnFail: FinishExecute(false)

Task_ExecuteHold:
- Get Blackboard PuzzleAssistTarget
- Cast to BPI_Interactable
- Call Interact(Astra)
- Wait for completion (latent)
- FinishExecute(true)

Task_PlayScanVFX:
- Get Player location
- Spawn Niagara at location (NS_Astra_Scan)
- FinishExecute(true)

Task_WaitAtNode:
- Set Blackboard CurrentState = WaitingAtNode
- FinishExecute(true) (latent, re-evaluated each tick)
```

---

## 4. UI Widgets

### 4.1 WBP_HUD (Main HUD)
```
Canvas Panel:
- Top Left: Organ Ability Icons (4 slots, show cooldown/resonance cost)
- Top Center: Resonance Currency (number + icon)
- Top Right: Astra Trust (0-10 pips) + Memory Fragments (12 slots)
- Bottom Center: Context Prompts (Phase Shift, Interact, Astra Command)
- Bottom Left: Zone Name + Fast Travel Hint

Animations:
- OrganUnlock: Pulse + sound when new organ granted
- ResonanceGain: Number pop + particle
- TrustChange: Pip fill/empty animation
- PhaseShiftPrompt: Fade in/out with hold progress bar

Bindings:
- Organ icons: Bind to Player.ASC.HasAbility(OrganTag)
- Resonance: Bind to SaveSystem.ResonanceCurrency
- Trust: Bind to Astra.TrustLevel
- Prompts: Bind to Player.CurrentVistaNode / InteractableInRange / AstraCommandAvailable
```

### 4.2 WBP_Journal
```
Tabbed Interface:
- Tab 1: Narrative (chronological entries)
- Tab 2: Clues (puzzle-relevant, filterable by zone)
- Tab 3: Map (Node Network fast travel)
- Tab 4: Settings (accessibility, audio, graphics)

Narrative Entry Widget:
- Title (localized)
- Date/Timestamp
- Body text (localized, supports rich text)
- Audio log play button (if VO exists)
- Related Puzzles (links to clue tab)

Clue Widget:
- Puzzle Name
- Clue Text (localized)
- Zone Filter Dropdown
- Highlight Button (purchases Tier 2 hint)

Map Widget:
- Node Network diagram (SVG or Canvas render)
- Current location highlight
- Fast travel buttons (enabled zones)
- Zone info tooltip (puzzles remaining, organs)

Bindings:
- Entries: Bind to SaveSystem.AstraMemoryFragments + TrustLevel
- Clues: Bind to PuzzleManager.ActivePuzzles + PurchasedHints
- Map: Bind to TimelineManager.ActivatedVistaNodes + FastTravelUnlocked
```

### 4.3 WBP_AstraRadialMenu
```
Radial Menu (8 slots):
1. Scan (Top) - IA_Astra_Command + Up
2. Wait (Top-Right) - IA_Astra_Command + Right
3. Follow (Right) - IA_Astra_Command + Right
4. Recall (Bottom-Right) - IA_Astra_Command + Down
5. Hold (Bottom) - IA_Astra_Command + Down
6. Cancel (Center) - Close menu

Input:
- Gamepad: Hold Y → Radial appears → Stick direction selects → Release confirms
- Keyboard: Hold E → Radial appears → Mouse hover selects → Click confirms
- Accessibility: Click to open, click to select (hold not required)

Visual:
- Center: Astra avatar + Trust level
- Segments: Icon + Label + Cooldown/availability
- Disabled segments: Grayed out (not unlocked, on cooldown, no valid target)
```

### 4.4 WBP_PhotoMode
```
Free Camera Controls:
- WASD/Left Stick: Move
- Mouse/Right Stick: Rotate
- Q/E / L1/R1: Up/Down
- Scroll / D-Pad: FOV (30-120)
- 1-4: Organ ability visual toggles
- Tab: Hide UI

Post-Process Controls:
- Exposure: -2 to +2 EV
- Contrast: 0.5 to 2.0
- Saturation: 0 to 2.0
- Color Grading: LUT selection
- DOF: Focus Distance, F-Stop, Focal Length
- Bloom: Intensity, Threshold
- Vignette: Intensity
- Film Grain: Intensity

Export:
- Screenshot (4K PNG)
- High-Res Screenshot (8K, tiled render)
- Camera Transform Copy (for sharing)
```

---

## 5. Level Setup

### 5.1 Persistent Level (LVL_Persistent)
```
Actors:
- Landscape (Nanite enabled)
- Global Lighting (SkyAtmosphere, DirectionalLight, ExponentialHeightFog, VolumetricCloud)
- TimelineManager (spawned via GameInstance)
- PuzzleManager (component on GameMode)
- AstraCompanion (spawned at player start)
- PlayerStart (for BP_Xylos)

World Partition:
- Grid_Large (Architecture): CellSize 50000, LoadingRange 150000
- Grid_Medium (Props): CellSize 10000, LoadingRange 50000
- Grid_Small (Detail): CellSize 2500, LoadingRange 15000
```

### 5.2 Zone Levels (Streaming)
```
Present Levels (always loaded via World Partition):
- LVL_EntryHangar_Present
- LVL_ReactorCore_Present
- LVL_BioContainment_Present
- LVL_CommsArray_Present
- LVL_Hangar18_Present
- LVL_Administration_Present
- LVL_SubLevel0_Present

Past Levels (loaded additively on Phase Shift):
- Past_EntryHangar
- Past_ReactorCore
- Past_BioContainment
- Past_CommsArray
- Past_Hangar18
- Past_Administration
- Past_SubLevel0

Level Streaming Volumes:
- Each zone has a streaming volume covering its bounds
- Past levels: Initially unloaded, loaded via TimelineManager
- Use World Partition streaming distances, not manual volumes
```

### 5.3 Vista Node Placement (Per Zone)
```
Entry Hangar: 1 Node (Orientation)
Reactor Core: 2 Nodes (Control Room A, Control Room B)
Bio-Containment: 2 Nodes (Observation Deck, Calibration Lab)
Comms Array: 2 Nodes (Array Control, Signal Analysis)
Hangar 18: 2 Nodes (Stasis Bay, Extraction Pod)
Administration: 1 Node (Director's Office)
Sub-Level 0: 3 Nodes (Synthesis A, B, C)

Each Node:
1. Place BP_VistaNode at interaction point
2. Position CinematicCamera for puzzle framing
3. Adjust TriggerVolume to cover approach
4. Set NodeID, ZoneID, DisplayName
5. Assign PhaseShiftSequence (LevelSequence)
6. Set ConnectedNodes, UnlocksPuzzles
7. Set RequiredOrgan (if gated)
8. Test Phase Shift in PIE
```

---

## 6. Sequencer Setup (Phase Shift Transitions)

### 6.1 Master Phase Shift Sequence (Per Vista Node)
```
LevelSequence: LS_PhaseShift_ZoneID_NodeID

Tracks:
1. Camera Cuts:
   - 0.0s: ShoulderCam (Player CameraComponent)
   - 1.5s: CinematicCamera (VistaNode CineCameraComponent)
   - 3.0s: End (hold on CinematicCamera)

2. Post Process (Temporal Distortion):
   - Material: M_PhaseShift_Distortion
   - 0.0s: Intensity 0
   - 0.5s: Intensity 1 (peak)
   - 2.5s: Intensity 0
   - Parameters: DistortionStrength, ChromaticAberration, TimeDilation

3. Audio:
   - 0.0s: PhaseShift_Start (spatial, at player)
   - 0.5s: PhaseShift_Buildup (2D, rising pitch)
   - 1.5s: PhaseShift_Transition (impact)
   - 2.5s: PhaseShift_Settle (ambient past timeline)

4. Visibility:
   - Present Actors: 0.0s Visible → 1.5s Hidden
   - Past Actors: 0.0s Hidden → 1.5s Visible

5. Event Track:
   - 1.5s: "OnPhaseShiftMidpoint" → TimelineManager.OnPastLevelLoaded
   - 3.0s: "OnPhaseShiftComplete" → TimelineManager.OnPhaseShiftComplete
```

### 6.2 Accessibility Variants
```
LS_PhaseShift_ZoneID_NodeID_InstantCut:
- 0.0s: Hard cut to CinematicCamera
- No post-process distortion
- Audio: Single PhaseShift_Transition
- Duration: 0.1s

LS_PhaseShift_ZoneID_NodeID_ReducedMotion:
- 1.5s blend (no distortion)
- No chromatic aberration
- Reduced audio intensity
- Duration: 1.5s
```

---

## 7. Testing Checklist

### 7.1 Per Sprint (Definition of Done)
```
Code:
- [ ] Compiles without warnings
- [ ] CI passes (GitHub Actions)
- [ ] Code reviewed

Art:
- [ ] QA gate passes (qa_check.py)
- [ ] Texel density ±10%
- [ ] USD export verified in UE5
- [ ] Material Layers work (Present/Past)

Design:
- [ ] DA_PuzzleDefinition complete
- [ ] Playtested internally
- [ ] Accessibility implemented

Narrative:
- [ ] Yarn compiles
- [ ] VO placeholder recorded
- [ ] Loc keys follow convention
```

### 7.2 M0 Gate (End of Sprint 2)
```
- [ ] Project compiles, runs in PIE
- [ ] Xylos traversal feels responsive (coyote/buffer/mantle verified)
- [ ] Phase Shift: Present → Past → Present completes without error
- [ ] Past puzzle solve triggers Present change (bridge extends)
- [ ] Blender asset exports to UE5 with correct scale, normals, materials
- [ ] Git LFS configured, CI passing
- [ ] Save/Load works across Phase Shift
```

---

## 8. Console Commands for Testing

```
# Timeline
Chronos51.DumpZoneStates
Chronos51.ForcePhaseShift ReactorCore_A
Chronos51.ToggleTimeline
Chronos51.SetTrust 10
Chronos51.GiveAllOrgans

# Puzzles
Chronos51.CompletePuzzle Reactor_PowerRouting
Chronos51.ResetPuzzle Reactor_PowerRouting
Chronos51.ShowPuzzleState Reactor_PowerRouting

# Save
Chronos51.SaveGame 1
Chronos51.LoadGame 1
Chronos51.ListSaves

# Debug
stat gpu
stat game
stat streaming
stat nanite
stat lumen
r.Nanite.Visualize 1
r.Lumen.Visualize 1
```

---

## 9. Next Steps After Setup

1. **Create all DataAssets** (InputConfig, PuzzleDefinitions, Organ Abilities)
2. **Build Behavior Tree** for Astra
3. **Create UI Widgets** (HUD, Journal, Radial Menu, Photo Mode)
4. **Set up Levels** (Persistent + Zone streaming)
5. **Place Vista Nodes** in Reactor Core (M1 vertical slice)
6. **Create Phase Shift Sequences** for each Vista Node
7. **Implement Yarn Dialogue** for Act 1
8. **Playtest Sprint 1-2 deliverables**

---

*Last Updated: 2026-11-13 | UE5.3 | Chronos51 Studios*