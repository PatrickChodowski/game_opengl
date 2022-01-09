import importlib.util
spec = importlib.util.spec_from_file_location("dupa", 
"/home/patrick/Documents/projects/game_opengl/blender/dupa.py")
dupa = importlib.util.module_from_spec(spec)
spec.loader.exec_module(dupa)

dupa.get_objects()