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

anims.clearout()
anims.create_camera()
anims.create_center_empty()

