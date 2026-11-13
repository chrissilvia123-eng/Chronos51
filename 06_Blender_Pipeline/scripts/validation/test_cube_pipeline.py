# Blender Pipeline Test Asset — Simple Cube Validation
# Run in Blender 5.3+ Python Console or as script

import bpy
import os

# ============================================================
# CONFIGURATION
# ============================================================
EXPORT_ROOT = r"E:\Chronos51\06_Blender_Pipeline\exports\test_cube"
ASSET_NAME = "SM_Test_Cube"
BLEND_FILE = os.path.join(EXPORT_ROOT, f"{ASSET_NAME}.blend")
USD_FILE = os.path.join(EXPORT_ROOT, f"{ASSET_NAME}.usd")

# ============================================================
# SCENE SETUP (Unit Scale = 0.01 for cm)
# ============================================================
def setup_scene():
    # Unit scale
    bpy.context.scene.unit_settings.system = 'METRIC'
    bpy.context.scene.unit_settings.scale_length = 0.01
    bpy.context.scene.unit_settings.length_unit = 'CENTIMETERS'
    
    # Color management
    bpy.context.scene.view_settings.view_transform = 'AgX'
    bpy.context.scene.view_settings.look = 'None'
    
    # Clean scene
    bpy.ops.object.select_all(action='SELECT')
    bpy.ops.object.delete()
    
    print("Scene configured: Metric, 0.01 scale, AgX")

# ============================================================
# CREATE TEST CUBE (1m = 100cm)
# ============================================================
def create_test_cube():
    # Create 1m cube (100cm)
    bpy.ops.mesh.primitive_cube_add(size=200, location=(0, 0, 100))  # Bottom at 0
    cube = bpy.context.active_object
    cube.name = ASSET_NAME
    
    # Apply transforms
    bpy.ops.object.transform_apply(location=True, rotation=True, scale=True)
    
    # Set origin to bottom center
    bpy.context.view_layer.objects.active = cube
    bpy.ops.object.origin_set(type='ORIGIN_CURSOR', center='MEDIAN')
    bpy.context.scene.cursor.location = (0, 0, 0)
    bpy.ops.object.origin_set(type='ORIGIN_CURSOR')
    
    # Shade smooth
    bpy.ops.object.shade_smooth()
    
    # Add custom normals (weighted normal equivalent)
    bpy.ops.mesh.customdata_custom_splitnormals_add()
    
    print(f"Created {ASSET_NAME}: 1m cube, pivot at bottom-center")
    return cube

# ============================================================
# UV UNWRAP
# ============================================================
def unwrap_uv(obj):
    bpy.context.view_layer.objects.active = obj
    bpy.ops.object.mode_set(mode='EDIT')
    bpy.ops.mesh.select_all(action='SELECT')
    bpy.ops.uv.smart_project(angle_limit=66, island_margin=0.03, area_weight=0.0)
    bpy.ops.object.mode_set(mode='OBJECT')
    
    # Verify UVs in 0-1 space
    uv_layer = obj.data.uv_layers.active
    if uv_layer:
        for poly in obj.data.polygons:
            for loop_idx in poly.loop_indices:
                uv = uv_layer.data[loop_idx].uv
                if uv.x < 0 or uv.x > 1 or uv.y < 0 or uv.y > 1:
                    print(f"WARNING: UV out of 0-1 range: {uv}")
    
    print("UV unwrapped: Smart Project, margin 0.03")

# ============================================================
# VERTEX PAINT (Timeline Variants)
# ============================================================
def paint_vertex_colors(obj):
    # Ensure vertex color layer
    if not obj.data.vertex_colors:
        obj.data.vertex_colors.new(name="TimelineVariants")
    
    vc_layer = obj.data.vertex_colors["TimelineVariants"]
    
    # Paint all vertices:
    # R = Damage (Present)
    # G = Moss (Present)
    # B = Rust (Present)
    # A = Clean (Past)
    for poly in obj.data.polygons:
        for loop_idx in poly.loop_indices:
            # Present: slight damage on top face
            if poly.normal.z > 0.9:  # Top face
                vc_layer.data[loop_idx].color = (0.2, 0.1, 0.05, 0.8)  # Damage, little moss, clean for past
            else:
                vc_layer.data[loop_idx].color = (0.1, 0.05, 0.02, 0.9)  # Minimal damage, mostly clean
    
    print("Vertex colors painted: R=Damage, G=Moss, B=Rust, A=Clean")

# ============================================================
# MATERIALS (Master Facility Material Slots)
# ============================================================
def setup_materials(obj):
    # Create material slots matching M_Master_Facility layers
    mat_names = [
        "M_Master_Facility_Base",      # Base layer (trim sheet / unique)
        "M_Master_Facility_Damage",    # Damage layer (vertex R)
        "M_Master_Facility_Moss",      # Moss layer (vertex G)
        "M_Master_Facility_Rust",      # Rust layer (vertex B)
        "M_Master_Facility_Clean",     # Clean layer (vertex A - Past)
        "M_Master_Facility_Emissive",  # Emissive (optional)
    ]
    
    for i, name in enumerate(mat_names):
        mat = bpy.data.materials.get(name)
        if not mat:
            mat = bpy.data.materials.new(name=name)
            mat.use_nodes = True
            nodes = mat.node_tree.nodes
            links = mat.node_tree.links
            
            # Clear default
            nodes.clear()
            
            # Basic Principled BSDF setup
            output = nodes.new('ShaderNodeOutputMaterial')
            principled = nodes.new('ShaderNodeBsdfPrincipled')
            links.new(principled.outputs['BSDF'], output.inputs['Surface'])
            
            # Set defaults per layer
            if "Base" in name:
                principled.inputs['Base Color'].default_value = (0.5, 0.5, 0.5, 1)
                principled.inputs['Roughness'].default_value = 0.7
                principled.inputs['Metallic'].default_value = 0.0
            elif "Damage" in name:
                principled.inputs['Base Color'].default_value = (0.2, 0.15, 0.1, 1)
                principled.inputs['Roughness'].default_value = 0.9
            elif "Moss" in name:
                principled.inputs['Base Color'].default_value = (0.15, 0.25, 0.1, 1)
                principled.inputs['Roughness'].default_value = 0.8
                principled.inputs['Metallic'].default_value = 0.0
            elif "Rust" in name:
                principled.inputs['Base Color'].default_value = (0.4, 0.2, 0.1, 1)
                principled.inputs['Roughness'].default_value = 0.85
            elif "Clean" in name:
                principled.inputs['Base Color'].default_value = (0.6, 0.6, 0.6, 1)
                principled.inputs['Roughness'].default_value = 0.4
                principled.inputs['Metallic'].default_value = 0.1
            elif "Emissive" in name:
                principled.inputs['Emission Color'].default_value = (0, 0.5, 1, 1)
                principled.inputs['Emission Strength'].default_value = 2.0
            
            output.location = (300, 0)
            principled.location = (0, 0)
        
        obj.data.materials.append(mat)
    
    print(f"Created {len(mat_names)} material slots matching M_Master_Facility layers")

# ============================================================
# QA CHECKS
# ============================================================
def run_qa_checks(obj):
    issues = []
    
    # Check manifold
    bpy.context.view_layer.objects.active = obj
    bpy.ops.object.mode_set(mode='EDIT')
    bpy.ops.mesh.select_all(action='DESELECT')
    bpy.ops.mesh.select_non_manifold()
    bpy.ops.object.mode_set(mode='OBJECT')
    non_manifold = sum(1 for v in obj.data.vertices if v.select)
    if non_manifold > 0:
        issues.append(f"Non-manifold vertices: {non_manifold}")
    
    # Check normals
    bpy.ops.object.mode_set(mode='EDIT')
    bpy.ops.mesh.select_all(action='DESELECT')
    bpy.ops.mesh.select_all(action='SELECT')
    bpy.ops.object.mode_set(mode='OBJECT')
    
    # Check UVs
    if not obj.data.uv_layers:
        issues.append("No UV layers")
    else:
        uv_layer = obj.data.uv_layers.active
        overlapping = 0
        # Simple overlap check (would need more robust in production)
        uvs = []
        for poly in obj.data.polygons:
            for loop_idx in poly.loop_indices:
                uv = uv_layer.data[loop_idx].uv
                uvs.append((round(uv.x, 4), round(uv.y, 4)))
        if len(uvs) != len(set(uvs)):
            issues.append("Potential overlapping UVs")
    
    # Check scale
    if obj.scale != (1.0, 1.0, 1.0):
        issues.append(f"Scale not applied: {obj.scale}")
    
    # Check vertex colors
    if "TimelineVariants" not in obj.data.vertex_colors:
        issues.append("Missing TimelineVariants vertex color layer")
    
    # Check material slots
    expected_slots = 6
    if len(obj.data.materials) != expected_slots:
        issues.append(f"Expected {expected_slots} material slots, got {len(obj.data.materials)}")
    
    # Tri count
    tri_count = sum(len(p.vertices) - 2 for p in obj.data.polygons)
    
    if issues:
        print("QA ISSUES:")
        for issue in issues:
            print(f"  - {issue}")
        return False
    else:
        print(f"QA PASSED: {tri_count} tris, UVs OK, vertex colors OK, materials OK, scale=1.0")
        return True

# ============================================================
# EXPORT USD
# ============================================================
def export_usd(obj, filepath):
    # Select only our object
    bpy.ops.object.select_all(action='DESELECT')
    obj.select_set(True)
    bpy.context.view_layer.objects.active = obj
    
    # USD Export settings for UE5 Nanite
    bpy.ops.wm.usd_export(
        filepath=filepath,
        selected_objects_only=True,
        export_materials=True,
        export_uvmaps=True,
        export_normals=True,
        export_colors=True,  # Vertex colors!
        export_subdivision=False,
        mesh_triangulate=True,
        apply_scale=True,
        verbose=False
    )
    
    print(f"Exported USD: {filepath}")

# ============================================================
# MAIN
# ============================================================
def main():
    os.makedirs(EXPORT_ROOT, exist_ok=True)
    
    setup_scene()
    cube = create_test_cube()
    unwrap_uv(cube)
    paint_vertex_colors(cube)
    setup_materials(cube)
    
    if run_qa_checks(cube):
        export_usd(cube, USD_FILE)
        
        # Save blend file
        bpy.ops.wm.save_as_mainfile(filepath=BLEND_FILE)
        print(f"Saved blend: {BLEND_FILE}")
        
        print("\n=== TEST ASSET COMPLETE ===")
        print(f"Import {USD_FILE} into UE5")
        print("Verify: Scale 100cm=1m, Nanite enabled, Vertex colors imported, 6 material slots")
    else:
        print("\n=== QA FAILED - Fix issues before export ===")

if __name__ == "__main__":
    main()