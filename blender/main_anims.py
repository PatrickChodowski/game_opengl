import bpy
import math
import mathutils
import os
import importlib.util
import bpy
import bmesh
spec_anims = importlib.util.spec_from_file_location("anims", 
"/home/patrick/Documents/projects/game_opengl/blender/anims.py")
anims = importlib.util.module_from_spec(spec_anims)
spec_anims.loader.exec_module(anims)

MODEL_NAME = "warrior2"
#anims.clearout()
anims.create_camera()
anims.create_center_empty()
anims.make_camera_track_empty()
camera_grid = anims.make_camera_loc_grid(4)

for anim_name in anims.ANIM_LIST:
  anims.import_animation(anims.ANIM_PATH, anim_name)
  anims.retarget(anim_name, "rig")

  for camera_loc_name, camera_loc in camera_grid.items():
    anims.set_camera_location(camera_loc['x'], camera_loc['y'], camera_loc['z'])
    anims.render(MODEL_NAME, anim_name, camera_loc_name)
