
import importlib.util
import bpy
import bmesh
spec_dupa = importlib.util.spec_from_file_location("dupa", 
"/home/patrick/Documents/projects/game_opengl/blender/dupa.py")
dupa = importlib.util.module_from_spec(spec_dupa)
spec_dupa.loader.exec_module(dupa)\

spec_grids = importlib.util.spec_from_file_location("grids", 
"/home/patrick/Documents/projects/game_opengl/blender/grids.py")
grids = importlib.util.module_from_spec(spec_grids)
spec_grids.loader.exec_module(grids)

spec_humans = importlib.util.spec_from_file_location("humans", 
"/home/patrick/Documents/projects/game_opengl/blender/humans.py")
humans = importlib.util.module_from_spec(spec_humans)
spec_humans.loader.exec_module(humans)



#################################
## GRID COLLECTION OF ITEM
#################################

#dupa.make_new_coll("axe")

# hook = grids.Hook("Cylinder", [0.01, 0.66, 0.34])
# hook.get_id()

# ## make grid:
# weapons_grid = grids.Grid(id=0, 
#                     transforms=grids.w_trans, 
#                     anims=grids.w_anims,
#                     camera_pos=[3,4,4], 
#                     w=600,
#                     h=600,
#                     type="weapon",
#                     hook=hook)

# weapons_grid = dupa.make_objects_from_grid("axe", weapons_grid)



#dupa.generate_data(grids.axe_txt, weapons_grid)
#dupa.focus_camera_on_grid()
#dupa.save("axe3", weapons_grid.w, weapons_grid.h)


# make sure axe is in collection
# align to y,z and set perspective to X
# resize to fit in 1x1 -> keep center of the object in the center of the square to make sure all rotations will fit
# origin tile in 0.0 (y,z)
# get hook -> pick vertex and get its global location rounded to 2 decimal points
# provide hook and coll name to main script
    


#################################
## MAKING NEW CHARACTER
#################################
humans.make_character("f_la01")
sk_name = humans.get_skeleton_name()
humans.set_tpose(sk_name)
final_sk_name = humans.finalize()
anim_obj_name = humans.import_animation("Taunt")
humans.retarget(anim_obj_name, final_sk_name)
humans.render_anim(humans.get_body_name())

