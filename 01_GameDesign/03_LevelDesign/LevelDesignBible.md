# Chronos 51 — Level Design Bible

## 1. Dual-Timeline Level Architecture

### 1.1 Zone Structure (Present ↔ Past)
Each zone exists in two states loaded additively:

| Zone | Present (2024) | Past (1947-1962) | Vista Nodes | Organ |
|------|----------------|------------------|-------------|-------|
| **Entry Hangar** | Sand dunes, collapsed roof, broken catwalks | Active launch bay, X-15 tests, Vespera crash | 1 (Orientation) | — |
| **Reactor Core** | Flooded, irradiated, pipes ruptured | Zero-point experiment, critical test | 2 (Control A, Control B) | Chronos Core |
| **Bio-Containment** | Overgrown, shattered jars, vines | Xenobiology lab, Xylos studied | 2 (Observation, Calibration) | Neural Loom |
| **Comms Array** | Fallen dishes, cable bridges | Deep space signal, first contact | 2 (Array Control, Signal Analysis) | Void Sail |
| **Hangar 18** | Doors welded, interior crushed | Vespera in stasis, Astra extracted | 2 (Stasis Bay, Extraction) | Gravity Anchor |
| **Administration** | Records burned, safe open | Directors' debate, Thorne's office | 1 (Director's Office) | — |
| **Sub-Level 0** | Collapsed, non-Euclidean (requires all organs) | Fracture birth moment, Astra's prison | 3 (Synthesis Nodes) | Astra's Core |

### 1.2 Vista Node Placement Rules
1. **One per major puzzle chain** (2 per zone for multi-room puzzles)
2. **Line of sight to puzzle objective** — camera frames the puzzle space
3. **Distinct visual identity** — unique lighting, architecture, color palette
4. **Fast travel hub** — activated nodes connect via Node Network
5. **Accessibility** — reachable via traversal abilities available at that point

### 1.3 Node Network Map
```
[Entry Hangar] ←→ [Reactor Core] ←→ [Bio-Containment]
                      ↓                    ↓
                [Administration] ←→ [Comms Array]
                      ↓                    ↓
                [Hangar 18] ←←←←←←←←←←←←←←←
                      ↓
                [Sub-Level 0] (All 4 Organs + Astra's Core)
```

**Gating Logic:**
- Physical paths open in Present only after Past puzzle solves
- Fast travel unlocks per node activation
- Sub-Level 0 requires all 4 Organs + 8+ Memory Fragments

---

## 2. Present Timeline Traversal Design (per platformer + level-design skills)

### 2.1 Metrics-Driven Layout
| Metric | Value | Application |
|--------|-------|-------------|
| Max Jump Distance | 3.5m | Gap between platforms |
| Max Mantle Height | 1.5m | Ledge placement |
| Safe Fall Distance | 8m | Drop placement without damage |
| Climbing Verticality | 12m per segment | Vine/cable/ladder runs |
| Glide Ratio (Void Sail) | 2:1 horizontal:vertical | Gap crossing design |
| Time Bubble Duration | 3s | Steam/puzzle timing windows |

### 2.2 Zone Traversal Flow (Reactor Core Example)
```
ENTRY (from Entry Hangar)
    ↓
[FLOODED CORRIDOR] → Mantle pipes → Time-slow steam burst → Glide gap
    ↓
[COLLAPSED CONTROL ROOM A] → Vista Node A (Phase Shift)
    ↓ (Present: Water drained, bridge extended after puzzle)
[BRIDGE TO CONTROL ROOM B] → Vista Node B (Phase Shift)
    ↓ (Present: Radiation cleared, elevator operational)
[REACTOR CORE CHAMBER] → Chronos Core cradle → Organ restoration
    ↓
[ELEVATOR TO BIO-CONTAINMENT] → Zone transition
```

### 2.3 Environmental Storytelling Beats (Present)
| Zone | Beat | Delivery Method |
|------|------|-----------------|
| Entry Hangar | Crash impact | Scorch pattern, Vespera silhouette in sand, Astra's first words |
| Reactor Core | Containment failure | Water level marks, radiation warning signs, Sparks' rabbit's foot |
| Bio-Containment | Specimen escape | Broken containment tubes, vine growth patterns, Vance's notebook |
| Comms Array | Signal sent | Dish alignment frozen mid-rotation, Twins' headsets on console |
| Hangar 18 | Extraction | Empty stasis cradle, neural interface cables severed, Astra's resonance |
| Administration | Cover-up | Burned files, Corwin's stopped watch, Thorne's empty chair |
| Sub-Level 0 | Fracture origin | Non-Euclidean geometry, temporal loops, Thorne's echo |

---

## 3. Past Timeline Puzzle Room Design (per puzzle skill)

### 3.1 Room Template (Fixed Camera)
```
[FIXED CINEMATIC CAMERA]
    ↓
[PUZZLE SPACE - 15x15m typical]
    ├── Interactive Elements (generators, dials, panels, waveforms)
    ├── NPC Positions (idle routines, clue delivery)
    ├── Environmental Clues (charts, logs, gauges, audio)
    └── Astra Anchor Point (Gamespeak command target)
```

### 3.2 Puzzle Room: Reactor Control Room A (Power Routing)
**Camera:** CineCam_ReactorCore_A — low angle, reactor core visible through window
**Interactables:**
- Generator Alpha Panel (left wall)
- Generator Gamma Panel (center)
- Generator Beta Panel (right wall)
- Master Sync Console (foreground)
**NPCs:** Sparks (at console), Corwin (pacing behind)
**Clues:**
- Sparks' mutter: "Alpha, Gamma, Beta... never Delta"
- Wall chart: "AUX GENERATOR PHASING SEQUENCE" (redacted)
- Radio log: Corwin's order "Alpha. Gamma. Beta. Never Delta."
**Astra Target:** Master Sync Console (Hold command)

### 3.3 Puzzle Room: Bio-Containment Observation (Waveform Matching)
**Camera:** CineCam_BioContainment_A — through observation glass, Xylos in containment visible
**Interactables:**
- Frequency Dial (0-1000 Hz)
- Amplitude Slider (0-1.0)
- Oscilloscope Display (target waveform)
- Calibration Button
**NPCs:** Dr. Vance (at console, humming), Colleague (urgent)
**Clues:**
- Vance hums 432.5 Hz (audio cue)
- Notebook: "Resonance matches neural oscillation — 432.5 Hz"
- Colleague: "Thorne wants dissection tomorrow. We calibrate tonight."
**Astra Target:** Calibration Button (Hold command)

### 3.4 Puzzle Room: Comms Array Signal Analysis (Multi-Node Sync)
**Camera:** CineCam_CommsArray_A — wide, dish array visible through windows
**Interactables:**
- Dish Alignment Controls (azimuth/elevation)
- Signal Frequency Tuner
- Transmission Button
- Recording Playback (The Reply)
**NPCs:** Maria Volkov (left console), Mikhail Volkov (right console)
**Clues:**
- Twins' overlapping dialogue: "Azimuth 142.5... Elevation -12... Frequency matches..."
- Recording: Deep space signal — coordinates encoded
- Wall map: Star chart with highlighted system
**Astra Target:** Transmission Button (Hold command — syncs with CommsArray_B)

---

## 4. Cross-Timeline Causality Design

### 4.1 Past → Present Change Types
| Past Action | Present Manifestation | Visual Language |
|-------------|----------------------|-----------------|
| Close valve/reroute pipe | Flooded room drains | Water recedes with golden shimmer |
| Calibrate machine | Broken machine repairs | Parts assemble with temporal particles |
| Extend bridge/activate platform | Gap bridged | Bridge extends from both sides, golden energy |
| Stabilize reactor | Radiation clears | Green glow fades, air shimmers clean |
| Align signal dish | Cable bridge forms | Cables tense, glow, become walkable |
| Extract neural pattern | PastVision overlay unlocks | UI shader activates, golden scan lines |

### 4.2 Visual Feedback System
- **Golden Temporal Energy** = Past action → Present result (consistent color language)
- **Phase Particles** = Transition moments, organ restoration
- **Resonance Highlight** = Neural Loom scan reveals past state in present
- **Memory Echo** = Audio/visual flash at organ restoration

---

## 5. Pacing & Teaching Curve (per level-design pacing-and-flow.md)

### 5.1 Beat Intensity Timeline
```gdscript
# Per zone: Tension/Rest curve
const ZONE_BEATS = {
    "EntryHangar": [
        {"beat": "Awakening", "type": "teach", "intensity": 0.1},
        {"beat": "FirstPhase", "type": "reveal", "intensity": 0.4},
        {"beat": "Orientation", "type": "teach", "intensity": 0.2},
        {"beat": "Departure", "type": "rest", "intensity": 0.1}
    ],
    "ReactorCore": [
        {"beat": "Approach", "type": "traversal", "intensity": 0.3},
        {"beat": "PhaseIn_A", "type": "puzzle", "intensity": 0.5},
        {"beat": "PowerRouting", "type": "puzzle", "intensity": 0.6},
        {"beat": "PhaseIn_B", "type": "puzzle", "intensity": 0.7},
        {"beat": "Sync", "type": "climax", "intensity": 0.9},
        {"beat": "OrganRestored", "type": "reward", "intensity": 0.4},
        {"beat": "MemoryFlash", "type": "reveal", "intensity": 0.8},
        {"beat": "Departure", "type": "rest", "intensity": 0.2}
    ],
    "BioContainment": [
        {"beat": "Approach", "type": "traversal", "intensity": 0.3},
        {"beat": "VanceLogs", "type": "reveal", "intensity": 0.4},
        {"beat": "WaveformPuzzle", "type": "puzzle", "intensity": 0.6},
        {"beat": "OrganRestored", "type": "reward", "intensity": 0.4},
        {"beat": "AstraTruth", "type": "reveal", "intensity": 0.9}
    ]
    # ... continues for each zone
}
```

### 5.2 Teaching New Mechanics
| Mechanic | Introduction Zone | Safe Practice | Pressure Test |
|----------|-------------------|---------------|---------------|
| Phase Shift | Entry Hangar | Orientation puzzle (no fail) | Reactor Core (multi-room) |
| Power Routing | Reactor Core | Single generator | 3-generator sequence |
| Astra Sync | Reactor Core | Single node hold | Dual-node 0.5s window |
| Waveform Matching | Bio-Containment | Single frequency | Dual param (freq + amp) |
| Multi-Node Sync | Comms Array | 2 nodes | 3 nodes (Sub-Level 0) |
| Time Dilation | Reactor Core (post-organ) | Steam vent cycle | Puzzle timing window |
| Past Vision | Bio-Containment (post-organ) | Static clue reveal | Dynamic puzzle hint |
| Glide | Comms Array (post-organ) | Short gap | Long traverse + wind |
| Wall Run | Hangar 18 (post-organ) | Vertical shaft | Horizontal traverse |

---

## 6. Level Streaming & World Partition

### 6.1 Streaming Structure
```
PersistentLevel
├── Landscape (Nanite)
├── Global Lighting (Lumen)
├── Player Start
├── TimelineManager
└── Zone Sub-Levels (World Partition Grids)
    ├── Grid_Large (Architecture, 500m cells)
    ├── Grid_Medium (Props, 100m cells)
    └── Grid_Small (Detail, 25m cells)

Additive Past Levels (loaded on Phase Shift)
├── Past_EntryHangar
├── Past_ReactorCore
├── Past_BioContainment
├── Past_CommsArray
├── Past_Hangar18
├── Past_Administration
└── Past_SubLevel0
```

### 6.2 Streaming Optimization
- **Pre-warm Past Levels**: Load adjacent past level when player approaches Vista Node (100m radius)
- **HLOD**: Nanite landscape no HLOD; architecture HLOD at 200m
- **Lumen**: GPU Lightmass for Present static; Lumen dynamic for Past (movable lights)
- **Virtual Textures**: 4k trim sheets, 2k unique, VT streaming pool 2GB

---

## 7. Playtest Metrics (Per Zone)

### 7.1 Key Metrics to Track
| Metric | Target | Tool |
|--------|--------|------|
| Time to first Phase Shift | < 5 min | Analytics |
| Puzzle solve time (avg) | 3-8 min | Analytics + Playtest |
| Hint usage rate | < 30% | Analytics |
| Fast travel usage | > 60% | Analytics |
| Motion sickness reports | 0% | Playtest survey |
| Traversal feel rating | > 4/5 | Playtest survey |
| Narrative clarity | > 4/5 | Playtest survey |
| Bug: Past solve not reflected in Present | 0 | Automated test |

### 7.2 Playtest Protocol
1. **Internal Daily**: 15-min traversal + 1 puzzle
2. **Weekly External**: 3 players, fresh eyes, think-aloud
3. **Milestone Vertical Slice**: 5 players, full zone, post-play interview
4. **Accessibility Test**: 3 players with disabilities per milestone