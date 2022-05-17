import bpy
import math
import mathutils
import os
import importlib.util
import bpy
import bmesh
spec_landscapes = importlib.util.spec_from_file_location("landscapes", 
"/home/patrick/Documents/projects/game_opengl/blender/landscapes.py")
ld = importlib.util.module_from_spec(spec_landscapes)
spec_landscapes.loader.exec_module(ld)

# bb = ld._get_landscape_bounding_box('Landscape')
# mmd = ld._get_min_max_bb(bb)
# ld._get_empty_positions(mmd)
ld.clear_empties()
ld.generate_empties('Landscape')
# ld.clear_cameras()
# ld.add_camera()
