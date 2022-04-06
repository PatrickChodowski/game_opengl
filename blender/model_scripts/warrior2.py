
import bpy
import os

model_path = "/home/patrick/Documents/projects/game_opengl/blender/src/warrior2"

mesh_path = f"{model_path}/base_mesh/skin_1.fbx"
textures_path = f"{model_path}/textures/"
arm_name = "root"

# load base mesh fbx

bpy.ops.import_scene.fbx(filepath = mesh_path,
                        use_anim = False,
                        ui_tab="ARMATURE",
                        automatic_bone_orientation=True)

 
texs_d = {'basecolor': 'Base Color', 
          'metallic': 'Metallic', 
          'roughness': 'Roughness', 
          'normal': 'Normal',
          'alpha': 'Alpha'}

parts = {'Mat_warrior_2_armour':'armour', 
         'Mat_warrior_2_Beard': 'beard', 
         'Mat_warrior_2_belt': 'belt',
         'Mat_warrior_2_body': 'body',
         'Mat_warrior_2_boots': 'boots',
         'Mat_warrior_2_Helmet': 'helmet',
         'Mat_warrior_2_eyelashes': 'eyelashes',
         'Mat_warrior_2_Fur': 'fur',
         'Mat_warrior_2_skeleton': 'skeleton',
         'pasted__Mat_necklace': 'necklace'
         }

for key_part, value_part in parts.items():
  mat_name = key_part
  mat = bpy.data.materials[mat_name]
  mat.use_nodes = True
  bsdf = mat.node_tree.nodes["Principled BSDF"]

  for k,v in texs_d.items():
    filepath = f"{textures_path}/{value_part}/{value_part}_{k}.tga"
    if os.path.exists(filepath):
      texImage = mat.node_tree.nodes.new('ShaderNodeTexImage')
      texImage.image = bpy.data.images.load(filepath)
      mat.node_tree.links.new(bsdf.inputs[v], texImage.outputs['Color'])


## hair colors:
# Crayola's Gold (230, 190, 138)
# Liver's dogs RGB: (94, 50, 31)
# Golden Brown (153, 101, 21)
# violin brown RGB: (99, 56, 0)


### cleaning object:
"""
- remove empties
- join all meshes into 1
- remove root
- scale down object
- rotate over z axis
- save and make copy
"""

# AMBIENT OCCLUSION