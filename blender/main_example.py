
import importlib.util
spec_dupa = importlib.util.spec_from_file_location("dupa", 
"/home/patrick/Documents/projects/game_opengl/blender/dupa.py")
dupa = importlib.util.module_from_spec(spec_dupa)
spec_dupa.loader.exec_module(dupa)\

spec_grids = importlib.util.spec_from_file_location("grids", 
"/home/patrick/Documents/projects/game_opengl/blender/grids.py")
grids = importlib.util.module_from_spec(spec_grids)
spec_grids.loader.exec_module(grids)

#dupa.make_new_coll("axe")
#dupa.make_objects_from_grid("axe", grids.weapons_grid)
#dupa.save("axe2", grids.weapons_grid.w, grids.weapons_grid.h)


dupa.generate_data(axe_txt, grids.weapons_grid)