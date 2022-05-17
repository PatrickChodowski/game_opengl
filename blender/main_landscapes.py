import bpy
import importlib.util

spec_landscapes = importlib.util.spec_from_file_location("landscapes", 
"/home/patrick/Documents/projects/game_opengl/blender/landscapes.py")
ld = importlib.util.module_from_spec(spec_landscapes)
spec_landscapes.loader.exec_module(ld)

ld.run()
