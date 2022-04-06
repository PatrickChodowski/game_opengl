
import bpy
import os
  
parent_path = "/home/patrick/Documents/projects/game_opengl/blender/src/warrior3/Textures/Light skin"

texs_d = {'basecolor': 'Base Color', 
          'metallic': 'Metallic', 
          'roughness': 'Roughness', 
          'normal': 'Normal',
          'alpha': 'Alpha'}

parts = {'Mat_Warrior_3_Armour':'armour', 
         'Mat_Warrior_3_Beard': 'beard', 
         'Mat_Warrior_3_Belt': 'belt',
         'Mat_Warrior_3_Body': 'body',
         'Mat_Warrior_3_Boots': 'boots',
         'Mat_Warrior_3_Helmet': 'helmet',
         'Mat_Warrior_3_Eyelash': 'eyelashes'}

for key_part, value_part in parts.items():
  mat_name = key_part
  mat = bpy.data.materials[mat_name]
  mat.use_nodes = True
  bsdf = mat.node_tree.nodes["Principled BSDF"]

  for k,v in texs_d.items():
    filepath = f"{parent_path}/{value_part}/{value_part}_{k}.tga"
    if os.path.exists(filepath):
      texImage = mat.node_tree.nodes.new('ShaderNodeTexImage')
      texImage.image = bpy.data.images.load(filepath)
      mat.node_tree.links.new(bsdf.inputs[v], texImage.outputs['Color'])
