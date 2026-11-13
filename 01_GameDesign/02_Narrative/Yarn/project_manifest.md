# Chronos 51 — Yarn Spinner Project Manifest
# Generated 2026-11-13
# All dialogue files for the complete game

# Global Systems (Required - Load First)
- globals/globals.yarn      # Global variables
- globals/functions.yarn    # Global functions  
- globals/commands.yarn     # Engine-bound commands

# Act 1: Arrival & Discovery (Sprints 1-6)
- act1/entry_hangar.yarn    # Tutorial, Containment Breach, Orientation Node
- act1/reactor_core.yarn    # Power Routing, Dampener Sync, Corwin encounter, Chronos Core

# Act 2: Restoration & Revelation (Sprints 7-14)
- act2/bio_containment.yarn # Neural Calibration, Purge Protocol, Neural Loom
- act2/comms_array.yarn     # Signal Triangulation, Archive Decryption, Void Sail

# Act 3: Synthesis & Resolution (Sprints 15-24)
- act3/hangar18.yarn        # Gravity Calibration, Stasis Recovery, Gravity Anchor
- act3/administration.yarn  # Director Auth, Fracture Containment, Astra Core
- act3/sublevel0.yarn       # 5-Stage Organ Synthesis, 4 Endings

# Side Content (Optional - Load Anytime)
- side/optional_content.yarn # Audio logs, memory fragments, NG+ content, accessibility

# Load Order in UE5:
# 1. Globals (auto-loaded by DialogueRunner)
# 2. Act 1 (Entry Hangar → Reactor Core)
# 3. Act 2 (Bio-Containment → Comms Array)  
# 4. Act 3 (Hangar 18 → Administration → Sub-Level 0)
# 5. Side content (streamed on demand)

# Localization Keys Format:
# DLG_[ACT]_[ZONE]_[NODE]_[LINE]
# CLUE_[PUZZLE]_[STEP]
# HINT_[PUZZLE]_[TIER]
# LOG_[CHARACTER]_[YEAR]
# FRAGMENT_[NUMBER]_[THEME]

# Voice-Over Naming:
# VO_[ACT]_[ZONE]_[NODE]_[LINE]_[CHARACTER]
# Example: VO_ACT1_REACTORCORE_A_01_ASTRA

# Total Estimated Dialogue Lines: ~450
# Total Estimated VO Minutes: ~35
# Languages: EN (base), ES, FR, DE, JA, KO, ZH (planned)