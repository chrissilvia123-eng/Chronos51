# Chronos 51 — GitHub Repository Setup

## Repository Initialization

Run from `E:\Chronos51\05_UE5_Project\Chronos51\`:

```powershell
# Initialize Git with LFS
git init
git lfs install

# Configure LFS for large assets
git lfs track "*.usd"
git lfs track "*.fbx"
git lfs track "*.obj"
git lfs track "*.glb"
git lfs track "*.gltf"
git lfs track "*.png"
git lfs track "*.jpg"
git lfs track "*.exr"
git lfs track "*.hdr"
git lfs track "*.wav"
git lfs track "*.ogg"
git lfs track "*.mp3"
git lfs track "*.uasset"
git lfs track "*.umap"
git lfs track "*.upk"

# Add .gitattributes
git add .gitattributes

# Create initial commit
git add .
git commit -m "Initial commit: Chronos 51 UE5 project foundation

- C++ core modules: GameMode, Character (GAS), TimelineManager, SaveSystem, PuzzleManager, VistaNode
- Config: DefaultEngine.ini (Nanite/Lumen/WorldPartition), DefaultInput.ini (Enhanced Input)
- GitHub Actions CI/CD workflow
- Blueprint setup guide
- Project README with sprint plan"

# Add GitHub remote (replace with your repo URL)
git remote add origin https://github.com/YOUR_ORG/Chronos51.git
git branch -M main
git push -u origin main
```

## GitHub Repository Settings

### Branch Protection Rules (main)
```
Settings → Branches → Add rule:
- Branch name pattern: main
- Require pull request reviews: 1
- Require status checks to pass: 
  - build-windows
  - build-linux
  - lint
  - test
- Require branches to be up to date before merging
- Include administrators
```

### Environments
```
Settings → Environments:
1. staging
   - Protection rules: Required reviewers (team leads)
   - Deployment branches: main only
   
2. production
   - Protection rules: Required reviewers (all leads), wait timer 5min
   - Deployment branches: tags only (v*)
```

### Repository Secrets
```
Settings → Secrets and variables → Actions → New repository secret:

EPIC_GAMES_TOKEN: <your Epic Games API token for UAT/BuildCookRun>
DISCORD_WEBHOOK: <Discord webhook URL for build notifications>
STEAM_APP_ID: <Steam app ID for SteamPipe upload>
STEAM_USERNAME: <Steamworks username>
STEAM_PASSWORD: <Steamworks password>
ITCH_API_KEY: <itch.io API key for butler push>
ITCH_USER: <itch.io username>
```

### Repository Variables
```
Settings → Secrets and variables → Actions → Variables:

UE5_VERSION: 5.3
PROJECT_NAME: Chronos51
DEFAULT_BRANCH: main
```

## GitHub Project Board

Create Project (Beta): "Chronos 51 Development"

### Views
1. **Sprint Board** (Kanban): Backlog → Sprint Ready → In Progress → Code Review → QA → Done
2. **Milestone Tracker** (Table): Group by Milestone (M0-M4), filter by Status
3. **Bug Tracker** (Table): Filter by label:bug, sort by priority
4. **Art Pipeline** (Kanban): Blender WIP → QA Gate → USD Export → UE5 Import → Integrated

### Custom Fields
- Sprint (Iteration)
- Milestone (M0-M4)
- Discipline (Code/Art/Design/Narrative/Audio/Tech)
- Priority (P0-P3)
- Story Points (1, 2, 3, 5, 8, 13)
- Definition of Done Checklist

## Labels

```
# Priority
P0 - Critical (blocker)
P1 - High
P2 - Medium
P3 - Low

# Type
bug
feature
tech-debt
art
design
narrative
audio
pipeline
documentation

# Discipline
code
art
design
narrative
audio
qa
production

# Milestone
M0 - Foundation
M1 - Vertical Slice
M2 - Alpha
M3 - Beta
M4 - Gold

# Status
needs-triage
needs-design
blocked
ready-for-qa
```

## GitHub Actions Workflows

Already created at `.github/workflows/ci-cd.yml`:
- **build-windows**: Windows build, editor test, packaging
- **build-linux**: Linux headless build test
- **lint**: C++ clang-tidy, Blueprint lint (if available)
- **test**: Unit tests, gameplay tests
- **package-staging**: Package for staging environment
- **release**: Tagged releases → GitHub Releases + Steam/itch deploy

### Required Self-Hosted Runners
```
Labels: windows-ue5, linux-ue5
Specs: 32GB RAM, 8+ cores, RTX 3080+/A4000+, 500GB SSD
Software: UE5.3+, Visual Studio 2022, Windows SDK, Git LFS
```

## Development Workflow

### Feature Branches
```
git checkout -b feature/short-description
# Work...
git add .
git commit -m "feat: brief description

- Detail 1
- Detail 2

Refs: #issue-number"
git push origin feature/short-description
# Create PR
```

### Commit Convention
```
feat:     New feature
fix:      Bug fix
refactor: Code restructuring
art:      Art asset updates
design:   Design document changes
narrative: Narrative/dialogue changes
pipeline: Pipeline/tooling changes
docs:     Documentation
chore:    Maintenance
test:     Test additions
perf:     Performance optimization
```

### PR Template
```markdown
## Description
Brief summary of changes.

## Type
- [ ] Feature
- [ ] Bug Fix
- [ ] Refactor
- [ ] Art/Asset
- [ ] Design/Doc
- [ ] Pipeline

## Testing
- [ ] Compiles (Windows/Linux)
- [ ] Editor runs (PIE)
- [ ] Unit tests pass
- [ ] Gameplay test: [describe]
- [ ] Art QA gate passes (if art)

## Checklist
- [ ] Code reviewed
- [ ] Lint passes
- [ ] No console errors/warnings
- [ ] Performance budget met
- [ ] Accessibility considered
- [ ] Documentation updated

## Related
Closes #issue-number
```

## Milestone Gates (Automated Checks)

### M0 Gate (Sprint 2 end)
- [ ] `build-windows` passes
- [ ] `build-linux` passes
- [ ] PIE launches without errors
- [ ] Phase Shift: Present → Past → Present works
- [ ] Test cube imports correctly (scale, normals, materials)
- [ ] Save/Load works across Phase Shift

### M1 Gate (Sprint 6 end)
- [ ] All M0 gates +
- [ ] Reactor Core vertical slice playable
- [ ] 2 puzzles: Power Routing + Dampener Sync
- [ ] Chronos Core organ granted
- [ ] Astra Trust system functional
- [ ] Journal/Map UI functional
- [ ] Photo Mode functional
- [ ] Accessibility: reduced motion + instant cuts implemented

### M2 Gate (Sprint 14 end)
- [ ] All M1 gates +
- [ ] All 4 zones playable (Entry, Reactor, Bio, Comms)
- [ ] All 4 organs granted
- [ ] Act 1 narrative complete
- [ ] Fast travel network functional
- [ ] Performance: 60fps @ 1440p/High on target hardware

### M3 Gate (Sprint 24 end)
- [ ] All M2 gates +
- [ ] All 7 zones playable
- [ ] Act 2 + 3 narrative complete
- [ ] All 4 endings implemented
- [ ] New Game+ mode
- [ ] Localization: EN, ES, FR, DE, JA, KO, ZH
- [ ] Steam/itch builds deploy successfully

### M4 Gate (Sprint 28 end)
- [ ] All M3 gates +
- [ ] Zero P0/P1 bugs
- [ ] All platforms certified (PC, potentially consoles)
- [ ] Launch build uploaded to all storefronts
- [ ] Press kit ready
- [ ] Post-launch roadmap defined

## Useful Commands

```powershell
# Sync with upstream
git fetch origin
git rebase origin/main

# Create milestone tag
git tag -a v0.1.0-m0 -m "M0 Foundation Complete"
git push origin v0.1.0-m0

# Clean untracked (careful!)
git clean -fdX

# LFS pull large files
git lfs pull

# Check LFS storage usage
git lfs ls-files -l | awk '{sum+=$1} END {print sum/1024/1024 " MB"}'
```

## Team Onboarding

1. Clone repo with LFS: `git clone --recurse-submodules https://github.com/ORG/Chronos51.git`
2. Run `Setup.bat` (UE5 prerequisite installer)
3. Open `Chronos51.uproject` in UE5.3+
4. Build Development Editor (VS2022)
5. Run `Chronos51.DumpZoneStates` in console to verify systems
6. Read `BLUEPRINT_SETUP_GUIDE.md` for Blueprint creation
7. Read `E:\Chronos51\06_Blender_Pipeline\pipeline_docs\quick_start.md` for art pipeline

---

*Last Updated: 2026-11-13 | Chronos51 Studios*