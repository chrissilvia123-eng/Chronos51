# Chronos 51 — Core Systems Design

## 1. Phase Shift System (The Core Mechanic)

### 1.1 Design Goals
- Seamless transition between Present (traversal) and Past (puzzles)
- No loading screens visible to player (async streaming)
- Clear visual language: golden temporal energy = past→present causality
- Accessibility: Instant cut option, reduced motion, Phase Shift skip

### 1.2 Player Flow
```
Present Traversal
    ↓ Approach Vista Node
[Prompt: "Hold PHASE to shift" + Vista Node hologram]
    ↓ Hold Phase (1.5s charge, cancelable)
[Camera blend: Shoulder → Fixed Cinematic (1.5s ease-in-out)]
    ↓ Past Timeline Loads (additive streaming)
Fixed Camera Puzzle Mode
    ↓ Solve Puzzle Chain
[Organ Restored → Present Changes Manifest]
    ↓ Auto-return or Manual "Return"
[Camera blend: Fixed → Shoulder (1.5s)]
Present Traversal (new path open)
```

### 1.3 Vista Node Data Structure
```yaml
# DataAsset: DA_VistaNode
nodeID: "ReactorCore_A"
displayName: "Reactor Control Room Alpha"
zoneID: "ReactorCore"
timeline: "Past" # or "Present" for present-only nodes
cinematicCamera: "CineCam_ReactorCore_A" # CineCameraActor reference
connectedNodes: ["ReactorCore_B", "BioContainment_A"]
unlocksPuzzles: ["Reactor_PowerRouting", "Reactor_Sync"]
requiredOrgan: "None" # or OrganID if gated
phaseShiftDuration: 3.0 # seconds
accessibility:
  instantCut: true
  reducedMotion: true
  skipOption: true
```

### 1.4 Technical Implementation (UE5)
- **BP_VistaNode**: Actor with BoxComponent trigger, CineCameraComponent, spline for multi-angle
- **TimelineManager (GameInstanceSubsystem)**: Manages CurrentTimeline enum, ZoneStates map, async level streaming
- **PhaseShiftTransition**: Level Sequence with camera blend, post-process material (temporal distortion), audio sweep
- **PastPawn**: Stripped traversal pawn (puzzle interaction only, no jump/mantle)
- **State Persistence**: ZoneState DataAsset applied on present load via WorldSubsystem

---

## 2. Traversal System (Present Timeline)

### 2.1 Movement Feel Targets (per platformer skill)
| Parameter | Value | Source |
|-----------|-------|--------|
| Walk Speed | 1.8 m/s | Comfortable exploration |
| Run Speed | 4.5 m/s | Urgent but controlled |
| Accel Time | 0.1s | Snappy response |
| Jump Height | 2.2m | ~3.5 tiles (UE5 scale) |
| Time to Apex | 0.35s | Weighty, not floaty |
| Fall Gravity | 1.8× rise | Snappy descent |
| Coyote Time | 0.1s | 6 frames @ 60fps |
| Jump Buffer | 0.12s | 7 frames @ 60fps |
| Variable Jump | 0.45× on release | Tap = short hop |
| Mantle Height | ≤ 1.5m | Auto-mantle ledges |
| Climb Speed | 2.5 m/s | Ladders/vines/cables |

### 2.2 Organ Abilities (Unlock Order)
| Organ | Ability | GAS Tags | Input | Traversal Impact |
|-------|---------|----------|-------|------------------|
| Chronos Core | TimeDilationBubble (local 0.3×, 3s) | Ability.Organ.Chronos | L1/LB hold | Slow steam, time puzzle sync |
| Neural Loom | PastVisionOverlay (shader) | Ability.Organ.Neural | R1/RB toggle | See past state in present |
| Void Sail | DoubleJump + Glide | Ability.Organ.Void | Jump (air) | Cross gaps, reach heights |
| Gravity Anchor | WallRun / CeilingHang | Ability.Organ.Gravity | Auto | Vertical traversal, new paths |

### 2.3 Camera System (per camera-systems skill)
- **Shoulder Camera**: SpringArm with deadzone (40×30cm), look-ahead 2m, exponential smoothing (rate 8)
- **Bounds Clamping**: Level streaming volume bounds, eased stop at edges
- **Transition Blend**: Sequencer-driven, fixed timestep, input disabled during blend
- **Photo Mode**: Free camera, DOF, exposure, time-of-day scrubber

---

## 3. Puzzle Framework (Past Timeline)

### 3.1 Puzzle Design Principles (per puzzle + level-design skills)
1. **Single Mechanic Focus** — Each puzzle chain teaches one core logic (Braid principle)
2. **Observation-Based** — Clues in environment, NPC routines, audio logs, documents
3. **No Time Pressure** — Turn-based logic, player controls pace (Deathloop QoL)
4. **Cross-Timeline Causality** — Past solution → Present physical change (Outer Wilds knowledge gate)
5. **Astra Coordination** — Gamespeak commands for multi-node sync puzzles

### 3.2 Puzzle Types & Examples
| Type | Core Logic | Past Example | Present Result |
|------|------------|--------------|----------------|
| Power Routing | Graph connection | Connect 3 generators in sequence | Reactor stabilizes, water drains |
| Sequence Alignment | Rotary/linear matching | Calibrate oscilloscope to 432.5Hz | Neural Loom frequency unlocked |
| Temporal Sync | Multi-node simultaneous | Astra holds switch B, player triggers A | Bridge extends in present |
| Waveform Matching | Audio-visual pattern | Match tone from Vance's hum | Bio-containment field drops |
| Archive Decryption | Cross-reference clues | Combine log + chart + specimen label | Access code for Hangar 18 |

### 3.3 Puzzle Data Structure (DataAsset: DA_PuzzleDefinition)
```yaml
puzzleID: "Reactor_PowerRouting"
displayName: "Reactor Stabilization"
type: "PowerRouting"
zoneID: "ReactorCore"
vistaNodes: ["ReactorCore_A", "ReactorCore_B"]
prerequisites: [] # PuzzleIDs that must be solved first
steps:
  - stepID: "GeneratorPhasing"
    description: "Phase generators Alpha, Gamma, Beta in order"
    clueSource: "Sparks_OMalley_Log_03" # StringTable ID
    solution: ["Alpha", "Gamma", "Beta"]
    astraCommand: "Hold" # Gamespeak command used
    reward: "ChronosCore_Fragment_1"
  - stepID: "DampenerSync"
    description: "Sync dampeners at Node A and B within 0.5s"
    clueSource: "Corwin_Log_Final"
    solution: { nodeA: true, nodeB: true, window: 0.5 }
    astraCommand: "Hold" # Astra holds Node B
    reward: "ChronosCore_Fragment_2"
organUnlock: "ChronosCore"
presentChanges:
  - zoneID: "ReactorCore"
    waterLevel: 0.0
    radiationCleared: true
    bridgeExtended: true
hintTiers:
  - cost: 10 # Resonance currency
    text: "DLG_HINT_REACTOR_1" # Sparks mentions the order
  - cost: 25
    text: "DLG_HINT_REACTOR_2" # Visual highlight on generator panels
  - cost: 50
    text: "DLG_HINT_REACTOR_3" # Full solution revealed
```

### 3.4 Teaching Progression (per level-design pacing)
| Zone | New Mechanic | Puzzle Count | Complexity |
|------|--------------|--------------|------------|
| Entry Hangar | Phase Shift basics | 1 | Single room, direct cause→effect |
| Reactor Core | Power routing, Astra sync | 3 | Multi-room, temporal window |
| Bio-Containment | Waveform matching | 2 | Audio-visual, frequency tuning |
| Comms Array | Signal propagation | 2 | Facility-wide, multi-node |
| Hangar 18 | All combined + traversal | 3 | Mastery, fluid timeline switching |
| Sub-Level 0 | Synthesis (all organs) | 1 | Simultaneous 3-node coordination |

---

## 4. Companion System (Astra)

### 4.1 Gamespeak-Lite Commands
| Command | Function | Context | Cooldown |
|---------|----------|---------|----------|
| **Scan** | Activate PastVisionOverlay at current location | Present, near clue | 5s |
| **Wait** | Stay at current position | Past, puzzle assist | — |
| **Follow** | Travel to target Vista Node | Past, between puzzles | — |
| **Recall** | Return to player immediately | Any | 10s |
| **Hold** | Activate/interact with targeted object | Past, sync puzzles | — |

### 4.2 Behavior Tree States
```
Selector
  ├── Sequence: [HasPuzzleAssistTask?, MoveToTarget, ExecuteHold]
  ├── Sequence: [PlayerIssuedFollow?, MoveToVistaNode, IdleAtNode]
  ├── Sequence: [PlayerIssuedScan?, PlayScanVFX, RevealClue]
  ├── Sequence: [PlayerIssuedWait?, IdleAtLocation]
  └── Sequence: [PlayerIssuedRecall?, MoveToPlayer, IdleNearPlayer]
```

### 4.3 Trust & Memory System
- **Trust (0-10)**: Increases with puzzle solves, Astra dialogue choices, Scan usage
- **Memory Fragments (12 total)**: Recovered via Neural Loom puzzles, each restores Astra dialogue
- **Ending Variants**: Determined by Trust + Fragments at finale

---

## 5. Save System (Dual-Timeline)

### 5.1 Save Schema (Versioned JSON)
```json
{
  "version": 5,
  "slot": 0,
  "timestamp": "2026-11-13T07:30:00Z",
  "playtime": 7245.3,
  "currentTimeline": "Present",
  "currentZone": "ReactorCore",
  "playerTransform": { "location": [0,0,0], "rotation": [0,45,0] },
  "activatedVistaNodes": ["EntryHangar_A", "ReactorCore_A", "ReactorCore_B"],
  "fastTravelUnlocked": ["EntryHangar", "ReactorCore"],
  "organsRestored": ["ChronosCore", "NeuralLoom"],
  "activeAbilities": ["TimeDilationBubble", "PastVisionOverlay"],
  "puzzleStates": {
    "Reactor_PowerRouting": { "step": 3, "sequence": ["Alpha","Gamma","Beta"], "solved": true },
    "Reactor_Sync": { "nodeA_triggered": true, "nodeB_triggered": true, "solved": true }
  },
  "zoneStates": {
    "ReactorCore": { "waterLevel": 0, "bridgeExtended": true, "radiationCleared": true }
  },
  "astraTrust": 4,
  "astraMemoryFragments": [1, 2, 3, 5, 7],
  "resonanceCurrency": 142,
  "settings": { "masterVolume": 0.8, "subtitles": true, "reducedMotion": false }
}
```

### 5.2 Atomic Write Pattern (per save-systems skill)
- Serialize to `Save_Slot_X.json.tmp`
- `rename` over `Save_Slot_X.json`
- Keep `.bak` of previous save
- Migration chain: v1→v2→v3→v4→v5
- Autosave on: Vista Node activation, Puzzle solve, Organ restore, Zone transition

---

## 6. Input Mapping (Enhanced Input)

### 6.1 Action Map: IMC_Traversal
| Action | Type | Keyboard | Gamepad | Notes |
|--------|------|----------|---------|-------|
| IA_Move | Axis2D | WASD | LStick | |
| IA_Look | Axis2D | Mouse | RStick | |
| IA_Jump | Digital | Space | A | Edge trigger |
| IA_Mantle | Digital | Space (near ledge) | A (near ledge) | Context-sensitive |
| IA_Sprint | Digital | LShift | L3 | Hold |
| IA_Crouch | Digital | LCtrl | B | Toggle |
| IA_Phase | Digital (hold) | Q | L2/LT | Vista Node only |
| IA_Organ_1 | Digital | 1 | D-Pad Up | Chronos Core |
| IA_Organ_2 | Digital | 2 | D-Pad Right | Neural Loom |
| IA_Organ_3 | Digital | 3 | D-Pad Down | Void Sail |
| IA_Organ_4 | Digital | 4 | D-Pad Left | Gravity Anchor |
| IA_Astra_Command | Digital | E | Y | Context radial menu |
| IA_Interact | Digital | F | X | Context-sensitive |
| IA_Journal | Digital | J | View | Toggle journal UI |
| IA_PhotoMode | Digital | F12 | — | |

### 6.2 Context Mapping Contexts
- **IMC_Traversal**: Default (Present + Past traversal)
- **IMC_PuzzlePanel**: Modal puzzle UI (dials, waveform, keypad)
- **IMC_Journal**: Journal navigation (gamepad focus)
- **IMC_PhotoMode**: Free camera controls
- **IMC_Cinematic**: Input disabled (Phase Shift blend, cutscenes)

---

## 7. Audio Design (per audio-design skill)

### 7.1 Bus Structure
| Bus | Purpose | Ducking |
|-----|---------|---------|
| Master | Final mix | — |
| Music | Adaptive layers (Ambient/Tension/Resolve) | VO (-18dB), UI (-6dB) |
| Ambience | Node-specific loops (wind, machinery, bioluminescence) | Crossfade 2s on node transition |
| SFX | Interactions, mechanisms, footsteps, abilities | — |
| UI | Menu clicks, journal page turn, hints | Music (-6dB) |
| VO | Astra narration, NPC logs, Thorne echo | Music (-18dB), Ambience (-12dB) |

### 7.2 Adaptive Music System
- **Per Zone**: 3-layer stems (Base / Tension / Resolve)
- **Intensity Driver**: Puzzle proximity + unsolved puzzle count
- **Transition**: Crossfade 3s, triggered by ZoneState changes
- **Past Timeline**: Period-appropriate score (1940s-60s orchestral/electronic hybrid)

---

## 8. Accessibility (Built-in from M0)
- **Full Rebinding**: All actions, gamepad/keyboard/mouse
- **Hold vs Toggle**: Sprint, Crouch, Scan, PastVision
- **Reduced Motion**: Instant Phase Shift cuts, disable camera shake, disable screen distortion
- **Color Blind**: Deuteranope/Protanope/Tritanope palettes for puzzle UI, Astra highlight
- **Subtitles**: All dialogue, speaker labels, sound effect descriptions
- **Text Scaling**: 0.8x – 2.0x UI text
- **Phase Shift Skip**: Auto-solve current puzzle chain (story mode)
- **Hint System**: Progressive hints for Resonance currency
- **No Fail States**: No death, no game over, no time pressure