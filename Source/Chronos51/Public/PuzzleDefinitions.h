// Puzzle Definitions Header
// All puzzles for Chronos 51 - 7 zones, ~18 puzzles total
// Each puzzle: Type, Steps, Astra Commands, Present Changes, Rewards

#pragma once

#include "CoreMinimal.h"
#include "DA_PuzzleDefinition.h"
#include "PuzzleDefinitions.generated.h"

// Zone 1: Entry Hangar (Tutorial)
inline void DefineEntryHangarPuzzles(TArray<FDA_PuzzleDefinition>& OutPuzzles)
{
    FDA_PuzzleDefinition Puzzle;
    
    // Puzzle 1: Containment Breach Protocol
    Puzzle.PuzzleID = "EntryHangar_ContainmentBreach";
    Puzzle.DisplayName = "Containment Breach Protocol";
    Puzzle.Type = EPuzzleType::Sequence;
    Puzzle.ZoneID = "EntryHangar";
    Puzzle.RequiredVistaNodes = {"EntryHangar_Orientation"};
    Puzzle.PrerequisitePuzzles = {};
    
    FDA_PuzzleStep Step1;
    Step1.StepID = "FieldAlpha";
    Step1.Description = "Stabilize Containment Field Alpha";
    Step1.ClueSourceID = "CLUE_FIELD_ALPHA_STABLE";
    Step1.SolutionData = {{"Field", "Alpha"}, {"Action", "Hold"}};
    Step1.RequiredAstraCommand = EAstraCommand::Hold;
    Step1.RewardFragmentID = "";
    
    FDA_PuzzleStep Step2;
    Step2.StepID = "FieldGamma";
    Step2.Description = "Stabilize Containment Field Gamma (Critical)";
    Step2.ClueSourceID = "CLUE_FIELD_GAMMA_CRITICAL";
    Step2.SolutionData = {{"Field", "Gamma"}, {"Action", "Hold"}, {"Window", "0.5"}};
    Step2.RequiredAstraCommand = EAstraCommand::Hold;
    Step2.RewardFragmentID = "";
    
    FDA_PuzzleStep Step3;
    Step3.StepID = "FieldBeta";
    Step3.Description = "Stabilize Containment Field Beta";
    Step3.ClueSourceID = "CLUE_FIELD_BETA_FINAL";
    Step3.SolutionData = {{"Field", "Beta"}, {"Action", "Hold"}};
    Step3.RequiredAstraCommand = EAstraCommand::Hold;
    Step3.RewardFragmentID = "Fragment_1";
    
    Puzzle.Steps = {Step1, Step2, Step3};
    Puzzle.OrganUnlock = EOrganType::None;
    Puzzle.PresentChanges = {
        {"ContainmentDoor_Open", EPresentChangeType::DoorUnlocked, "", 1.0f, 2.0f},
        {"OrientationNode_Active", EPresentChangeType::VistaNodeActivated, "", 1.0f, 0.0f}
    };
    Puzzle.HintTiers = {
        {10, "HINT_CONTAINMENT_1", {}},
        {25, "HINT_CONTAINMENT_2", {"FieldGenerators"}},
        {50, "HINT_CONTAINMENT_3", {"FieldGenerators", "SequencePanel"}}
    };
    Puzzle.EstimatedSolveTimeMinutes = 5;
    
    OutPuzzles.Add(Puzzle);
}

// Zone 2: Reactor Core
inline void DefineReactorCorePuzzles(TArray<FDA_PuzzleDefinition>& OutPuzzles)
{
    FDA_PuzzleDefinition Puzzle;
    
    // Puzzle 2: Power Routing (Vista Node A)
    Puzzle.PuzzleID = "Reactor_PowerRouting";
    Puzzle.DisplayName = "Reactor Stabilization";
    Puzzle.Type = EPuzzleType::Sequence;
    Puzzle.ZoneID = "ReactorCore";
    Puzzle.RequiredVistaNodes = {"ReactorCore_A"};
    Puzzle.PrerequisitePuzzles = {"EntryHangar_ContainmentBreach"};
    
    FDA_PuzzleStep Step1;
    Step1.StepID = "GeneratorAlpha";
    Step1.Description = "Phase Generator Alpha";
    Step1.ClueSourceID = "CLUE_GENERATOR_ALPHA_SPARKS";
    Step1.SolutionData = {{"Generator", "Alpha"}, {"Phase", "Past"}};
    Step1.RequiredAstraCommand = EAstraCommand::Hold;
    Step1.RewardFragmentID = "";
    
    FDA_PuzzleStep Step2;
    Step2.StepID = "GeneratorGamma";
    Step2.Description = "Phase Generator Gamma (Oscilloscope Peak)";
    Step2.ClueSourceID = "CLUE_OSCILLOSCOPE_PEAK";
    Step2.SolutionData = {{"Generator", "Gamma"}, {"Phase", "Present"}, {"Window", "0.3"}};
    Step2.RequiredAstraCommand = EAstraCommand::Hold;
    Step2.RewardFragmentID = "";
    
    FDA_PuzzleStep Step3;
    Step3.StepID = "GeneratorBeta";
    Step3.Description = "Phase Generator Beta (Precision)";
    Step3.ClueSourceID = "CLUE_BETA_PRECISION";
    Step3.SolutionData = {{"Generator", "Beta"}, {"Phase", "Future"}, {"Window", "0.2"}};
    Step3.RequiredAstraCommand = EAstraCommand::Hold;
    Step3.RewardFragmentID = "Fragment_2";
    
    Puzzle.Steps = {Step1, Step2, Step3};
    Puzzle.OrganUnlock = EOrganType::ChronosCore;
    Puzzle.PresentChanges = {
        {"Reactor_WaterDrain", EPresentChangeType::WaterLevel, "", 0.0f, 5.0f},
        {"Reactor_BridgeExtend", EPresentChangeType::BridgeExtended, "", 1.0f, 3.0f},
        {"Reactor_RadiationClear", EPresentChangeType::RadiationCleared, "", 1.0f, 2.0f},
        {"ReactorCore_B_Accessible", EPresentChangeType::DoorUnlocked, "", 1.0f, 1.0f}
    };
    Puzzle.HintTiers = {
        {15, "HINT_POWER_1", {}},
        {35, "HINT_POWER_2", {"GeneratorPanels", "Oscilloscope"}},
        {70, "HINT_POWER_3", {"GeneratorPanels", "Oscilloscope", "PhaseDisplay"}}
    };
    Puzzle.EstimatedSolveTimeMinutes = 8;
    
    OutPuzzles.Add(Puzzle);
    
    // Puzzle 3: Dampener Sync (Vista Node B) - Requires Astra simultaneous Hold
    Puzzle.PuzzleID = "Reactor_DampenerSync";
    Puzzle.DisplayName = "Dampener Synchronization";
    Puzzle.Type = EPuzzleType::Simultaneous;
    Puzzle.ZoneID = "ReactorCore";
    Puzzle.RequiredVistaNodes = {"ReactorCore_B"};
    Puzzle.PrerequisitePuzzles = {"Reactor_PowerRouting"};
    
    FDA_PuzzleStep SyncStep;
    SyncStep.StepID = "DampenerSync";
    SyncStep.Description = "Synchronize Dampener Controls A & B simultaneously (0.5s window)";
    SyncStep.ClueSourceID = "CLUE_DAMPENER_WINDOW";
    SyncStep.SolutionData = {{"NodeA", "true"}, {"NodeB", "true"}, {"Window", "0.5"}, {"RequiresAstra", "true"}};
    SyncStep.RequiredAstraCommand = EAstraCommand::Hold;
    SyncStep.RewardFragmentID = "Fragment_3";
    
    Puzzle.Steps = {SyncStep};
    Puzzle.OrganUnlock = EOrganType::None;
    Puzzle.PresentChanges = {
        {"Reactor_RadiationClear", EPresentChangeType::RadiationCleared, "", 1.0f, 2.0f},
        {"BioContainment_Access", EPresentChangeType::DoorUnlocked, "", 1.0f, 1.0f}
    };
    Puzzle.HintTiers = {
        {20, "HINT_DAMPENER_1", {}},
        {45, "HINT_DAMPENER_2", {"DampenerPanels", "TimerDisplay"}},
        {80, "HINT_DAMPENER_3", {"DampenerPanels", "TimerDisplay", "AstraPosition"}}
    };
    Puzzle.EstimatedSolveTimeMinutes = 6;
    
    OutPuzzles.Add(Puzzle);
}

// Zone 3: Bio-Containment
inline void DefineBioContainmentPuzzles(TArray<FDA_PuzzleDefinition>& OutPuzzles)
{
    FDA_PuzzleDefinition Puzzle;
    
    // Puzzle 4: Neural Calibration (Grants Neural Loom)
    Puzzle.PuzzleID = "BioContainment_NeuralCalibration";
    Puzzle.DisplayName = "Neural Loom Calibration";
    Puzzle.Type = EPuzzleType::LogicGate;
    Puzzle.ZoneID = "BioContainment";
    Puzzle.RequiredVistaNodes = {"BioContainment_Observation", "BioContainment_Calibration"};
    Puzzle.PrerequisitePuzzles = {"Reactor_DampenerSync"};
    
    FDA_PuzzleStep Step1;
    Step1.StepID = "SubjectIdentification";
    Step1.Description = "Identify viable neural subjects from cryo-pods";
    Step1.ClueSourceID = "CLUE_CRYO_VITALS";
    Step1.SolutionData = {{"Subjects", "3,7,12"}, {"Criteria", "NeuralActivity>0.8"}};
    Step1.RequiredAstraCommand = EAstraCommand::Scan;
    Step1.RewardFragmentID = "";
    
    FDA_PuzzleStep Step2;
    Step2.StepID = "SequenceMapping";
    Step2.Description = "Map neural pathways using Astra's Scan at each node";
    Step2.ClueSourceID = "CLUE_NEURAL_PATHWAYS";
    Step2.SolutionData = {{"Pathway", "Alpha-Beta-Gamma-Delta"}, {"Direction", "Bidirectional"}};
    Step2.RequiredAstraCommand = EAstraCommand::Scan;
    Step2.RewardFragmentID = "Fragment_4";
    
    FDA_PuzzleStep Step3;
    Step3.StepID = "CalibrationSync";
    Step3.Description = "Synchronize calibration at Observation and Calibration nodes";
    Step3.ClueSourceID = "CLUE_CALIBRATION_TIMING";
    Step3.SolutionData = {{"NodeA", "Observation"}, {"NodeB", "Calibration"}, {"Window", "1.0"}, {"RequiresAstra", "true"}};
    Step3.RequiredAstraCommand = EAstraCommand::Hold;
    Step3.RewardFragmentID = "Fragment_5";
    
    Puzzle.Steps = {Step1, Step2, Step3};
    Puzzle.OrganUnlock = EOrganType::NeuralLoom;
    Puzzle.PresentChanges = {
        {"BioContainment_ContainmentCleared", EPresentChangeType::RadiationCleared, "", 1.0f, 3.0f},
        {"BioContainment_SpecimenAccess", EPresentChangeType::DoorUnlocked, "", 1.0f, 1.0f},
        {"CommsArray_Access", EPresentChangeType::DoorUnlocked, "", 1.0f, 1.0f}
    };
    Puzzle.HintTiers = {
        {20, "HINT_NEURAL_1", {}},
        {50, "HINT_NEURAL_2", {"CryoPods", "NeuralMap", "CalibrationConsoles"}},
        {90, "HINT_NEURAL_3", {"CryoPods", "NeuralMap", "CalibrationConsoles", "PathwayHighlight"}}
    };
    Puzzle.EstimatedSolveTimeMinutes = 12;
    
    OutPuzzles.Add(Puzzle);
    
    // Puzzle 5: Containment Purge (Environmental)
    Puzzle.PuzzleID = "BioContainment_PurgeProtocol";
    Puzzle.DisplayName = "Containment Purge Protocol";
    Puzzle.Type = EPuzzleType::Environmental;
    Puzzle.ZoneID = "BioContainment";
    Puzzle.RequiredVistaNodes = {"BioContainment_Observation"};
    Puzzle.PrerequisitePuzzles = {"BioContainment_NeuralCalibration"};
    
    FDA_PuzzleStep PurgeStep;
    PurgeStep.StepID = "PurgeSequence";
    PurgeStep.Description = "Activate purge sequence: Vent → Sterilize → Seal → Verify";
    PurgeStep.ClueSourceID = "CLUE_PURGE_PROTOCOL";
    PurgeStep.SolutionData = {{"Sequence", "Vent,Sterilize,Seal,Verify"}, {"AstraCommands", "Scan,Wait,Hold,Recall"}};
    PurgeStep.RequiredAstraCommand = EAstraCommand::Scan;
    PurgeStep.RewardFragmentID = "Fragment_6";
    
    Puzzle.Steps = {PurgeStep};
    Puzzle.OrganUnlock = EOrganType::None;
    Puzzle.PresentChanges = {
        {"BioContainment_PathClear", EPresentChangeType::BridgeExtended, "", 1.0f, 4.0f},
        {"BioContainment_SamplesAccessible", EPresentChangeType::DoorUnlocked, "", 1.0f, 1.0f}
    };
    Puzzle.HintTiers = {
        {15, "HINT_PURGE_1", {}},
        {40, "HINT_PURGE_2", {"PurgeControls", "StatusLights"}},
        {75, "HINT_PURGE_3", {"PurgeControls", "StatusLights", "TimerDisplay"}}
    };
    Puzzle.EstimatedSolveTimeMinutes = 7;
    
    OutPuzzles.Add(Puzzle);
}

// Zone 4: Comms Array
inline void DefineCommsArrayPuzzles(TArray<FDA_PuzzleDefinition>& OutPuzzles)
{
    FDA_PuzzleDefinition Puzzle;
    
    // Puzzle 6: Signal Triangulation (Grants Void Sail)
    Puzzle.PuzzleID = "CommsArray_SignalTriangulation";
    Puzzle.DisplayName = "Signal Triangulation";
    Puzzle.Type = EPuzzleType::LogicGate;
    Puzzle.ZoneID = "CommsArray";
    Puzzle.RequiredVistaNodes = {"CommsArray_Control", "CommsArray_Analysis"};
    Puzzle.PrerequisitePuzzles = {"BioContainment_NeuralCalibration"};
    
    FDA_PuzzleStep Step1;
    Step1.StepID = "DishAlignment";
    Step1.Description = "Align three dishes to triangulate Astra's origin signal";
    Step1.ClueSourceID = "CLUE_DISH_BEARINGS";
    Step1.SolutionData = {{"Dish1", "45.2"}, {"Dish2", "137.8"}, {"Dish3", "284.5"}, {"Target", "OriginSignal"}};
    Step1.RequiredAstraCommand = EAstraCommand::Scan;
    Step1.RewardFragmentID = "Fragment_7";
    
    FDA_PuzzleStep Step2;
    Step2.StepID = "FrequencyMatch";
    Step2.Description = "Match frequency modulation to decode temporal coordinates";
    Step2.ClueSourceID = "CLUE_FREQUENCY_PATTERN";
    Step2.SolutionData = {{"BaseFreq", "7.83"}, {"Modulation", "Schumann"}, {"Harmonics", "3"}};
    Step2.RequiredAstraCommand = EAstraCommand::Hold;
    Step2.RewardFragmentID = "";
    
    FDA_PuzzleStep Step3;
    Step3.StepID = "TransmissionBurst";
    Step3.Description = "Send calibration burst through aligned array";
    Step3.ClueSourceID = "CLUE_BURST_TIMING";
    Step3.SolutionData = {{"Duration", "3.0"}, {"Power", "Maximum"}, {"AstraRelay", "true"}};
    Step3.RequiredAstraCommand = EAstraCommand::Follow;
    Step3.RewardFragmentID = "Fragment_8";
    
    Puzzle.Steps = {Step1, Step2, Step3};
    Puzzle.OrganUnlock = EOrganType::VoidSail;
    Puzzle.PresentChanges = {
        {"CommsArray_DishesAligned", EPresentChangeType::BridgeExtended, "", 1.0f, 5.0f},
        {"CommsArray_SignalClear", EPresentChangeType::RadiationCleared, "", 1.0f, 2.0f},
        {"Hangar18_Access", EPresentChangeType::DoorUnlocked, "", 1.0f, 1.0f}
    };
    Puzzle.HintTiers = {
        {25, "HINT_COMMS_1", {}},
        {60, "HINT_COMMS_2", {"DishControls", "FrequencyDisplay", "SignalStrength"}},
        {100, "HINT_COMMS_3", {"DishControls", "FrequencyDisplay", "SignalStrength", "TargetHighlight"}}
    };
    Puzzle.EstimatedSolveTimeMinutes = 15;
    
    OutPuzzles.Add(Puzzle);
    
    // Puzzle 7: Archive Decryption
    Puzzle.PuzzleID = "CommsArray_ArchiveDecryption";
    Puzzle.DisplayName = "Archive Decryption";
    Puzzle.Type = EPuzzleType::Cipher;
    Puzzle.ZoneID = "CommsArray";
    Puzzle.RequiredVistaNodes = {"CommsArray_Analysis"};
    Puzzle.PrerequisitePuzzles = {"CommsArray_SignalTriangulation"};
    
    FDA_PuzzleStep DecryptStep;
    DecryptStep.StepID = "DecryptLogs";
    DecryptStep.Description = "Decrypt Director's personal logs using recovered cipher keys";
    DecryptStep.ClueSourceID = "CLUE_CIPHER_KEYS";
    DecryptStep.SolutionData = {{"Keys", "Thorne,Reyes,Corwin"}, {"Algorithm", "TemporalXOR"}, {"Logs", "1947-1962"}};
    DecryptStep.RequiredAstraCommand = EAstraCommand::Scan;
    DecryptStep.RewardFragmentID = "Fragment_9";
    
    Puzzle.Steps = {DecryptStep};
    Puzzle.OrganUnlock = EOrganType::None;
    Puzzle.PresentChanges = {
        {"CommsArray_LogsAccessible", EPresentChangeType::DoorUnlocked, "", 1.0f, 1.0f},
        {"Administration_Access", EPresentChangeType::DoorUnlocked, "", 1.0f, 1.0f}
    };
    Puzzle.HintTiers = {
        {20, "HINT_DECRYPT_1", {}},
        {50, "HINT_DECRYPT_2", {"CipherTerminal", "KeyFragments"}},
        {85, "HINT_DECRYPT_3", {"CipherTerminal", "KeyFragments", "DecryptedPreview"}}
    };
    Puzzle.EstimatedSolveTimeMinutes = 10;
    
    OutPuzzles.Add(Puzzle);
}

// Zone 5: Hangar 18
inline void DefineHangar18Puzzles(TArray<FDA_PuzzleDefinition>& OutPuzzles)
{
    FDA_PuzzleDefinition Puzzle;
    
    // Puzzle 8: Gravity Anchor Calibration (Grants Gravity Anchor)
    Puzzle.PuzzleID = "Hangar18_GravityCalibration";
    Puzzle.DisplayName = "Gravity Anchor Calibration";
    Puzzle.Type = EPuzzleType::Physics;
    Puzzle.ZoneID = "Hangar18";
    Puzzle.RequiredVistaNodes = {"Hangar18_StasisBay", "Hangar18_ExtractionPod"};
    Puzzle.PrerequisitePuzzles = {"CommsArray_SignalTriangulation"};
    
    FDA_PuzzleStep Step1;
    Step1.StepID = "FieldStabilization";
    Step1.Description = "Stabilize gravity field around Stasis Bay using anchor emitters";
    Step1.ClueSourceID = "CLUE_GRAVITY_FIELD";
    Step1.SolutionData = {{"Emitters", "4"}, {"TargetStability", "0.95"}, {"Pattern", "Tetrahedral"}};
    Step1.RequiredAstraCommand = EAstraCommand::Hold;
    Step1.RewardFragmentID = "Fragment_10";
    
    FDA_PuzzleStep Step2;
    Step2.StepID = "ExtractionSequence";
    Step2.Description = "Execute extraction pod launch sequence with gravity assist";
    Step2.ClueSourceID = "CLUE_EXTRACTION_TRAJECTORY";
    Step2.SolutionData = {{"Angle", "42.5"}, {"Velocity", "1200"}, {"GravityAssist", "true"}, {"Window", "2.0"}};
    Step2.RequiredAstraCommand = EAstraCommand::Follow;
    Step2.RewardFragmentID = "";
    
    FDA_PuzzleStep Step3;
    Step3.StepID = "AnchorSynchronization";
    Step3.Description = "Synchronize all gravity anchors across both Vista Nodes";
    Step3.ClueSourceID = "CLUE_ANCHOR_SYNC";
    Step3.SolutionData = {{"Nodes", "StasisBay,ExtractionPod"}, {"PhaseLock", "true"}, {"RequiresAstra", "true"}};
    Step3.RequiredAstraCommand = EAstraCommand::Hold;
    Step3.RewardFragmentID = "Fragment_11";
    
    Puzzle.Steps = {Step1, Step2, Step3};
    Puzzle.OrganUnlock = EOrganType::GravityAnchor;
    Puzzle.PresentChanges = {
        {"Hangar18_GravityStable", EPresentChangeType::RadiationCleared, "", 1.0f, 3.0f},
        {"Hangar18_PlatformExtended", EPresentChangeType::BridgeExtended, "", 1.0f, 4.0f},
        {"Administration_Access", EPresentChangeType::DoorUnlocked, "", 1.0f, 1.0f},
        {"SubLevel0_Access", EPresentChangeType::DoorUnlocked, "", 1.0f, 1.0f}
    };
    Puzzle.HintTiers = {
        {30, "HINT_GRAVITY_1", {}},
        {70, "HINT_GRAVITY_2", {"AnchorEmitters", "FieldVisualizer", "TrajectoryDisplay"}},
        {120, "HINT_GRAVITY_3", {"AnchorEmitters", "FieldVisualizer", "TrajectoryDisplay", "SyncIndicators"}}
    };
    Puzzle.EstimatedSolveTimeMinutes = 18;
    
    OutPuzzles.Add(Puzzle);
    
    // Puzzle 9: Stasis Recovery
    Puzzle.PuzzleID = "Hangar18_StasisRecovery";
    Puzzle.DisplayName = "Stasis Recovery Protocol";
    Puzzle.Type = EPuzzleType::Environmental;
    Puzzle.ZoneID = "Hangar18";
    Puzzle.RequiredVistaNodes = {"Hangar18_StasisBay"};
    Puzzle.PrerequisitePuzzles = {"Hangar18_GravityCalibration"};
    
    FDA_PuzzleStep RecoveryStep;
    RecoveryStep.StepID = "WakeSubjects";
    RecoveryStep.Description = "Revive surviving crew from stasis pods using chrono-resonance";
    RecoveryStep.ClueSourceID = "CLUE_STASIS_VITALS";
    RecoveryStep.SolutionData = {{"Pods", "3"}, {"ResonanceFreq", "ChronosCore"}, {"Sequence", "Simultaneous"}};
    RecoveryStep.RequiredAstraCommand = EAstraCommand::Hold;
    RecoveryStep.RewardFragmentID = "Fragment_12";
    
    Puzzle.Steps = {RecoveryStep};
    Puzzle.OrganUnlock = EOrganType::None;
    Puzzle.PresentChanges = {
        {"Hangar18_CrewRevived", EPresentChangeType::DoorUnlocked, "", 1.0f, 1.0f},
        {"Hangar18_TestimonyUnlocked", EPresentChangeType::DoorUnlocked, "", 1.0f, 1.0f}
    };
    Puzzle.HintTiers = {
        {25, "HINT_STASIS_1", {}},
        {60, "HINT_STASIS_2", {"StasisPods", "VitalMonitors", "ResonanceEmitter"}},
        {100, "HINT_STASIS_3", {"StasisPods", "VitalMonitors", "ResonanceEmitter", "SyncTiming"}}
    };
    Puzzle.EstimatedSolveTimeMinutes = 10;
    
    OutPuzzles.Add(Puzzle);
}

// Zone 6: Administration
inline void DefineAdministrationPuzzles(TArray<FDA_PuzzleDefinition>& OutPuzzles)
{
    FDA_PuzzleDefinition Puzzle;
    
    // Puzzle 10: Director's Authorization (Narrative/Logic)
    Puzzle.PuzzleID = "Administration_DirectorAuth";
    Puzzle.DisplayName = "Director's Authorization";
    Puzzle.Type = EPuzzleType::LogicGate;
    Puzzle.ZoneID = "Administration";
    Puzzle.RequiredVistaNodes = {"Administration_DirectorsOffice"};
    Puzzle.PrerequisitePuzzles = {"Hangar18_GravityCalibration", "CommsArray_ArchiveDecryption"};
    
    FDA_PuzzleStep Step1;
    Step1.StepID = "OverrideCodes";
    Step1.Description = "Input three override codes from Thorne, Reyes, Corwin logs";
    Step1.ClueSourceID = "CLUE_OVERRIDE_CODES";
    Step1.SolutionData = {{"Thorne", "ASCENT"}, {"Reyes", "ANCHOR"}, {"Corwin", "CONTINUITY"}};
    Step1.RequiredAstraCommand = EAstraCommand::Scan;
    Step1.RewardFragmentID = "";
    
    FDA_PuzzleStep Step2;
    Step2.StepID = "TemporalKey";
    Step2.Description = "Use Astra to generate temporal authorization key";
    Step2.ClueSourceID = "CLUE_TEMPORAL_KEY";
    Step2.SolutionData = {{"Source", "AstraCore"}, {"Algorithm", "ChronosNeuralVoidGravity"}, {"Duration", "5.0"}};
    Step2.RequiredAstraCommand = EAstraCommand::Hold;
    Step2.RewardFragmentID = "";
    
    Puzzle.Steps = {Step1, Step2};
    Puzzle.OrganUnlock = EOrganType::None;
    Puzzle.PresentChanges = {
        {"Administration_SubLevelAccess", EPresentChangeType::DoorUnlocked, "", 1.0f, 2.0f},
        {"Administration_RecordsUnsealed", EPresentChangeType::DoorUnlocked, "", 1.0f, 1.0f}
    };
    Puzzle.HintTiers = {
        {20, "HINT_AUTH_1", {}},
        {50, "HINT_AUTH_2", {"OverrideTerminal", "LogTerminals", "AstraInterface"}},
        {80, "HINT_AUTH_3", {"OverrideTerminal", "LogTerminals", "AstraInterface", "CodeHighlight"}}
    };
    Puzzle.EstimatedSolveTimeMinutes = 8;
    
    OutPuzzles.Add(Puzzle);
    
    // Puzzle 11: Fracture Containment (Pre-Sub-Level 0)
    Puzzle.PuzzleID = "Administration_FractureContainment";
    Puzzle.DisplayName = "Fracture Containment Protocol";
    Puzzle.Type = EPuzzleType::Simultaneous;
    Puzzle.ZoneID = "Administration";
    Puzzle.RequiredVistaNodes = {"Administration_DirectorsOffice"};
    Puzzle.PrerequisitePuzzles = {"Administration_DirectorAuth"};
    
    FDA_PuzzleStep ContainStep;
    ContainStep.StepID = "ContainFracture";
    ContainStep.Description = "Deploy all four organ resonances simultaneously to stabilize fracture";
    ContainStep.ClueSourceID = "CLUE_FRACTURE_STABILIZATION";
    ContainStep.SolutionData = {{"Organs", "Chronos,Neural,Void,Gravity"}, {"Simultaneous", "true"}, {"Duration", "10.0"}, {"AstraCore", "true"}};
    ContainStep.RequiredAstraCommand = EAstraCommand::Hold;
    ContainStep.RewardFragmentID = "";
    
    Puzzle.Steps = {ContainStep};
    Puzzle.OrganUnlock = EOrganType::AstraCore;
    Puzzle.PresentChanges = {
        {"SubLevel0_Stabilized", EPresentChangeType::RadiationCleared, "", 1.0f, 5.0f},
        {"SubLevel0_PathOpen", EPresentChangeType::BridgeExtended, "", 1.0f, 3.0f}
    };
    Puzzle.HintTiers = {
        {50, "HINT_FRACTURE_1", {}},
        {100, "HINT_FRACTURE_2", {"ResonanceEmitters", "FractureVisualizer", "OrganIndicators"}},
        {200, "HINT_FRACTURE_3", {"ResonanceEmitters", "FractureVisualizer", "OrganIndicators", "TimingGuide"}}
    };
    Puzzle.EstimatedSolveTimeMinutes = 12;
    
    OutPuzzles.Add(Puzzle);
}

// Zone 7: Sub-Level 0 (Final)
inline void DefineSubLevel0Puzzles(TArray<FDA_PuzzleDefinition>& OutPuzzles)
{
    FDA_PuzzleDefinition Puzzle;
    
    // Puzzle 12: Organ Synthesis (Final)
    Puzzle.PuzzleID = "SubLevel0_OrganSynthesis";
    Puzzle.DisplayName = "Organ Synthesis";
    Puzzle.Type = EPuzzleType::Sequence;
    Puzzle.ZoneID = "SubLevel0";
    Puzzle.RequiredVistaNodes = {"SubLevel0_SynthesisA", "SubLevel0_SynthesisB", "SubLevel0_SynthesisC"};
    Puzzle.PrerequisitePuzzles = {"Administration_FractureContainment"};
    
    FDA_PuzzleStep Step1;
    Step1.StepID = "ChronosSynthesis";
    Step1.Description = "Synthesize Chronos Core at Station A - Temporal Anchor";
    Step1.ClueSourceID = "CLUE_SYNTHESIS_CHRONOS";
    Step1.SolutionData = {{"Station", "A"}, {"Organ", "Chronos"}, {"Resonance", "7.83Hz"}, {"Duration", "15.0"}};
    Step1.RequiredAstraCommand = EAstraCommand::Hold;
    Step1.RewardFragmentID = "";
    
    FDA_PuzzleStep Step2;
    Step2.StepID = "NeuralSynthesis";
    Step2.Description = "Synthesize Neural Loom at Station B - Memory Integration";
    Step2.ClueSourceID = "CLUE_SYNTHESIS_NEURAL";
    Step2.SolutionData = {{"Station", "B"}, {"Organ", "Neural"}, {"Fragments", "12"}, {"Duration", "20.0"}};
    Step2.RequiredAstraCommand = EAstraCommand::Hold;
    Step2.RewardFragmentID = "";
    
    FDA_PuzzleStep Step3;
    Step3.StepID = "VoidSynthesis";
    Step3.Description = "Synthesize Void Sail at Station C - Dimensional Bridge";
    Step3.ClueSourceID = "CLUE_SYNTHESIS_VOID";
    Step3.SolutionData = {{"Station", "C"}, {"Organ", "Void"}, {"Frequency", "Schumann"}, {"Duration", "25.0"}};
    Step3.RequiredAstraCommand = EAstraCommand::Hold;
    Step3.RewardFragmentID = "";
    
    FDA_PuzzleStep Step4;
    Step4.StepID = "GravitySynthesis";
    Step4.Description = "Synthesize Gravity Anchor at Station C - Reality Anchor";
    Step4.ClueSourceID = "CLUE_SYNTHESIS_GRAVITY";
    Step4.SolutionData = {{"Station", "C"}, {"Organ", "Gravity"}, {"FieldStrength", "1.0"}, {"Duration", "30.0"}};
    Step4.RequiredAstraCommand = EAstraCommand::Hold;
    Step4.RewardFragmentID = "";
    
    FDA_PuzzleStep Step5;
    Step5.StepID = "AstraCoreSynthesis";
    Step5.Description = "Final Synthesis: Astra Core - Ship Consciousness Restoration";
    Step5.ClueSourceID = "CLUE_SYNTHESIS_ASTRA";
    Step5.SolutionData = {{"AllOrgans", "true"}, {"Trust", ">=8"}, {"Fragments", "12"}, {"Choice", "EndingDeterminant"}};
    Step5.RequiredAstraCommand = EAstraCommand::Hold;
    Step5.RewardFragmentID = "";
    
    Puzzle.Steps = {Step1, Step2, Step3, Step4, Step5};
    Puzzle.OrganUnlock = EOrganType::AstraCore;
    Puzzle.PresentChanges = {
        {"Ship_Restored", EPresentChangeType::Custom, "ShipRestored", 1.0f, 0.0f},
        {"Fracture_Healed", EPresentChangeType::Custom, "FractureHealed", 1.0f, 0.0f}
    };
    Puzzle.HintTiers = {
        {0, "HINT_SYNTHESIS_1", {}},
        {0, "HINT_SYNTHESIS_2", {}},
        {0, "HINT_SYNTHESIS_3", {}}
    };
    Puzzle.EstimatedSolveTimeMinutes = 45;
    
    OutPuzzles.Add(Puzzle);
}

// Master function to get all puzzles
inline TArray<FDA_PuzzleDefinition> GetAllPuzzleDefinitions()
{
    TArray<FDA_PuzzleDefinition> AllPuzzles;
    
    DefineEntryHangarPuzzles(AllPuzzles);
    DefineReactorCorePuzzles(AllPuzzles);
    DefineBioContainmentPuzzles(AllPuzzles);
    DefineCommsArrayPuzzles(AllPuzzles);
    DefineHangar18Puzzles(AllPuzzles);
    DefineAdministrationPuzzles(AllPuzzles);
    DefineSubLevel0Puzzles(AllPuzzles);
    
    return AllPuzzles;
}