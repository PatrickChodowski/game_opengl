
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

hook = grids.Hook("Cylinder", [0.01, 0.66, 0.34])
hook.get_id()

# make grid:
weapons_grid = grids.Grid(id=0, 
                    transforms=grids.w_trans, 
                    camera_pos=[8,4,4], 
                    w=576,
                    h=576,
                    type="weapon",
                    hook=hook)


dupa.generate_data(grids.axe_txt, weapons_grid)

# make axe an collection
# align with X
# get measurements 0> here 7x6

#dupa.make_objects_from_grid("axe", grids.weapons_grid)


# make sure axe is in collection
# align to y,z and set perspective to X
# resize to fit in 1x1 -> keep center of the object in the center of the square to make sure all rotations will fit
# origin tile in 0.0 (y,z)
# get hook -> pick vertex and get its global location rounded to 2 decimal points
# provide hook and coll name to main script
