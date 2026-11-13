# Chronos 51 — Blender Pipeline Quick Start

## Prerequisites
- Blender 5.3+
- UE5.3+ project at `E:\Chronos51\05_UE5_Project\Chronos51`
- Python 3.10+ (for validation scripts)

## 1. Blender Project Setup

### 1.1 Unit Scale (CRITICAL)
```
Scene Properties → Units → Unit System: Metric
Unit Scale: 0.01 (1 Blender unit = 1 cm = 1 UE5 unit)
Length: Centimeters
Mass: Kilograms
Time: Seconds
```

### 1.2 Color Management
```
Render Properties → Color Management:
Display Device: sRGB
View Transform: AgX
Look: None (or AgX Base Contrast)
Sequencer: None
```

### 1.3 Template Scene Setup
Create `E:\Chronos51\06_Blender_Pipeline\references\material_library\Chronos51_Template.blend` with:
- Unit scale configured
- Master Material node group (see below)
- Trim sheet layout guide
- Camera rig for Vista Nodes
- Lighting presets (Present/Past)

---

## 2. Master Material (Node Group: `Chronos51_Master_Facility`)

### 2.1 Input Sockets
| Socket | Type | Default | Purpose |
|--------|------|---------|---------|
| BaseColor | Color | #808080 | Albedo (sRGB) |
| Normal | Vector | (0,0,1) | Normal map (DirectX) |
| ORM | Color | (1,1,0) | R=Occlusion, G=Roughness, B=Metalness |
| Emissive | Color | #000000 | Emissive (sRGB) |
| Opacity | Float | 1.0 | Alpha mask |
| Vertex_Damage | Float | 0.0 | Vertex paint R channel |
| Vertex_Moss | Float | 0.0 | Vertex paint G channel |
| Vertex_Rust | Float | 0.0 | Vertex paint B channel |
| Vertex_Clean | Float | 0.0 | Vertex paint A channel |

### 2.2 Layer Blending Logic
```
Base Layer (Trim Sheet / Unique Texture)
    ↓
Damage Layer (Vertex_Damage) → Multiply → Add to BaseColor (darken), Increase Roughness
    ↓
Moss Layer (Vertex_Moss) → Mix → Override BaseColor (green), Decrease Metalness
    ↓
Rust Layer (Vertex_Rust) → Mix → Override BaseColor (orange-brown), Increase Roughness
    ↓
Clean Layer (Vertex_Clean) → Invert → Mask out Damage/Moss/Rust (Past timeline)
    ↓
Output → Principled BSDF
```

### 2.3 UE5 Material Parameters (Exported via MaterialX)
```glsl
// In UE5: M_Master_Facility
// Material Layers:
// 1. Base (Required)
// 2. Damage (Optional, vertex paint R)
// 3. Moss (Optional, vertex paint G)  
// 4. Rust (Optional, vertex paint B)
// 5. Clean (Optional, vertex paint A - Past only)
// 6. Emissive (Optional, separate mask)
```

---

## 3. Asset Creation Workflow

### 3.1 Hero Asset (High → Low Baked)
```bash
# 1. High-poly in Blender
# Name: ReactorCore_Console_high
# Tris: 2-5M (sculpt/sub-d)

# 2. Auto-retopo
python -m blender_retopology_uv auto_retopo \
  --objects ReactorCore_Console_high \
  --method quadriflow \
  --tris 200000 \
  --output ReactorCore_Console

# 3. UV Pack
python -m blender_retopology_uv uv_pack \
  --objects ReactorCore_Console \
  --method seams \
  --seams-from-sharp 60 \
  --margin-px 16

# 4. Texel Density Check (Hero: 1024 px/m)
python -m blender_game_optimization texel_density \
  --objects ReactorCore_Console \
  --check 1024

# 5. QA Gate (MANDATORY)
python -m blender_asset_qa qa_check \
  --objects ReactorCore_Console \
  --profile game

# 6. Bake Maps
python -m blender_texture_baking bake_maps \
  --low ReactorCore_Console \
  --high ReactorCore_Console_high \
  --maps normal ao curvature id \
  --pack-orm \
  --directx \
  --output //textures/T_ReactorCore_Console_

# 7. Smart Material (Optional wear)
python -m blender_texturing smart_material \
  --objects ReactorCore_Console \
  --curvature-map //textures/T_ReactorCore_Console_CURV.png \
  --ao-map //textures/T_ReactorCore_Console_AO.png \
  --wear 0.3 \
  --grime 0.2 \
  --edge-wear 0.4

# 8. Self-bake wear into basecolor/roughness/metallic
python -m blender_texture_baking bake_maps \
  --low ReactorCore_Console \
  --maps basecolor roughness metallic \
  --output //textures/T_ReactorCore_Console_

# 9. Export USD
python -m blender_export_pipelines export_asset \
  --preset unreal_nanite_usd \
  --objects ReactorCore_Console \
  --out //export/ReactorCore_Console.usd
```

### 3.2 Modular Kit Asset (Trim Sheet)
```bash
# 1. Model kit pieces to grid (50cm/1m snap)
# Pivots: Bottom-center (walls/floors), Corner (corners), Center (props)

# 2. UV to Trim Sheet (0-1 space, aligned to grid cells)
python -m blender_retopology_uv uv_pack \
  --objects SM_Kit_Wall_Straight_400 SM_Kit_Floor_400x400 SM_Kit_Corner_400 \
  --method trim_sheet \
  --trim-layout //textures/TrimSheet_Kit_01_layout.png \
  --margin-px 8

# 3. Texel Density: 512 px/m
python -m blender_game_optimization texel_density \
  --objects SM_Kit_* \
  --set 512

# 4. QA Gate
python -m blender_asset_qa qa_check \
  --objects SM_Kit_* \
  --profile game

# 5. Export Kit Collection
python -m blender_export_pipelines export_asset \
  --preset unreal_nanite_usd \
  --collection Kit_Facility_Standard \
  --out //export/Kit_Facility_Standard.usd
```

### 3.3 Vista Node Camera
```python
# In Blender: Create camera for each Vista Node
# Naming: CAM_Vista_ZoneID_NodeID (e.g., CAM_Vista_ReactorCore_A)

# Settings (match UE5 CineCameraActor):
# - Sensor Width: 36mm (full frame)
# - Focal Length: 24mm (wide), 35mm (standard), 50mm (detail), 85mm (portrait)
# - F-Stop: 2.8
# - Focus Distance: Set to focal subject
# - Aspect Ratio: 16:9 (sensor height auto)

# Composition:
# - Rule of thirds for puzzle objective
# - Leading lines to interaction point
# - Depth layers: foreground framing, mid puzzle, background vista
# - No clipping of critical geometry

# Export with scene
python -m blender_export_pipelines export_asset \
  --preset unreal_camera_usd \
  --objects CAM_Vista_* \
  --out //export/Cameras.usd
```

---

## 4. Dual-Timeline Variants

### 4.1 Vertex Paint Masks (Single Mesh, Two Timelines)
```
Vertex Paint Mode:
- Red Channel (R): Damage (Present only)
- Green Channel (G): Moss/Growth (Present only)
- Blue Channel (B): Rust/Decay (Present only)
- Alpha Channel (A): Clean/Pristine (Past only)

Workflow:
1. Paint Present damage/moss/rust on R/G/B
2. Paint Past clean areas on A (inverse of damage)
3. Export vertex colors with mesh
4. In UE5: Material Layer Blending reads vertex colors
```

### 4.2 Material Layer Assignment
| Timeline | Damage (R) | Moss (G) | Rust (B) | Clean (A) |
|----------|------------|----------|----------|-----------|
| **Present** | 1.0 | 1.0 | 1.0 | 0.0 |
| **Past** | 0.0 | 0.0 | 0.0 | 1.0 |

---

## 5. QA Checklist (Run Before EVERY Export)

```bash
python -m blender_asset_qa qa_check \
  --objects <asset_name> \
  --profile game
```

**Must Pass ALL:**
- [ ] No non-manifold geometry
- [ ] No inverted normals
- [ ] No duplicate vertices
- [ ] No n-gons (triangulated on export)
- [ ] No floating/detached parts
- [ ] No degenerate faces
- [ ] UVs in 0-1 space (or UDIM tiles)
- [ ] No overlapping UVs (except intentional trim sheet)
- [ ] Texel density within 10% of target
- [ ] Pivot at logical origin (bottom-center for props)
- [ ] Scale = 1.0, Rotation applied (Ctrl+A → Scale/Rotation)
- [ ] Naming convention: SM_ / SK_ / CAM_ / M_ / T_
- [ ] Material slots match UE5 Master Material layers
- [ ] Vertex colors exported (for timeline variants)

---

## 6. Post-Export Verification

```bash
# Re-import to empty Blender scene
python -m blender_pipeline_automation import_scene \
  --file //export/Asset.usd \
  --verify

# Checks:
# [ ] Dimensions match original (±1cm)
# [ ] Vertex count matches
# [ ] UV islands preserved
# [ ] Vertex colors exported
# [ ] Material slots correct
# [ ] Armature scale = 1.0 (not 100x)
# [ ] Animation clips present (if skeletal)
```

---

## 7. UE5 Import Settings

### 7.1 USD Import
```
Project Settings → USD:
bImportMaterials = True
bImportMeshes = True
bImportCameras = True
bImportLights = True
bImportAnimations = True
bCreatePhysicsAssets = False
bGenerateNaniteMesh = True
bGenerateLumenMeshCards = True
bCreateVirtualTextures = True
MaterialImportMode = UseExistingAssets
DefaultMaterial = /Game/Materials/M_Master_Facility
```

### 7.2 Texture Settings
```
Base Color: sRGB = True
Normal: sRGB = False, Compression = BC5
ORM: sRGB = False, Compression = BC7
Emissive: sRGB = True
Mask/Opacity: sRGB = False
```

### 7.3 Nanite Settings (Static Mesh)
```
Build Nanite: True
Keep Percent Triangles: 100 (or 90 for very high poly)
Fallback Percent Triangles: 5
Fallback Relative Error: 0.01
```

---

## 8. Asset Naming Convention

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
| Past Level | Past_ | Past_ReactorCore |

### Texture Suffixes
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

## 9. Automation Scripts

### 9.1 Batch Export
```bash
# Process entire folder
python -m blender_pipeline.batch_process \
  --input E:/Chronos51/06_Blender_Pipeline/assets \
  --output E:/Chronos51/06_Blender_Pipeline/exports \
  --preset unreal_nanite_usd \
  --workers 4
```

### 9.2 Asset Registry Manifest (Generated)
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

## 10. Troubleshooting

| Issue | Solution |
|-------|----------|
| Scale 100x in UE5 | Blender unit scale not 0.01, or export scale wrong |
| Normals inverted | Recalculate outside (Shift+N) in Blender |
| Normal map green up | Bake with `--directx` flag |
| Material pink in UE5 | Texture not found, check paths and naming |
| Nanite not enabled | Check `bGenerateNaniteMesh=True` in import settings |
| Lumen leaking | Mesh thickness > 10cm, enable Two-Sided Distance Field |
| Vertex colors missing | Enable "Vertex Colors" in USD export |
| Animation not importing | Check armature scale = 1.0, not 100x |

---

## 11. Weekly Pipeline Review

Every Friday:
- [ ] Run batch QA on new assets
- [ ] Update asset registry manifest
- [ ] Profile Nanite/Lumen performance in UE5
- [ ] Update trim sheet if new patterns needed
- [ ] Document any new material layer requirements

---

*Last Updated: 2026-11-13 | Blender 5.3 | UE5.3 | Chronos51 Studios*