# Chronos 51 — Puzzle Design Bible

## 1. Puzzle Philosophy (per puzzle + level-design skills)

### 1.1 Core Principles
1. **Knowledge Gates, Not Ability Gates** — Progress blocked by *understanding*, not movement skills (Outer Wilds)
2. **Single Mechanic Depth** — Each puzzle chain explores one logic type completely (Braid)
3. **Observation > Trial/Error** — Clues embedded in environment, NPC routines, documents
4. **Cross-Timeline Causality** — Past solution → Present physical change (clear visual language)
5. **Astra as Co-Processor** — Gamespeak commands enable multi-node coordination
6. **No Time Pressure** — Turn-based logic, player controls pace (Deathloop QoL)

### 1.2 Puzzle Difficulty Curve
| Tier | Zones | Complexity | Clue Directness | Astra Involvement |
|------|-------|------------|-----------------|-------------------|
| **Tutorial** | Entry Hangar | Single room, 1 step | Explicit (NPC says it) | None |
| **Novice** | Reactor Core | 2 rooms, 2-3 steps | Environmental (logs, charts) | Hold (single node) |
| **Apprentice** | Bio-Containment | 2 rooms, audio-visual | Implicit (humming, patterns) | Hold + Scan |
| **Adept** | Comms Array | Facility-wide, 3 nodes | Cross-reference (map + signal) | Hold (dual-node sync) |
| **Master** | Hangar 18 | All mechanics combined | Synthesis (memory + observation) | Full command set |
| **Synthesis** | Sub-Level 0 | 3-node simultaneous | All previous knowledge | Full coordination |

---

## 2. Puzzle Type Library

### 2.1 Power Routing (Graph Logic)
**Core Mechanic:** Connect sources → transformers → targets via cable paths
**Variations:**
- Sequential phasing (order matters)
- Load balancing (capacity constraints)
- Redundancy (multiple paths to target)
**Clue Types:** Wall schematics, engineer logs, gauge readings, NPC dialogue
**Present Result:** Power restored → doors open, elevators work, lights on

### 2.2 Sequence Alignment (Rotary/Linear Matching)
**Core Mechanic:** Rotate rings, slide tracks, calibrate dials to match target pattern
**Variations:**
- Concentric rings (independent rotation)
- Interlocking gears (rotation affects neighbors)
- Multi-axis calibration (frequency + amplitude + phase)
**Clue Types:** Oscilloscope targets, audio tones, visual patterns, mathematical relationships
**Present Result:** Machine calibrated → produces item, opens path, stabilizes system

### 2.3 Temporal Synchronization (Multi-Node Timing)
**Core Mechanic:** Trigger events at multiple locations within tight time window
**Variations:**
- Dual-node (player + Astra)
- Triple-node (player + Astra + environmental cycle)
- Cascading (A triggers B triggers C)
**Clue Types:** Radio orders, NPC countdowns, visual sync pulses, audio cues
**Present Result:** Simultaneous activation → bridge extends, containment drops, signal transmits

### 2.4 Waveform Matching (Audio-Visual Pattern)
**Core Mechanic:** Adjust frequency/amplitude/phase to match target waveform
**Variations:**
- Single parameter (frequency only)
- Dual parameter (frequency + amplitude)
- Full synthesis (freq + amp + phase + harmonics)
**Clue Types:** NPC humming, audio logs, oscilloscope displays, resonance phenomena
**Present Result:** Field harmonized → containment drops, neural link established, door unlocks

### 2.5 Archive Decryption (Cross-Reference Logic)
**Core Mechanic:** Combine multiple documents/clues to derive code/sequence/location
**Variations:**
- Substitution cipher (symbol → value)
- Multi-document synthesis (log + chart + label)
- Environmental cross-reference (gauge + log + NPC statement)
**Clue Types:** Burned documents (Neural Loom reveals), safe combinations, coordinate systems
**Present Result:** Access granted → Hangar 18 doors, Sub-Level 0 coordinates, Thorne's terminal

---

## 3. Puzzle Chain Specifications

### 3.1 Reactor Core: "Zero-Point Stabilization" (Chronos Core)
| Step | Puzzle Type | Location | Clue Source | Solution | Astra Command | Present Change |
|------|-------------|----------|-------------|----------|---------------|----------------|
| 1 | Power Routing | Control Room A | Sparks' mutter + wall chart | Alpha → Gamma → Beta | Hold (Sync Console) | Generators online |
| 2 | Temporal Sync | Control Room A + B | Corwin's radio order | Dampener A + B within 0.5s | Hold (Node B Dampener) | Core stabilized |
| 3 | Sequence Alignment | Master Console | Oscilloscope target | Frequency 7.83 Hz (Schumann) | Hold (Calibrate) | Radiation clears, water drains, bridge extends |

**Teaching:** Sequential logic → Timing coordination → Precision calibration
**Organ:** Chronos Core → Time Dilation Bubble

### 3.2 Bio-Containment: "Neural Resonance" (Neural Loom)
| Step | Puzzle Type | Location | Clue Source | Solution | Astra Command | Present Change |
|------|-------------|----------|-------------|----------|---------------|----------------|
| 1 | Archive Decryption | Observation Deck | Vance's notebook (Neural Loom scan) | 432.5 Hz / 0.8 amp | Scan (Reveal notebook) | Frequency known |
| 2 | Waveform Matching | Calibration Lab | Vance humming + oscilloscope | Freq: 432.5, Amp: 0.8, Phase: 0 | Hold (Calibrate) | Neural Loom calibrated |
| 3 | Temporal Sync | Containment Field + Observation | Vance + Colleague dialogue | Field sync + neural pulse simultaneous | Hold (Field Controller) | Containment drops, Neural Loom accessible |

**Teaching:** Audio-visual pattern → Hidden info reveal → Dual-station coordination
**Organ:** Neural Loom → Past Vision Overlay

### 3.3 Comms Array: "First Contact Protocol" (Void Sail)
| Step | Puzzle Type | Location | Clue Source | Solution | Astra Command | Present Change |
|------|-------------|----------|-------------|----------|---------------|----------------|
| 1 | Sequence Alignment | Array Control | Twins' overlapping dialogue | Azimuth 142.5, Elev -12 | Hold (Dish Alignment) | Dishes aligned |
| 2 | Archive Decryption | Signal Analysis | Recording playback + star chart | Target: Gliese 581 system | Scan (Reveal coordinates) | Signal locked |
| 3 | Temporal Sync | Array Control + Deep Space Relay | Twins' countdown | Transmit + Relay acknowledge within 1s | Hold (Relay Trigger) | Signal sent, Reply received |

**Teaching:** Spatial alignment → Cross-reference decryption → Long-distance sync
**Organ:** Void Sail → Double Jump + Glide

### 3.4 Hangar 18: "Stasis Extraction" (Gravity Anchor)
| Step | Puzzle Type | Location | Clue Source | Solution | Astra Command | Present Change |
|------|-------------|----------|-------------|----------|---------------|----------------|
| 1 | Power Routing | Stasis Bay | Thorne's terminal (decrypted) | Core → Buffer → Field → Anchor | Hold (Field Buffer) | Stasis field stable |
| 2 | Waveform Matching | Neural Interface | Astra's fragmented memory | Neural pattern: Astra's resonance | Scan (Astra memory) | Interface ready |
| 3 | Multi-Node Synthesis | Stasis Bay + Extraction Pod + Control | All previous + Thorne's final log | Simultaneous: Field + Interface + Release | Hold (Field) + Hold (Interface) + Recall | Vespera freed, Gravity Anchor extracted |

**Teaching:** Full mechanic synthesis + Memory integration + Three-station coordination
**Organ:** Gravity Anchor → Wall Run / Ceiling Hang

### 3.5 Sub-Level 0: "Fracture Seal" (Astra's Core — Synthesis)
| Step | Puzzle Type | Location | Requirement | Solution | Astra Command | Present Change |
|------|-------------|----------|-------------|----------|---------------|----------------|
| 1 | Synthesis | Reactor Core Past (Node A) | Chronos Core + Neural Loom | Time-dilate reactor pulse | Hold (Reactor Pulse) | Temporal anchor A set |
| 2 | Synthesis | Comms Array Past (Node B) | Void Sail + Gravity Anchor | Glide to relay, wall-run to console | Follow (Relay) + Hold (Console) | Temporal anchor B set |
| 3 | Synthesis | Bio-Containment Past (Node C) | All 4 Organs | Neural Loom broadcast + Past Vision | Scan (Broadcast) + Wait | Temporal anchor C set |
| 4 | Final Choice | Fracture Core | All 3 Anchors + Astra's Core | Seal fracture + Open wormhole | Recall (Unity) | Vespera restored, Wormhole opens |

**Teaching:** Ultimate synthesis — all mechanics, all organs, all trust
**Organ:** Astra's Core → Final Departure

---

## 4. Hint System Design (Progressive, Resonance Currency)

### 4.1 Hint Tier Structure
| Tier | Cost | Reveal Level | Example (Reactor Power Routing) |
|------|------|--------------|----------------------------------|
| **1** | 10 Resonance | Nudge | "Sparks mentioned an order... Alpha, something, Beta" |
| **2** | 25 Resonance | Highlight | Generator panels glow gold in sequence |
| **3** | 50 Resonance | Solution | "The order is Alpha → Gamma → Beta. Never Delta." |

### 4.2 Resonance Economy
- **Earned:** Puzzle solves (100), Memory fragments (50), Exploration (10-25), Astra dialogue (5)
- **Spent:** Hints (10/25/50), Cosmetic (Astra visual variants, Photo mode filters)
- **Balance:** ~500 total earnable, ~300 needed for all hints → encourages selective use

---

## 5. Accessibility in Puzzles
- **Color Blind:** Symbol + color coding (never color-only)
- **Audio Impaired:** Visual waveform display, subtitle clues, vibration cues
- **Motor Impaired:** Hold→Toggle for Astra commands, extended sync windows (0.5s → 2s)
- **Cognitive:** Hint system, puzzle skip (Story Mode), simplified UI mode
- **No Time Pressure:** All sync windows generous, no fail states

---

## 6. Puzzle Data Format (DA_PuzzleDefinition DataAsset)

```cpp
// C++ DataAsset Structure
USTRUCT(BlueprintType)
struct FPuzzleStep
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere) FName StepID;
    UPROPERTY(EditAnywhere) FText Description;
    UPROPERTY(EditAnywhere) FName ClueSourceID; // StringTable reference
    UPROPERTY(EditAnywhere) TMap<FName, FString> SolutionData; // Flexible key-value
    UPROPERTY(EditAnywhere) EAstraCommand RequiredAstraCommand;
    UPROPERTY(EditAnywhere) FName RewardFragmentID;
};

USTRUCT(BlueprintType)
struct FPuzzleHint
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere) int32 Cost;
    UPROPERTY(EditAnywhere) FText HintText;
    UPROPERTY(EditAnywhere) TArray<AActor*> HighlightActors; // For Tier 2
};

UCLASS(BlueprintType)
class UDAPuzzleDefinition : public UDataAsset
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere) FName PuzzleID;
    UPROPERTY(EditAnywhere) FText DisplayName;
    UPROPERTY(EditAnywhere) EPuzzleType Type;
    UPROPERTY(EditAnywhere) FName ZoneID;
    UPROPERTY(EditAnywhere) TArray<FName> RequiredVistaNodes;
    UPROPERTY(EditAnywhere) TArray<FName> PrerequisitePuzzles;
    UPROPERTY(EditAnywhere) TArray<FPuzzleStep> Steps;
    UPROPERTY(EditAnywhere) FName OrganUnlock;
    UPROPERTY(EditAnywhere) TArray<FZoneStateChange> PresentChanges;
    UPROPERTY(EditAnywhere) TArray<FPuzzleHint> HintTiers;
    UPROPERTY(EditAnywhere) float EstimatedSolveTimeMinutes;
};
```

---

## 7. Automated Validation (CI Integration)

### 7.1 Puzzle Solvability Tests
- **Graph Reachability:** All Present paths reachable after Past solves
- **State Consistency:** ZoneState changes applied correctly
- **Prerequisite Order:** No circular dependencies in puzzle graph
- **Resource Balance:** Resonance earnable > spendable for hints

### 7.2 Playtest Bot (Headless)
```python
# Pseudocode for automated playthrough verification
def verify_critical_path():
    save = new_game()
    for zone in ZONE_ORDER:
        for puzzle in zone.puzzles:
            solve_puzzle(puzzle, save)
            assert present_changes_applied(puzzle.presentChanges, save)
            assert next_zone_accessible(zone, save)
    assert sublevel0_unlocked(save)
    assert all_endings_reachable(save)
```