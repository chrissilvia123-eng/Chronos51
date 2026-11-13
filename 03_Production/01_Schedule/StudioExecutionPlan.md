# Chronos 51 — Studio Execution Plan (Industry Standard)

## How Real Studios Run Production

| Studio Practice | Chronos 51 Adaptation |
|-----------------|----------------------|
| **Pre-production** → Vertical Slice → Production → Alpha → Beta → Gold | M0 Prototype → M1 Vertical Slice → M2 Production → M3 Content Complete → M4 Polish → Launch |
| **2-week sprints** with Sprint Planning, Daily Standups, Sprint Review, Retro | 2-week sprints, Monday planning, Friday review |
| **Definition of Done** per discipline (Code, Art, Design, Audio) | DoD checklists per milestone |
| **Horizontal Slice first** (all systems working rough) → Vertical Slice (one zone polished) | M0 = Horizontal (traversal + phase shift + save), M1 = Vertical (Reactor Core) |
| **Playtest every sprint** (internal + external) | Weekly internal, bi-weekly external |
| **Metrics-driven** (completion rates, fun, clarity, performance) | Automated telemetry + playtest surveys |
| **Risk-first** (tackle highest uncertainty early) | Phase Shift comfort, dual-timeline sync, Nanite perf in M0 |
| **Pipeline before content** (artist can't work without working pipeline) | Blender→UE5 USD pipeline validated Week 1 |

---

## Sprint Structure (2-Week Cycles)

### Sprint Cadence
```
Week 1 (Mon-Fri)          Week 2 (Mon-Fri)
├── Mon: Sprint Planning  ├── Mon: Mid-sprint check
├── Tue-Thu: Deep Work    ├── Tue-Thu: Deep Work
├── Fri: Sprint Review    ├── Fri: Sprint Review + Retro
│   ├── Demo to team      │   ├── Demo to team
│   ├── Metrics review    │   ├── Metrics review
│   ├── Blocker triage    │   ├── Blocker triage
│   └── Next sprint prep  │   └── Next sprint prep
└── Weekend: Rest         └── Weekend: Rest
```

### Daily Standup (15 min max)
```
Each person answers:
1. What did you complete yesterday?
2. What will you complete today?
3. What blocks you? (needs help from whom?)
```

### Sprint Review (30 min)
```
1. Demo working features (record for async review)
2. Metrics: completion rate, bug count, performance, playtest scores
3. Decide: Accept / Rework / Cut for each deliverable
4. Update Definition of Done checklist
```

### Retrospective (30 min)
```
Start / Stop / Continue:
- What should we START doing?
- What should we STOP doing?
- What should we CONTINUE doing?
→ 1 action item per person for next sprint
```

---

## M0: Foundation & Prototype (Sprints 1-2)

### Sprint 1 (Weeks 1-2): "Project Boot + Traversal Core"

#### Monday - Sprint Planning
- [ ] Create UE5 project at `E:\Chronos51\05_UE5_Project\Chronos51` (C++, 5.3+)
- [ ] Enable plugins: Enhanced Input, GAS, Niagara, MetaSounds, World Partition, USD
- [ ] Configure Project Settings (Nanite, Lumen, Virtual Textures, Physics timestep)
- [ ] Set up Git LFS + GitHub repo + branch protection (main, develop)
- [ ] Create GitHub Actions workflow: Build Editor → Run Tests → Upload Artifacts

#### Tuesday - Blender Pipeline Validation
- [ ] Blender 5.3 + USD export test: Cube → USD → UE5 import → verify scale (1uu=1cm), normals, materials
- [ ] Create Material Layer test: Base + Damage + Moss + Rust + Clean vertex paint masks
- [ ] Export test asset through full pipeline → verify in UE5 (Nanite enabled, Lumen cards generated)
- [ ] Document pipeline steps in `BlenderPipeline.md` Quick Start section

#### Wednesday - Xylos Traversal Prototype
- [ ] Create `Chronos51Character` (C++ extends `ACharacter`) with GAS AbilitySystemComponent
- [ ] Implement movement: Walk/Run accel 0.1s, Jump height 2.2m, time-to-apex 0.35s, fall gravity 1.8×
- [ ] Add Coyote Time (0.1s), Jump Buffer (0.12s), Variable Jump (0.45× on release)
- [ ] Add Mantle (≤1.5m ledges, auto-detect via capsule trace)
- [ ] Shoulder Camera: SpringArm + deadzone (40×30cm) + look-ahead (2m) + exponential smoothing (rate 8)

#### Thursday - Input + Save System
- [ ] Enhanced Input: Create all IAs (Move, Look, Jump, Mantle, Sprint, Crouch, Phase, Organs 1-4, Astra, Interact, Journal)
- [ ] Input Mapping Contexts: IMC_Traversal, IMC_PuzzlePanel, IMC_Journal, IMC_PhotoMode, IMC_Cinematic, IMC_AstraCommand
- [ ] SaveSystem: Atomic JSON write (tmp → rename → .bak), version migration chain (v1→v5), autosave triggers

#### Friday - Sprint Review
- [ ] **Demo**: Walk → Run → Jump → Mantle → Save → Quit → Load → Continue
- [ ] **Metrics**: Frame time < 16ms, input latency < 2 frames, save/load < 500ms
- [ ] **DoD Check**: Code compiles, no warnings, runs headless in CI, documented in README

---

### Sprint 2 (Weeks 3-4): "Phase Shift + Dual Timeline"

#### Monday - Sprint Planning
- [ ] Define Phase Shift acceptance criteria: <3s transition, no input during blend, Past level loads async, pawn swap works
- [ ] Create `TimelineManager` GameInstanceSubsystem skeleton
- [ ] Create `VistaNode` actor with CineCameraComponent, trigger volume, PhaseShiftSequence reference

#### Tuesday - Phase Shift Implementation
- [ ] TimelineManager.RequestPhaseShift(): Disable input → Play LevelSequence (camera blend) → Async load Past sub-level
- [ ] OnPastLevelLoaded(): Spawn PastPawn at anchor → Possess → Activate CineCamera → Broadcast OnTimelineChanged
- [ ] ReturnToPresent(): Reverse sequence → Destroy PastPawn → Possess MainPawn → Apply ZoneState changes

#### Wednesday - Dual Timeline State
- [ ] ZoneState DataAsset: WaterLevel, BridgeExtended, RadiationCleared, DoorStates, MachineStates
- [ ] PuzzleManager: StartPuzzle, ProcessAction, CheckSolved, OnPuzzleSolved → TimelineManager.RecordZoneChange
- [ ] Present changes: Visual feedback (golden shimmer particles, temporal particles, 3s transition)

#### Thursday - Vista Node + First Puzzle
- [ ] Place Vista Node in test level (Entry Hangar) with CineCamera framing a simple puzzle object
- [ ] Create test puzzle: "Activate 3 Generators in Order" (Power Routing type)
- [ ] DataAsset: DA_PuzzleDefinition with steps, clues, Astra Hold command, PresentChanges
- [ ] Integrate: Phase In → Solve Puzzle → Phase Out → Verify Present Change (bridge extends)

#### Friday - Sprint Review (M0 GATE)
- [ ] **Demo**: Full loop — Traverse → Vista Node → Phase Shift → Past Puzzle → Phase Out → Present Changed
- [ ] **Metrics**: Phase Shift < 3s, 0 motion sickness (self-test), Present change visible, save persists state
- [ ] **DoD Check**: 
  - [ ] Code compiles, CI passes
  - [ ] Blender→UE5 pipeline documented + working
  - [ ] Traversal feels responsive (coyote/buffer verified)
  - [ ] Phase Shift comfort tested (instant cut option works)
  - [ ] Save/Load works across Phase Shift
  - [ ] **DECISION**: GO to M1 / EXTEND M0 / PIVOT

---

## M1: Vertical Slice - Reactor Core (Sprints 3-6)

### Sprint 3 (Weeks 5-6): "Reactor Core Blockout + Art Pipeline"

| Day | Level Design | Environment Art | Tech |
|-----|--------------|-----------------|------|
| Mon | Blockout Present Reactor Core (metrics-driven) | Create Blender asset list from blockout | Zone streaming setup (World Partition grids) |
| Tue | Blockout Past Reactor Core (puzzle spaces) | Hero asset: Reactor Console (high→low→bake) | Past level streaming config |
| Wed | Place Vista Nodes A & B with cameras | Kit assets: Pipes, generators, panels (trim sheet) | Lighting: GPU Lightmass Present, Lumen Past |
| Thu | Define puzzle spaces + Astra anchor points | Texture: Trim sheet + unique hero textures | Material Layer blending setup |
| Fri | **Review**: Walk Present → Phase A → Phase B → metrics | **Review**: Asset QA pass in UE5 | **Review**: Streaming hitches < 100ms |

**Sprint 3 DoD**: Reactor Core blockout playable, 5 hero assets in UE5 passing QA, lighting mood established

---

### Sprint 4 (Weeks 7-8): "Reactor Core Puzzles + Chronos Core"

| Day | Gameplay | Narrative | Tech |
|-----|----------|-----------|------|
| Mon | Power Routing puzzle (3 generators) | Write Sparks/Corwin dialogue (Yarn) | PuzzleManager integration |
| Tue | Temporal Sync puzzle (dual node) | Record placeholder VO (TTS) | Astra Hold command implementation |
| Wed | Calibration puzzle (frequency match) | Memory flash sequence | Chronos Core GAS ability |
| Thu | Organ restoration cinematic | Trust + Fragment system | Time Dilation Bubble traversal |
| Fri | **Review**: All 3 puzzles solve → Organ granted → Ability works | **Review**: Dialogue triggers correctly | **Review**: Performance + save persistence |

**Sprint 4 DoD**: Reactor Core vertical slice complete — traversal, puzzles, organ, ability, narrative, save all working

---

### Sprint 5 (Weeks 9-10): "Polish + Accessibility + Playtest Prep"

| Day | Polish | Accessibility | QA |
|-----|--------|---------------|----|
| Mon | VFX: Phase Shift, temporal particles, organ restore | Full rebinding UI | Internal playtest (3 people) |
| Tue | SFX: MetaSounds for abilities, puzzles, ambience | Color blind palettes (3 types) | Bug triage (P0/P1/P2) |
| Wed | UI: Journal, hint system, fast travel map | Reduced motion (instant Phase Shift) | Performance profile (stat gpu) |
| Thu | Camera: Photo mode, DOF, exposure | Hint system (3 tiers, Resonance cost) | External playtest prep |
| Fri | **Review**: Polish pass complete | **Review**: Accessibility audit | **Review**: Playtest script ready |

**Sprint 5 DoD**: Reactor Core vertical slice polished, accessible, ready for external playtest

---

### Sprint 6 (Weeks 11-12): "External Playtest + M1 Gate"

| Day | Activity |
|-----|----------|
| Mon | Recruit 5 external playtesters (Discord, friends, local) |
| Tue | Run playtests (30 min each, think-aloud, record) |
| Wed | Run playtests |
| Thu | Analyze: completion time, stuck points, clarity scores, motion sickness, fun rating |
| Fri | **M1 GATE REVIEW**: Decision — GO to M2 / REWORK / CUT SCOPE |

**M1 Gate Criteria (ALL must pass):**
- [ ] 5/5 playtesters complete Reactor Core in 20-35 min
- [ ] Average clarity rating ≥ 4/5
- [ ] 0 motion sickness reports
- [ ] 60 FPS @ 1440p on target spec (RTX 3070 / RX 6700 XT)
- [ ] 0 P0 bugs, < 5 P1 bugs
- [ ] Save/Load works across full zone
- [ ] All accessibility options functional

---

## M2: Full Production (Sprints 7-16) — Parallel Tracks

### Track Structure (3 people minimum)
```
Track A: Environment Art (1.5 FTE)
├── Sprint 7-8: Bio-Containment assets + lighting
├── Sprint 9-10: Comms Array assets + lighting
├── Sprint 11-12: Hangar 18 assets + lighting
├── Sprint 13-14: Administration + Sub-Level 0 assets
├── Sprint 15-16: Polish, LODs, optimization

Track B: Gameplay + Systems (1.0 FTE)
├── Sprint 7-8: Neural Loom ability + Bio puzzles
├── Sprint 9-10: Void Sail ability + Comms puzzles
├── Sprint 11-12: Gravity Anchor + Hangar 18 puzzles
├── Sprint 13-14: Astra Core + Sub-Level 0 synthesis puzzle
├── Sprint 15-16: New Game+, Photo mode, endings

Track C: Narrative + Design (0.5 FTE each)
├── All sprints: Yarn scripts, VO direction, playtest coordination
├── Sprint 7-8: Bio-Containment narrative (Vance, memory fragments)
├── Sprint 9-10: Comms Array narrative (Twins, signal)
├── Sprint 11-12: Hangar 18 narrative (Thorne, extraction)
├── Sprint 13-14: Ending variants (4× Trust×Fragment matrix)
├── Sprint 15-16: Localization CSV export + QA
```

### Bi-Weekly Sync Points (Every 2 Sprints)
| Sync | Participants | Agenda |
|------|--------------|--------|
| **Zone Handoff** | Art + Design + Tech | Blockout approved → Art begins; Puzzle spaces locked |
| **Ability Integration** | Gameplay + Art + Tech | New organ ability works in traversal + puzzles |
| **Playtest Review** | All | External playtest results → decisions |
| **Performance Budget** | Tech + Art | Nanite clusters, Lumen cost, streaming hitches |

---

## M3: Content Complete (Sprints 17-20)

### Sprint 17-18: Sub-Level 0 + Synthesis Puzzle
- Non-Euclidean art, fracture VFX, Thorne's Echo AI
- 3-node simultaneous coordination puzzle (all organs required)
- Astra Trust/Fragments gate final choice

### Sprint 19: Endings Implementation
- 4 ending variants cinematic (Sequencer)
- New Game+ (memory retention, cosmetic rewards)
- Photo mode, lore compendium, credits

### Sprint 20: M3 Gate — Content Complete
- All zones playable start-to-finish
- All endings reachable
- 0 P0/P1 bugs
- Performance certified on target platforms

---

## M4: Polish & Launch (Sprints 21-24)

### Sprint 21: Performance + Bug Bash
- Nanite cluster optimization, Lumen CVars, streaming hitch elimination
- Full regression test (automated + manual)
- Platform builds (Steam, Epic Shipping config)

### Sprint 22: Localization + Certification
- 10+ languages QA (Yarn CSV → in-game)
- Steam/Epic store page, age rating, achievements
- Console certification (if applicable)

### Sprint 23: Marketing + Press
- 4K screenshots, cinematic trailer (Sequencer)
- Press kits, influencer keys, Steam page live
- Launch build uploaded to Steam/Epic

### Sprint 24: Launch + Day-1 Patch
- Launch day monitoring
- Day-1 patch ready (known issues from beta)
- Post-launch retrospective

---

## Tools & Templates to Use Today

### 1. GitHub Projects Board (Create Now)
```
Columns: Backlog → Sprint Ready → In Progress → In Review → Done
Labels: 
  - Type: code, art, design, narrative, audio, tech
  - Priority: P0 (blocker), P1 (high), P2 (medium), P3 (low)
  - Sprint: S1, S2, S3...
  - Discipline: traversal, phase-shift, puzzles, narrative, art, pipeline
```

### 2. Sprint Template (Copy for each sprint)
```markdown
# Sprint X (Week Y-Z) — [Theme]

## Goal
[One sentence: what must be true at end of sprint]

## Commitments
- [ ] Deliverable 1 (Owner) — DoD: [specific criteria]
- [ ] Deliverable 2 (Owner) — DoD: [specific criteria]
- [ ] Deliverable 3 (Owner) — DoD: [specific criteria]

## Capacity
- [Name]: [Hours available] — [Assigned tasks]
- [Name]: [Hours available] — [Assigned tasks]

## Risks
- [Risk]: [Mitigation]

## Sprint Review (Friday)
- Demo: [What to show]
- Metrics: [What to measure]
- Decisions: [Accept/Rework/Cut per deliverable]
```

### 3. Definition of Done Checklists

**Code DoD:**
- [ ] Compiles without warnings (UE5 + C++ static analysis)
- [ ] Runs in CI (GitHub Actions build + headless test)
- [ ] Unit tests pass (if applicable)
- [ ] Documented in header comments (public API)
- [ ] Code reviewed (PR approved)
- [ ] No hardcoded paths/magic numbers (use DataAssets/config)

**Art DoD:**
- [ ] Passes `qa_check.py --profile game` (no non-manifold, inverted normals, etc.)
- [ ] Texel density within 10% of target (512 kit / 1024 hero)
- [ ] Naming convention: SM_/SK_/CAM_/M_/T_
- [ ] Exported via USD pipeline, verified in UE5 (Nanite enabled, Lumen cards)
- [ ] Material Layers work (Present/Past variants via vertex paint)
- [ ] LODs generated (auto for Nanite, manual for skeletal)

**Design DoD:**
- [ ] Puzzle documented in DA_PuzzleDefinition (steps, clues, Astra commands, PresentChanges)
- [ ] Playtested internally (no soft-locks, clear feedback)
- [ ] Accessibility: color blind, reduced motion, hint system
- [ ] Integrated with TimelineManager (ZoneState changes applied)

**Narrative DoD:**
- [ ] Yarn script compiles, no undefined variables
- [ ] VO placeholder (TTS) recorded for all lines
- [ ] Trust/Fragment gates documented
- [ ] Localization keys follow convention (`DLG_ZONE_BEAT_LINE`)

---

## Week 1 Action Checklist (Do This Today)

### Morning (2 hours)
- [ ] Create UE5 project: `E:\Chronos51\05_UE5_Project\Chronos51` (C++, 5.3+)
- [ ] Enable plugins: Enhanced Input, GAS, Niagara, MetaSounds, World Partition, USD
- [ ] Configure Project Settings per `TechnicalArchitecture.md` Section 1.1
- [ ] Init Git repo + Git LFS + push to GitHub

### Midday (2 hours)
- [ ] Blender test: Default cube → USD export → UE5 import → verify 1uu=1cm, normals correct
- [ ] Create simple Material Layer test in Blender (vertex paint R/G/B/A) → export → UE5 Master Material
- [ ] Document exact steps in `E:\Chronos51\06_Blender_Pipeline\pipeline_docs\quick_start.md`

### Afternoon (2 hours)
- [ ] Create `Chronos51Character.h/.cpp` with GAS AbilitySystemComponent
- [ ] Implement movement component settings (walk/run/jump/mantle/coyote/buffer)
- [ ] Create Enhanced Input Actions + Mapping Contexts (IMC_Traversal)
- [ ] Test in editor: WASD move, Space jump, Shift sprint, Ctrl crouch

### Evening (1 hour)
- [ ] Create GitHub Actions workflow (build + headless test)
- [ ] Push to GitHub → verify CI passes
- [ ] Write Sprint 1 plan in GitHub Projects

---

## Metrics Dashboard (Track Weekly)

| Metric | Target | Tool |
|--------|--------|------|
| **Velocity** | 80%+ commitments met | GitHub Projects |
| **Build Health** | 100% CI passing | GitHub Actions |
| **Performance** | 60 FPS @ 1440p (M1+) | `stat gpu` / Unreal Insights |
| **Playtest Clarity** | ≥ 4/5 | Google Forms post-playtest |
| **Motion Sickness** | 0% reports | Playtest survey |
| **Bug Count** | 0 P0, < 5 P1 per sprint | GitHub Issues |
| **Save/Load Time** | < 500ms | In-game telemetry |
| **Phase Shift Time** | < 3s | In-game telemetry |
| **Streaming Hitches** | < 100ms | `stat streaming` |

---

## Decision Gates (No-Go = Stop & Fix)

| Gate | When | Criteria | If Failed |
|------|------|----------|-----------|
| **M0 Gate** | End Sprint 2 | Full Phase Shift loop works, traversal feels good, pipeline validated | Extend M0 1-2 weeks |
| **M1 Gate** | End Sprint 6 | Vertical slice playtest passes all criteria | Rework Reactor Core / Cut scope |
| **M2 Gate** | End Sprint 16 | All zones playable, 0 P0/P1, perf certified | Extend production / Cut zone |
| **M3 Gate** | End Sprint 20 | Content complete, all endings reachable | Polish sprint extended |
| **Launch Gate** | End Sprint 24 | Platform cert passed, loc QA passed, marketing ready | Delay launch |

---

## Communication Protocol

| Channel | Purpose | Frequency |
|---------|---------|-----------|
| **Daily Standup** | Sync blockers, progress | Daily 15 min (async OK via Discord) |
| **Sprint Review** | Demo, metrics, decisions | Bi-weekly Friday 30 min |
| **Retrospective** | Process improvement | Bi-weekly Friday 30 min |
| **Playtest Debrief** | Analyze external feedback | Post-playtest 60 min |
| **Zone Handoff** | Art↔Design↔Tech alignment | Per zone start |
| **Emergency** | P0 blockers | Immediate (Discord @here) |

---

*This plan follows industry-standard production methodology adapted for Chronos 51's scope. Execute Sprint 1 today. Review Friday. Adjust Monday.*