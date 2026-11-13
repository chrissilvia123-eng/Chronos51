# Chronos 51 — UE5 Project

## Quick Start

### Prerequisites
- Unreal Engine 5.3+
- Visual Studio 2022 (with Game Development with C++ workload)
- Git LFS
- Blender 5.3+ (for asset pipeline)

### Opening the Project
1. Open `E:\Chronos51\05_UE5_Project\Chronos51\Chronos51.uproject`
2. If prompted, select "Open" (not "Convert in Place")
3. Wait for shader compilation and project generation

### Building from Command Line
```batch
cd E:\Chronos51\05_UE5_Project\Chronos51
"C:\Program Files\Epic Games\UE_5.3\Engine\Build\BatchFiles\Build.bat" Chronos51Editor Win64 Development -Project="Chronos51.uproject"
```

### Running Tests
```batch
"C:\Program Files\Epic Games\UE_5.3\Engine\Binaries\Win64\UnrealEditor-Cmd.exe" "Chronos51.uproject" -run=AutomationTest -TestNames="Chronos51.*" -unattended -nullrhi -nosound
```

---

## Project Structure

```
Source/Chronos51/
├── Public/                    # Header files (.h)
│   ├── Chronos51GameModeBase.h
│   ├── Chronos51Character.h
│   ├── TimelineManager.h
│   ├── SaveSystem.h
│   ├── PuzzleManager.h
│   ├── DA_PuzzleDefinition.h
│   ├── VistaNode.h
│   └── InputConfig.h
├── Private/                   # Implementation files (.cpp)
│   ├── Chronos51GameModeBase.cpp
│   ├── Chronos51Character.cpp
│   ├── TimelineManager.cpp
│   ├── SaveSystem.cpp
│   ├── PuzzleManager.cpp
│   └── VistaNode.cpp
├── Chronos51.Build.cs         # Module build configuration
└── Chronos51.h/.cpp           # Module entry point
```

---

## Core Systems Overview

### 1. TimelineManager (GameInstanceSubsystem)
- Manages dual-timeline state (Present/Past/Transitioning)
- Handles Phase Shift transitions with async level streaming
- Persists ZoneState changes across timelines
- Controls fast travel network

### 2. Chronos51Character
- GAS-integrated character with AbilitySystemComponent
- Movement feel: Coyote time (0.1s), Jump buffer (0.12s), Variable jump (0.45×)
- Mantle detection (≤1.5m ledges)
- Asymmetric gravity (fall 1.8× rise)
- Organ abilities granted via GAS

### 3. Phase Shift System
```
Present Traversal
    ↓ Approach Vista Node
[Prompt: Hold PHASE]
    ↓ Camera blend (1.5s ease-in-out)
[Async load Past sub-level]
    ↓ PastPawn spawn + CineCamera activate
Fixed Camera Puzzle Mode
    ↓ Solve Puzzle Chain
[Organ Restored → ZoneState changes]
    ↓ Auto-return or Manual
[Camera blend back]
Present Traversal (new paths open)
```

### 4. Puzzle System
- DataAsset-driven: `DA_PuzzleDefinition`
- 5 types: PowerRouting, SequenceAlignment, TemporalSync, WaveformMatching, ArchiveDecryption
- Astra Gamespeak integration: Scan, Wait, Follow, Recall, Hold
- Hint system (3 tiers, Resonance currency)
- Cross-timeline causality: Past solve → Present change

### 5. Save System
- Atomic write (tmp → rename → .bak)
- Versioned migration chain (v1→v5)
- Autosave on: Vista Node activation, Puzzle solve, Organ restore, Zone transition
- Slots: 4 manual + 1 autosave

---

## Input Mapping (Enhanced Input)

| Action | Type | Keyboard | Gamepad | Context |
|--------|------|----------|---------|---------|
| Move | Axis2D | WASD | LStick | Traversal |
| Look | Axis2D | Mouse | RStick | Traversal |
| Jump | Digital | Space | A | Traversal |
| Mantle | Digital | Space (ctx) | A (ctx) | Traversal |
| Sprint | Digital | LShift | L3 | Traversal |
| Crouch | Digital | LCtrl | B | Traversal |
| Phase | Hold | Q | L2/LT | Traversal (Vista Node) |
| Organ 1-4 | Digital | 1-4 | D-Pad | Traversal |
| Astra Command | Digital | E | Y | AstraCommand |
| Interact | Digital | F | X | Traversal |
| Journal | Digital | J | View | Journal |
| PhotoMode | Digital | F12 | — | PhotoMode |
| Pause | Digital | Esc | Start | Cinematic |

---

## Development Workflow

### Sprint Cadence (2 weeks)
- **Monday**: Sprint Planning
- **Tue-Thu**: Deep Work
- **Friday**: Sprint Review + Retrospective

### Definition of Done

**Code:**
- [ ] Compiles without warnings
- [ ] CI passes (GitHub Actions)
- [ ] Code reviewed (PR approved)
- [ ] No hardcoded paths/magic numbers

**Art:**
- [ ] Passes `qa_check.py --profile game`
- [ ] Texel density ±10% target
- [ ] USD export verified in UE5
- [ ] Material Layers work (Present/Past)

**Design:**
- [ ] DA_PuzzleDefinition complete
- [ ] Playtested internally
- [ ] Accessibility implemented

**Narrative:**
- [ ] Yarn compiles
- [ ] VO placeholder recorded
- [ ] Loc keys follow convention

---

## Key Configuration Files

| File | Purpose |
|------|---------|
| `Config/DefaultEngine.ini` | Nanite, Lumen, World Partition, Physics, Rendering |
| `Config/DefaultInput.ini` | Enhanced Input actions, mappings, contexts |
| `Config/DefaultGame.ini` | Gameplay settings, default maps, game mode |
| `.github/workflows/ci-cd.yml` | CI/CD pipeline |

---

## Asset Pipeline (Blender → UE5)

```
Blender 5.3
    ↓ High-poly sculpt/sub-d
    ↓ Auto-retopo (Quadriflow)
    ↓ UV pack (seams from sharp, 16px margins)
    ↓ Bake maps (Normal, AO, Curvature, ID + ORM pack, DirectX)
    ↓ Material Layers (Base + Damage + Moss + Rust + Clean via vertex paint)
    ↓ Export USD (Nanite-ready)
    ↓ UE5 Import (Nanite + Lumen Mesh Cards + Virtual Textures)
```

### Naming Conventions
| Type | Prefix | Example |
|------|--------|---------|
| Static Mesh (Nanite) | SM_ | SM_ReactorCore_Console |
| Skeletal Mesh | SK_ | SK_Xylos |
| Camera | CAM_ | CAM_Vista_ReactorCore_A |
| Material | M_ | M_Master_Facility |
| Texture | T_ | T_ReactorCore_Console_BASE |
| Blueprint | BP_ | BP_VistaNode |
| DataAsset | DA_ | DA_PuzzleDefinition |
| Level | LVL_ | LVL_ReactorCore_Present |

---

## Performance Targets

| Platform | Resolution | Target | GPU Budget |
|----------|------------|--------|------------|
| PC High | 1440p | 60 FPS | RTX 3070 / RX 6700 XT |
| PC Ultra | 4K | 60 FPS | RTX 4080 / RX 7900 XTX |
| Console | 4K | 30 FPS | PS5 / Xbox Series X |

**Frame Budget (16.67ms @ 60fps):**
- Base Pass (Nanite): 4-6ms
- Lumen GI: 3-5ms
- Lumen Reflections: 1-2ms
- Virtual Shadow Maps: 2-3ms
- Niagara VFX: 1-2ms
- Gameplay/Logic: 1-2ms
- UI/PostProcess: 1ms

---

## Debug Commands (Console)

```console
# Timeline debugging
Chronos51.DumpZoneStates
Chronos51.ForcePhaseShift <NodeID>
Chronos51.ToggleTimeline

# Puzzle debugging
Chronos51.CompletePuzzle <PuzzleID>
Chronos51.ResetPuzzle <PuzzleID>
Chronos51.ShowPuzzleState <PuzzleID>

# Save debugging
Chronos51.SaveGame <Slot>
Chronos51.LoadGame <Slot>
Chronos51.ListSaves

# Performance
stat gpu
stat game
stat streaming
stat nanite
stat lumen
r.Nanite.Visualize 1
r.Lumen.Visualize 1
```

---

## Next Steps for Development

1. **Week 1 (Sprint 1):**
   - [ ] Verify project opens and compiles
   - [ ] Test Blender→UE5 USD pipeline
   - [ ] Implement traversal feel (coyote/buffer/mantle)
   - [ ] Set up Enhanced Input in BP

2. **Week 2 (Sprint 1 continued):**
   - [ ] Phase Shift prototype (1 Vista Node)
   - [ ] Dual-timeline state sync
   - [ ] Save/Load across Phase Shift

3. **Week 3-4 (Sprint 2):**
   - [ ] Reactor Core blockout
   - [ ] First puzzle chain implementation
   - [ ] M0 Gate: Full loop playable

---

## Support Files Location

| Category | Path |
|----------|------|
| Game Design Docs | `E:\Chronos51\01_GameDesign\` |
| Narrative/Yarn | `E:\Chronos51\01_GameDesign\02_Narrative\Yarn\` |
| Level Design | `E:\Chronos51\01_GameDesign\03_LevelDesign\` |
| Puzzles | `E:\Chronos51\01_GameDesign\04_Puzzles\` |
| Production Schedule | `E:\Chronos51\03_Production\01_Schedule\` |
| Technical Architecture | `E:\Chronos51\04_Technical\01_Architecture\` |
| Blender Pipeline | `E:\Chronos51\06_Blender_Pipeline\` |
| Reference | `E:\Chronos51\07_Reference\` |

---

*Last Updated: 2026-11-13 | UE5.3 | Chronos51 Studios*