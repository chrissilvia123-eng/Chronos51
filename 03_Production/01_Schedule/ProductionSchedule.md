# Chronos 51 — Production Schedule & Tracking

## 1. Milestone Timeline (48 Weeks / ~11 Months)

### 1.1 M0: Foundation & Prototype (Weeks 1-4)
| Week | Deliverable | Owner | Status | Notes |
|------|-------------|-------|--------|-------|
| 1 | UE5 Project created, C++ modules, Git LFS | Tech Lead | ☐ | World Partition, Nanite, Lumen, GAS enabled |
| 1 | Blender→UE5 USD pipeline validated | Tech Artist | ☐ | Scale, normals, materials, cameras |
| 2 | Xylos traversal prototype (walk/jump/mantle/coyote/buffer) | Gameplay Prog | ☐ | Metrics from platformer skill |
| 2 | Shoulder camera + deadzone + look-ahead | Gameplay Prog | ☐ | Per camera-systems skill |
| 3 | Phase Shift prototype (1 Vista Node) | Tech Lead | ☐ | Camera blend, async past load, pawn swap |
| 3 | Dual-timeline state (ZoneState) | Gameplay Prog | ☐ | Past solve → Present change (bridge test) |
| 4 | CI/CD pipeline (build + headless test) | Tech Lead | ☐ | GitHub Actions, automation test |
| 4 | **M0 Gate**: Playable traverse → Phase Shift → Past puzzle → Present change | All | ☐ | **CRITICAL MILESTONE** |

### 1.2 M1: Vertical Slice — Reactor Core (Weeks 5-12)
| Week | Deliverable | Owner | Status | Notes |
|------|-------------|-------|--------|-------|
| 5-6 | Reactor Core blockout (Present + Past) | Level Design | ☐ | Metrics-driven, Vista Node cameras placed |
| 6-7 | Hero assets: Reactor console, generators, pipes | Env Art | ☐ | Nanite, trim sheets, vertex paint variants |
| 7-8 | Lumen lighting (Present baked + Past dynamic) | Lighting | ☐ | GPU Lightmass, clinical vs ruined mood |
| 8-9 | Puzzle implementation: Power Routing → Sync → Calibrate | Gameplay Prog | ☐ | DataAsset-driven, Astra sync |
| 9-10 | Chronos Core organ + Time Dilation Bubble | Gameplay Prog | ☐ | GAS ability, traversal integration |
| 10-11 | Narrative: Astra dialogue, Sparks/Corwin logs, memory flash | Narrative | ☐ | Yarn Spinner, VO placeholder (TTS) |
| 11-12 | Polish: VFX, SFX, MetaSounds, UI, accessibility | All | ☐ | Phase Shift comfort, hint system |
| 12 | **M1 Gate**: External playtest (5 players, 30 min) | All | ☐ | **VERTICAL SLICE COMPLETE** |

### 1.3 M2: Full Facility Production (Weeks 13-32)
| Phase | Zones | Weeks | Parallel Tracks |
|-------|-------|-------|-----------------|
| **2A** | Bio-Containment | 13-18 | Art: Assets + Lighting \| Design: Puzzles + Narrative \| Tech: Neural Loom ability |
| **2B** | Comms Array | 19-24 | Art: Assets + Lighting \| Design: Puzzles + Narrative \| Tech: Void Sail ability |
| **2C** | Hangar 18 | 25-30 | Art: Assets + Lighting \| Design: Puzzles + Narrative \| Tech: Gravity Anchor ability |
| **2D** | Administration + Sub-Level 0 blockout | 31-32 | Art: Final assets \| Design: Synthesis puzzle \| Tech: Astra Core |

| Week | Cross-Cutting Deliverables | Owner |
|------|----------------------------|-------|
| 13-32 | Puzzle library components (cable router, waveform, sync trigger) | Gameplay Prog |
| 13-32 | Node Network fast travel map UI | UI/UX |
| 13-32 | Astra behavior tree + Gamespeak commands | AI/Companion |
| 13-32 | Memory fragment system + Trust tracking | Narrative |
| 13-32 | Accessibility features (full rebind, hints, color blind, reduced motion) | All |
| 13-32 | Performance optimization passes (per zone) | Tech |
| 13-32 | Localization prep (Yarn CSV export) | Narrative |

### 1.4 M3: Sub-Level 0 & Ending (Weeks 33-40)
| Week | Deliverable | Owner | Notes |
|------|-------------|-------|-------|
| 33-35 | Sub-Level 0 art (non-Euclidean, fracture visuals) | Env Art | Requires all 4 organ traversal |
| 35-37 | Synthesis puzzle (3-node simultaneous coordination) | Gameplay Prog | Most complex puzzle |
| 37-38 | Ending sequences (4 variants based on Trust/Fragments) | Narrative | Cinematics in Sequencer |
| 38-39 | New Game+ (memory retention, cosmetic rewards) | Gameplay Prog | |
| 39-40 | Photo mode, lore compendium, credits | UI/UX | |

### 1.5 M4: Polish, Optimize, Ship (Weeks 41-48)
| Week | Deliverable | Owner |
|------|-------------|-------|
| 41-42 | Performance pass: Nanite clusters, Lumen, streaming hitches | Tech |
| 42-43 | Bug triage: P0/P1/P2, regression testing | QA |
| 43-44 | Platform builds: Steam, Epic (Shipping config, IOStore, Zen) | Tech |
| 44-45 | Localization QA (10+ languages) | Narrative |
| 45-46 | Marketing assets: 4K screenshots, trailer, Steam page | Marketing |
| 46-47 | Certification (if console) / Store review | Production |
| 47-48 | Launch build, day-1 patch prep | All |

---

## 2. Team Roles & Capacity (Solo → Small Team Scaling)

### 2.1 Solo Developer (Weeks 1-12)
| Role | Responsibilities | Tools |
|------|------------------|-------|
| **Tech Lead / Gameplay Prog** | All code, systems, CI | UE5 C++, Blueprint, GitHub Actions |
| **Level Designer** | Blockout, pacing, puzzle layout | UE5 Modeling Mode, Figma |
| **Env Artist** | Hero assets, modular kit, lighting | Blender, Substance, UE5 |
| **Narrative Designer** | Yarn scripts, dialogue, lore | Yarn Spinner, TTS for placeholder VO |
| **Tech Artist** | Pipeline, shaders, optimization | Blender Python, UE5 Material Editor |

### 2.2 Core Team (Weeks 13-32, +3-4 people)
| Role | FTE | Responsibilities |
|------|-----|------------------|
| Gameplay Programmer | 1.0 | Puzzles, abilities, Timeline Manager, save system |
| Level Designer | 1.0 | Zone layout, puzzle integration, pacing, playtest |
| Environment Artist | 1.5 | Hero assets, kit pieces, lighting, Nanite optimization |
| Character Artist | 0.5 | Xylos, Astra VFX, animations |
| Narrative Designer | 0.5 | Yarn scripts, VO direction, localization, ending variants |
| Technical Artist | 0.5 | Pipeline automation, Material Layers, performance |
| QA / Playtest Coordinator | 0.5 | Test plans, external playtests, accessibility, bug tracking |

### 2.3 Production (Weeks 33-48, +2 people)
| Role | FTE | Responsibilities |
|------|-----|------------------|
| Producer | 0.5 | Schedule, platform, certification, launch |
| Marketing/Community | 0.5 | Trailer, Steam page, press kit, community |

---

## 3. Risk Register & Mitigations

| ID | Risk | Likelihood | Impact | Mitigation | Owner | Status |
|----|------|------------|--------|------------|-------|--------|
| R1 | Phase Shift motion sickness | High | Critical | Prototype Week 2; test with sensitive users; instant cut accessibility; vestibular comfort settings | Tech Lead | ☐ |
| R2 | Dual-timeline state sync bugs | High | Critical | Formal ZoneState DataAsset; automated integration tests; visual debug overlay; "Force Sync" cheat | Gameplay Prog | ☐ |
| R3 | Nanite/Lumen perf on mid-range | Medium | High | Scalable settings from M0; Nanite fallback; Lumen SW/HWRT toggle; VT pool size; profile early | Tech Lead | ☐ |
| R4 | Puzzle soft-locks (Present path blocked) | Medium | High | Automated playthrough bot verifies critical path after each solve; "Reset Zone" debug; generous collision | Gameplay Prog | ☐ |
| R5 | Astra AI interfering/getting stuck | Medium | Medium | Simple BT (3 states); "Teleport to Player" fallback; puzzle-assist is scripted sequence | AI/Companion | ☐ |
| R6 | Scope creep (zones/puzzles/organs) | High | High | Fixed scope: 6 zones, 4 organs, 12-18 Vista Nodes, ~25 puzzles; cut list from M0 | Producer | ☐ |
| R7 | Narrative/puzzle disconnect | Medium | Medium | Reactor Core teaches rule explicitly; golden temporal energy visual language; Astra reinforces | Narrative | ☐ |
| R8 | Camera blend jitter at Vista Nodes | Low | Medium | Pre-author curves in Sequencer; test 30/60/120 FPS; fixed timestep; disable input during blend | Tech Lead | ☐ |
| R9 | VO recording delays/budget | Medium | Medium | TTS placeholder until Week 35; batch recording session; remote actors | Narrative | ☐ |
| R10 | Blender→UE5 pipeline breakage | Low | High | Automated QA gate on every export; USD round-trip test in CI; version-pinned Blender/UE5 | Tech Artist | ☐ |

---

## 4. Definition of Done (Per Milestone)

### M0 Done
- [ ] UE5 project compiles, runs headless in CI
- [ ] Xylos traversal feels responsive (coyote 0.1s, buffer 0.12s, variable jump)
- [ ] Phase Shift: Present → Past → Present completes without error
- [ ] Past puzzle solve triggers Present change (bridge extends)
- [ ] Blender asset exports to UE5 with correct scale, normals, materials
- [ ] Git LFS configured, .uproject in repo, GitHub Actions passing

### M1 Done
- [ ] Reactor Core zone complete (Present + Past)
- [ ] 3 puzzles implemented + Chronos Core organ
- [ ] Time Dilation Bubble integrated into traversal
- [ ] Astra dialogue + VO placeholder for all Reactor beats
- [ ] Phase Shift comfort validated (0 motion sickness reports)
- [ ] External playtest: 5 players complete zone, avg 25 min, >4/5 clarity

### M2 Done
- [ ] All 5 zones complete (Present + Past)
- [ ] All 4 organs implemented + traversal abilities
- [ ] Node Network fast travel functional
- [ ] Astra behavior tree + all Gamespeak commands working
- [ ] Memory fragment + Trust system complete
- [ ] All accessibility features implemented
- [ ] Performance: 60 FPS @ 1440p on target spec

### M3 Done
- [ ] Sub-Level 0 complete + Synthesis puzzle
- [ ] All 4 endings implemented + playable
- [ ] New Game+ functional
- [ ] Photo mode, lore compendium, credits

### M4 Done
- [ ] Shipping build passes platform certification
- [ ] All localization QA passed
- [   ] Marketing assets delivered
- [ ] Launch build uploaded to Steam/Epic
- [ ] Day-1 patch ready

---

## 5. Tracking Tools & Artifacts

### 5.1 Primary Tools
| Tool | Purpose | Location |
|------|---------|----------|
| **GitHub Projects** | Task board, milestones, issues | github.com/yourorg/Chronos51 |
| **GitHub Actions** | CI/CD, automated tests | .github/workflows/ |
| **Google Sheets** | Playtest metrics, schedule tracking | Shared drive |
| **Notion/Obsidian** | Design docs, meeting notes | Local + Sync |
| **Discord/Slack** | Team comms, daily standups | #chronos51 |

### 5.2 Key Artifacts (Version Controlled)
```
E:\Chronos51\
├── 00_ProjectRoot\README.md
├── 01_GameDesign\
│   ├── 01_CoreSystems\CoreSystemsDesign.md
│   ├── 02_Narrative\NarrativeDesignBible.md
│   ├── 02_Narrative\Yarn\ (all .yarn files)
│   ├── 03_LevelDesign\LevelDesignBible.md
│   ├── 04_Puzzles\PuzzleDesignBible.md
│   ├── 05_Characters\ (character sheets)
│   └── 06_UI_UX\ (wireframes, flowcharts)
├── 02_ArtDirection\
│   ├── 01_ConceptArt\ (mood boards, refs)
│   ├── 02_CharacterDesign\ (turnarounds, callouts)
│   ├── 03_EnvironmentDesign\ (zone keys, lighting refs)
│   ├── 04_VFX_Reference\ (Niagara designs)
│   └── 05_StyleGuides\ (color, material, UI)
├── 03_Production\
│   ├── 01_Schedule\ProductionSchedule.md (this file)
│   ├── 02_Tracking\ (sprint boards, burndown)
│   ├── 03_Playtests\ (reports, metrics)
│   └── 04_Builds\ (build logs, changelogs)
├── 04_Technical\
│   ├── 01_Architecture\TechnicalArchitecture.md
│   ├── 02_Performance\ (profiles, budgets)
│   ├── 03_Pipeline\BlenderPipeline.md
│   └── 04_Tools\ (custom editor tools)
├── 05_UE5_Project\ (UE5 project root)
├── 06_Blender_Pipeline\ (per BlenderPipeline.md)
└── 07_Reference\
    ├── 01_Games\ (Outer Wilds, Deathloop, Braid analyses)
    ├── 02_Technical\ (UE5 docs, GAS, Nanite guides)
    └── 03_Art\ (material refs, lighting studies)
```

---

## 6. Weekly Cadence

### Monday (Planning)
- [ ] Sync: Review last week's metrics, blockers
- [ ] Plan: Assign tasks for week (GitHub Projects)
- [ ] Playtest: Schedule external sessions

### Tuesday-Thursday (Execution)
- [ ] Daily standup (15 min): What did you do? What's next? Blockers?
- [ ] Deep work blocks (2-3 hours uninterrupted)
- [ ] Code reviews (PRs within 4 hours)

### Friday (Review)
- [ ] Build verification: Run latest CI build, smoke test
- [ ] Playtest review: Watch recordings, document findings
- [ ] Retro: What worked? What didn't? Process improvements?
- [ ] Update: Schedule, risk register, definition of done

---

## 7. Budget Tracking (Indie Scale)

| Category | Estimated Cost | Notes |
|----------|----------------|-------|
| **Software** | $0-500 | UE5 (free), Blender (free), Substance ($20/mo), Yarn (free) |
| **Assets** | $0-2,000 | Quixel (free w/ UE5), Marketplace kits, audio libraries |
| **VO Recording** | $3,000-8,000 | 6 actors × 4 hrs × $150/hr + studio |
| **Localization** | $2,000-5,000 | 10 langs × ~20k words × $0.08-0.12/word |
| **Marketing** | $2,000-10,000 | Trailer, Steam page, press, influencer keys |
| **Platform Fees** | $100 | Steam Direct ($100), Epic (free) |
| **Contingency (20%)** | $1,500-5,000 | |
| **TOTAL** | **$8,600-30,600** | Scales with team size & scope |

---

## 8. Go/No-Go Gates

| Gate | Criteria | Decision |
|------|----------|----------|
| **M0 → M1** | Phase Shift prototype works end-to-end; traversal feels good; pipeline validated | GO / EXTEND / PIVOT |
| **M1 → M2** | Vertical slice playtest >4/5 clarity; 0 motion sickness; performance on target | GO / REWORK / CUT SCOPE |
| **M2 → M3** | All zones playable; all organs implemented; synthesis puzzle prototyped | GO / EXTEND |
| **M3 → M4** | All endings implemented; Sub-Level 0 complete; performance certified | GO / POLISH |
| **Launch** | Platform certification passed; localization QA passed; marketing ready | SHIP / DELAY |

---

*Last Updated: 2026-11-13 | Next Review: 2026-11-20 (End of Week 1)*