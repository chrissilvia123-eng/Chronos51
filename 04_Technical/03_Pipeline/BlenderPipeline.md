# Chronos 51 — Blender → UE5 Pipeline (Nanite/Lumen)

## 1. Pipeline Overview

### 1.1 Core Philosophy
- **Nanite-First**: Author high-poly, let Nanite handle LOD/streaming
- **Lumen-Ready**: Baked lighting for Present (GPU Lightmass), dynamic for Past
- **USD-Based**: Blender → USD → UE5 for variant sets, cameras, materials
- **QA-Gated**: Every asset passes `blender-asset-qa` before export

### 1.2 Asset Categories & Budgets
| Category | Triangle Budget | Texture Budget | Nanite | LODs | Notes |
|----------|-----------------|----------------|--------|------|-------|
| **Hero Architecture** | 500k-2M | 4k unique + trim | Yes | Auto | Vista Node focal points |
| **Kit Architecture** | 50k-200k | 2k trim sheet | Yes | Auto | Modular walls, floors, ceilings |
| **Hero Props** | 100k-500k | 2k-4k unique | Yes | Auto | Interactables, organ cradles |
| **Kit Props** | 10k-50k | 2k trim sheet | Yes | Auto | Clutter, consoles, pipes |
| **Foliage/Decals** | N/A | 1k-2k atlas | No | Manual | PCG-scattered, decal actors |
| **Characters** | 150k (Xylos) | 4k UDIM | No (Skeletal) | Manual | Rigged, animated |

---

## 2. Blender Workflow (per blender-workflow-recipes)

### 2.1 Hero Asset Pipeline (High → Low Baked)
```bash
# 1. High-poly sculpt/sub-d in Blender
# Name: X_high (e.g., ReactorCore_Console_high)
# Tools: blender-sculpting, blender-hardsurface-modeling

# 2. Auto-retopo for game mesh
python S/blender-retopology-uv/scripts/auto_retopo.py \
  --objects ReactorCore_Console_high \
  --method quadriflow \
  --tris 200000 \
  --output ReactorCore_Console

# 3. UV packing (seams from sharp edges, 16px margins)
python S/blender-retopology-uv/scripts/uv_pack.py \
  --objects ReactorCore_Console \
  --method seams \
  --seams-from-sharp 60 \
  --margin-px 16

# 4. Texel density check (target 512 px/m for kit, 1024 for hero)
python S/blender-game-optimization/scripts/texel_density.py \
  --objects ReactorCore_Console \
  --check 1024

# 5. QA Gate (REQUIRED before baking)
python S/blender-asset-qa/scripts/qa_check.py \
  --objects ReactorCore_Console \
  --profile game
# Fix any: non-manifold, inverted normals, duplicate verts, n-gons, floating parts

# 6. Bake maps from high to low
python S/blender-texture-baking/scripts/bake_maps.py \
  --low ReactorCore_Console \
  --high ReactorCore_Console_high \
  --maps normal ao curvature id \
  --pack-orm \
  --directx \
  --output //textures/T_ReactorCore_Console_

# 7. Build PBR material (measured values, Material Layer ready)
python S/blender-pbr-materials/scripts/build_pbr_material.py \
  --name M_ReactorCore_Console \
  --textures //textures \
  --objects ReactorCore_Console \
  --preset ue5_nanite_layers

# 8. Smart material wear/grime (optional, from curvature/AO)
python S/blender-texturing/scripts/smart_material.py \
  --objects ReactorCore_Console \
  --curvature-map //textures/T_ReactorCore_Console_CURV.png \
  --ao-map //textures/T_ReactorCore_Console_AO.png \
  --wear 0.3 \
  --grime 0.2 \
  --edge-wear 0.4

# 9. Self-bake wear into basecolor/roughness/metallic
python S/blender-texture-baking/scripts/bake_maps.py \
  --low ReactorCore_Console \
  --maps basecolor roughness metallic \
  --output //textures/T_ReactorCore_Console_

# 10. Export via USD (preferred) or FBX
python S/blender-export-pipelines/scripts/export_asset.py \
  --preset unreal_nanite_usd \
  --objects ReactorCore_Console \
  --out //export/ReactorCore_Console.usd
```

### 2.2 Modular Kit Pipeline (Trim Sheet Based)
```bash
# 1. Blockout in UE5 Modeling Mode (real scale, grid snap)
# 2. Hero pieces in Blender → retopo → UV to trim sheet layout
# 3. UV pack to trim sheet (0-1 space, aligned to grid)
python S/blender-retopology-uv/scripts/uv_pack.py \
  --objects SM_Kit_Wall_Straight_400 SM_Kit_Floor_400x400 SM_Kit_Corner_400 \
  --method trim_sheet \
  --trim-layout //textures/TrimSheet_Kit_01_layout.png \
  --margin-px 8

# 4. Texel density: 512 px/m
python S/blender-game-optimization/scripts/texel_density.py \
  --objects SM_Kit_* \
  --set 512

# 5. QA Gate
python S/blender-asset-qa/scripts/qa_check.py \
  --objects SM_Kit_* \
  --profile game

# 6. Export kit as collection (single USD with variants)
python S/blender-export-pipelines/scripts/export_asset.py \
  --preset unreal_nanite_usd \
  --collection Kit_Facility_Standard \
  --out //export/Kit_Facility_Standard.usd
```

---

## 3. Dual-Timeline Asset Variants

### 3.1 Variant Strategy (Single Mesh, Material Layers)
| Asset | Present Variant | Past Variant | Technique |
|-------|-----------------|--------------|-----------|
| Architecture | Damaged, moss, cracks, rust | Pristine, clean, painted | Vertex paint masks → Material Layer blending |
| Machinery | Broken, static, rusted | Functional, animated, clean | Same mesh; Past = SkeletalMesh + Anim |
| Fluids | Stagnant, green, low | Flowing, clear, lit | Niagara (Past) vs Static mesh + shader (Present) |
| Lighting | Dim, shafted, bioluminescent | Bright, fluorescent, clinical | Lumen scene data per timeline |
| Signage | Faded, broken, overgrown | Crisp, illuminated, readable | Texture variant via Material Layer |

### 3.2 Material Layer Setup (UE5)
```cpp
// Master Material: M_Master_Facility
// Layers (in order):
// 1. Base (trim sheet / unique)
// 2. Damage (cracks, chips) - vertex paint R channel
// 3. Moss/Growth - vertex paint G channel  
// 4. Rust/Decay - vertex paint B channel
// 5. Clean/Pristine - vertex paint A channel (Past only)
// 6. Emissive (signs, screens) - separate mask

// Present Instance: Damage=1, Moss=1, Rust=1, Clean=0
// Past Instance: Damage=0, Moss=0, Rust=0, Clean=1
```

### 3.3 Blender Vertex Paint Workflow
```python
# In Blender: Vertex paint masks for each timeline
# R = Damage (Present)
# G = Moss/Growth (Present)  
# B = Rust (Present)
# A = Clean (Past)

# Export vertex colors with mesh
# In UE5: Material Layer Blending reads vertex colors
```

---

## 4. Vista Node Camera Authoring

### 4.1 Blender Camera Setup
```python
# Per Vista Node: Dedicated camera object
# Naming: CAM_Vista_ZoneID_NodeID (e.g., CAM_Vista_ReactorCore_A)

# Camera Settings (match UE5 CineCameraActor):
# - Sensor Width: 36mm (full frame)
# - Focal Length: Per composition (24mm wide, 50mm standard, 85mm detail)
# - F-Stop: 2.8 (DOF control)
# - Focus Distance: Set to focal subject
# - Sensor Height: Auto (16:9)

# Composition Rules:
# - Rule of thirds for puzzle objective
# - Leading lines to interaction point
# - Depth layers: foreground framing, mid puzzle, background vista
# - No clipping of critical geometry
```

### 4.2 Camera Export (USD)
```bash
# Export cameras with transforms
python S/blender-export-pipelines/scripts/export_asset.py \
  --preset unreal_camera_usd \
  --objects CAM_Vista_* \
  --out //export/Cameras.usd

# In UE5: Import USD → CineCameraActors created automatically
# Bind to Vista Node actors via Level Sequence for Phase Shift blend
```

---

## 5. Lighting & Lookdev Pipeline

### 5.1 Present Timeline (Baked + Lumen)
```bash
# 1. Blender Cycles lookdev reference
# - HDRI: Overcast/Interior (match zone mood)
# - Lighting: 3-point + practical emissives
# - Render: 4k EXR for reference

# 2. UE5 GPU Lightmass (Present static geometry)
# - Lightmap UV: Channel 1 (auto-generated on import)
# - Lightmap Resolution: 512-2048 per object (texel density)
# - Static Lights: Rectangular, Spot, Point (baked)
# - Sky Atmosphere: Overcast, low sun, volumetric fog

# 3. Lumen Dynamic (Present movable)
# - Player flashlight, Astra glow, organ abilities
# - Emissive materials: bioluminescent flora, panels
```

### 5.2 Past Timeline (Lumen Dynamic)
```bash
# 1. Blender Cycles lookdev reference
# - HDRI: Studio/Clinical (bright, clean)
# - Lighting: Fluorescent arrays, practical task lights
# - Render: 4k EXR for reference

# 2. UE5 Lumen (Fully Dynamic)
# - Movable Rectangular lights (fluorescent banks)
# - Spot lights (task lighting)
# - Emissive materials (screens, indicators, panels)
# - Sky Atmosphere: Clear, bright, no fog
# - Post Process: Clinical color grade, slight bloom
```

### 5.3 Color Management
- **Blender:** AgX view transform, sRGB textures, Linear EXR renders
- **UE5:** ACES 1.0 SDR, sRGB textures, Linear for Normal/ORM/Roughness/Metalness
- **ORM Packing:** R=Occlusion, G=Roughness, B=Metalness (UE5 standard)
- **Normal Maps:** DirectX (Green channel down) — baked with `--directx` flag

---

## 6. Character Pipeline (Xylos + Astra)

### 6.1 Xylos (Player Character)
```bash
# 1. High-poly sculpt (blender-sculpting)
# - Target: 2M tris (chitin plates, crests, interface nodes)
# - UV: UDIM layout (4 tiles: Body, Head, Hands, Feet)
# - Symmetry: X-axis, asymmetrical details painted

# 2. Retopo (blender-character-rigging)
# - Target: 150k tris (Nanite not for skeletal)
# - Edge flow: Animation-ready (elbows, knees, face)
# - UV: Same UDIM layout transferred

# 3. Rigging (Rigify + Game Skeleton)
python S/blender-character-rigging/scripts/rigify_build.py \
  --add basic_human \
  --fit Xylos_Body \
  --metarig META_Xylos \
  --generate

# 4. Game Skeleton (UE5 compatible)
python S/blender-character-rigging/scripts/rigify_build.py \
  --rig RIG_Xylos \
  --game-skeleton \
  --profile unreal

# 5. Skinning (Heat + Proxy fallback)
python S/blender-character-rigging/scripts/bind_mesh.py \
  --armature GAME_RIG_Xylos \
  --meshes Xylos_Body

# 6. Stress Test
python S/blender-character-rigging/scripts/stress_poses.py \
  --armature GAME_RIG_Xylos

# 7. Export Skeletal
python S/blender-export-pipelines/scripts/export_asset.py \
  --preset unreal_skeletal \
  --objects GAME_RIG_Xylos \
  --out //export/SK_Xylos.fbx
```

### 6.2 Astra (Hard-Light Companion)
```bash
# Procedural in UE5 Niagara (no Blender mesh)
# - Particle system: Constellation shader
# - Shape: Vaguely humanoid, vaguely ship-like
# - Animation: Niagara simulation (orbit, scan, memory, distress, unity)
# - Color: Prismatic gradient driven by EAstraVisualState
```

---

## 7. Quality Assurance Gates (MANDATORY)

### 7.1 Per-Asset QA Checklist
```bash
# Run before EVERY export
python S/blender-asset-qa/scripts/qa_check.py \
  --objects <asset_name> \
  --profile game

# Must pass ALL:
# ✅ No non-manifold geometry
# ✅ No inverted normals
# ✅ No duplicate vertices
# ✅ No n-gons (triangulated)
# ✅ No floating/detached parts
# ✅ No degenerate faces
# ✅ UVs in 0-1 space (or UDIM tiles)
# ✅ No overlapping UVs (except intentional)
# ✅ Texel density within 10% of target
# ✅ Pivot at logical origin (bottom-center for props)
# ✅ Scale = 1.0, Rotation applied
# ✅ Naming convention: SM_ / SK_ / CAM_ / M_ / T_
# ✅ Material slots match UE5 Master Material layers
```

### 7.2 Per-Export Verification
```bash
# After export, re-import to empty Blender scene
python S/blender-pipeline-automation/scripts/import_scene.py \
  --file //export/Asset.usd \
  --verify

# Checks:
# ✅ Dimensions match original (±1cm)
# ✅ Vertex count matches
# ✅ UV islands preserved
# ✅ Vertex colors exported
# ✅ Material slots correct
# ✅ Armature scale = 1.0 (not 100x)
# ✅ Animation clips present (if skeletal)
```

---

## 8. USD Pipeline (Blender → UE5)

### 8.1 USD Export Settings
```python
# blender-export-pipelines/scripts/export_asset.py --preset unreal_nanite_usd
# Includes:
# - Mesh: Triangulated, Nanite-ready
# - Materials: MaterialX (preview) + UE5 Master Material references
# - Variants: Present/Past material assignments
# - Cameras: CineCameraActor compatible
# - Lights: Rectangular/Spot/Point (Movable for Past)
# - Vertex Colors: For Material Layer blending
# - Custom Properties: ZoneID, AssetType, TimelineVariants
```

### 8.2 UE5 USD Import Settings
```ini
; Project Settings → USD
bImportMaterials=true
bImportMeshes=true
bImportCameras=true
bImportLights=true
bImportAnimations=true
bCreatePhysicsAssets=false
bGenerateNaniteMesh=true
bGenerateLumenMeshCards=true
bCreateVirtualTextures=true
MaterialImportMode=UseExistingAssets
DefaultMaterial=/Game/Materials/M_Master_Facility
```

---

## 9. Asset Naming Conventions

### 9.1 Object Naming
| Type | Prefix | Example |
|------|--------|---------|
| Static Mesh (Nanite) | SM_ | SM_ReactorCore_Console, SM_Kit_Wall_Straight_400 |
| Skeletal Mesh | SK_ | SK_Xylos, SK_Astra_Projection |
| Camera | CAM_ | CAM_Vista_ReactorCore_A |
| Material | M_ | M_ReactorCore_Console, M_Master_Facility |
| Texture | T_ | T_ReactorCore_Console_BASE, T_TrimSheet_Kit_01 |
| Blueprint | BP_ | BP_VistaNode, BP_Xylos |
| DataAsset | DA_ | DA_PuzzleDefinition, DA_VistaNode |
| Niagara | NS_ | NS_PhaseShift, NS_Astra_Constellation |
| Level | LVL_ | LVL_ReactorCore_Present, LVL_ReactorCore_Past |
| World Partition Grid | WPG_ | WPG_Architecture, WPG_Props, WPG_Detail |

### 9.2 Texture Suffixes
| Map | Suffix | Color Space |
|-----|--------|-------------|
| Base Color | _BASE | sRGB |
| Normal (DirectX) | _NORM | Linear |
| ORM Packed | _ORM | Linear |
| Emissive | _EMIS | sRGB |
| Opacity/Mask | _MASK | Linear |
| Curvature | _CURV | Linear |
| Ambient Occlusion | _AO | Linear |
| ID Mask | _ID | Linear |

---

## 10. Automation Scripts (Blender Pipeline)

### 10.1 Batch Processing
```python
# blender-pipeline-automation/scripts/batch_process.py
# Process entire folder of .blend files
# python -m blender_pipeline.batch_process --input //assets --output //export --preset unreal_nanite_usd

# Features:
# - Parallel processing (configurable workers)
# - QA gate enforcement (fail = stop)
# - Incremental (only changed files)
# - Manifest generation (asset registry)
# - Size reporting (tris, textures, UV density)
```

### 10.2 Asset Registry (JSON Manifest)
```json
{
  "assets": [
    {
      "name": "SM_ReactorCore_Console",
      "type": "StaticMesh",
      "tris": 184532,
      "uv_tiles": 1,
      "texel_density": 1024,
      "textures": ["BASE", "NORM", "ORM", "EMIS"],
      "material": "M_Master_Facility",
      "variants": ["Present", "Past"],
      "zone": "ReactorCore",
      "export_path": "//export/SM_ReactorCore_Console.usd",
      "last_modified": "2026-11-13T07:30:00Z",
      "qa_status": "PASS"
    }
  ]
}
```

---

## 11. Storage on E Drive
```
E:\Chronos51\06_Blender_Pipeline\
├── assets/
│   ├── characters/
│   │   ├── Xylos/
│   │   └── Astra/
│   ├── architecture/
│   │   ├── hero/
│   │   ├── kit/
│   │   └── vista_nodes/
│   ├── props/
│   │   ├── hero/
│   │   └── kit/
│   └── environments/
│       ├── present/
│       └── past/
├── exports/
│   ├── usd/
│   ├── fbx/
│   └── textures/
├── scripts/
│   ├── custom_blender_tools/
│   └── validation/
├── references/
│   ├── material_library.blend
│   ├── trim_sheets/
│   └── lighting_presets/
└── pipeline_docs/
    ├── naming_conventions.md
    ├── export_checklist.md
    └── troubleshooting.md
```