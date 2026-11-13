# Chronos 51 — Narrative Design Bible

## 1. Narrative Structure Overview

### 1.1 Story Arc (3 Acts)
| Act | Zones | Core Revelation | Astra Relationship |
|-----|-------|-----------------|-------------------|
| **I: Awakening** | Entry Hangar → Reactor Core | Diaspora happened; Astra chose to anchor | Protective guide, fragmented memory |
| **II: Scattered** | Bio-Containment → Comms Array → Hangar 18 | Astra *volunteered*; Thorne pulled them; signal was invitation | Trust fractures → rebuild through action |
| **III: Synthesis** | Sub-Level 0 | Fracture seal requires mutual choice; wormhole home | Equal partners; choose future together |

### 1.2 Ending Matrix (Trust × Memory Fragments)
| Trust \ Fragments | 0-3 (Low) | 4-7 (Med) | 8-10 (High) | 11-12 (All) |
|-------------------|-----------|-----------|-------------|-------------|
| **0-3 (Low)** | Tragic: Stranded, Astra lost, chitin dulls | Bittersweet: Vespera barely flies, Astra silent | Bittersweet: Depart alone, Earth changed | Good: Depart alone, Astra recovering |
| **4-7 (Med)** | Bittersweet: Astra fragmented, Vespera damaged | Good: Vespera functional, Astra recovering | Good: Depart together, Astra healing | True: Vespera whole, Astra whole, new signal |
| **8-10 (High)** | Good: Depart together, Astra healing | True: Vespera whole, Astra whole, new signal | True: Vespera whole, Astra whole, new signal | **TRUE ENDING** |

---

## 2. Yarn Spinner Implementation (per dialogue-systems skill)

### 2.1 Project Structure
```
Chronos51/
├── Yarn/
│   ├── ProjectSettings.yarnproject
│   ├── Global/
│   │   ├── Variables.yarn
│   │   ├── Functions.yarn
│   │   └── Commands.yarn
│   ├── Act1/
│   │   ├── EntryHangar.yarn
│   │   └── ReactorCore.yarn
│   ├── Act2/
│   │   ├── BioContainment.yarn
│   │   ├── CommsArray.yarn
│   │   └── Hangar18.yarn
│   ├── Act3/
│   │   └── SubLevel0.yarn
│   ├── Astra/
│   │   ├── Trust_Low.yarn
│   │   ├── Trust_Med.yarn
│   │   ├── Trust_High.yarn
│   │   └── MemoryFragments.yarn
│   └── Past_NPCs/
│       ├── Vance.yarn
│       ├── Corwin.yarn
│       ├── Thorne.yarn
│       ├── Sparks.yarn
│       └── Twins.yarn
├── Localization/
│   ├── en/Strings.csv
│   ├── fr/Strings.csv
│   ├── de/Strings.csv
│   └── ... (10+ languages)
└── UE5 Integration/
    ├── YarnProject.uasset
    ├── DialogueRunnerSubsystem.h/.cpp
    └── DialogueUIWidget/
```

### 2.2 Global Variables (Global/Variables.yarn)
```yarn
title: GlobalVariables
---
<<declare $trust = 0>>
<<declare $memoryFragments = []>>
<<declare $astraState = "fragmented">> // fragmented, recovering, whole
<<declare $currentZone = "EntryHangar">>
<<declare $organsRestored = []>>
<<declare $puzzleStates = {}>>
<<declare $resonance = 0>>
<<declare $difficulty = "normal">> // normal, story (skip puzzles)
<<declare $accessibility = {}>>
<<declare $playthroughID = ">>

// Helper functions
<<function hasFragment(fragmentID)>>
<<if $memoryFragments.indexOf(fragmentID) != -1>>
<<return true>>
<<else>>
<<return false>>
<</if>>
<