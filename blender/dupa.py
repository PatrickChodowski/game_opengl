import bpy
import numpy as np
import math
import importlib

spec_grids = importlib.util.spec_from_file_location("grids", 
"/home/patrick/Documents/projects/game_opengl/blender/grids.py")
grids = importlib.util.module_from_spec(spec_grids)
spec_grids.loader.exec_module(grids)
 
def get_objects() -> list:
  objs = list()
  objects = bpy.context.editable_objects
  ### get objects that are not Sun and Camera
  for obj in objects:
     if (obj.name_full != "Sun") & (obj.name_full != "Camera"): 
         print(f"Added {obj.name_full} to work objects list")
         objs.append(obj)
  return objs
         
def set_state(obj_list: list, state: bool) -> None:
    msg = ""
    if not state:
        msg = "Un"
    for obj in obj_list:
        obj.select_set(state=state)
        print(f"{msg}selecting object {obj.name_full}")
            
def move_collection(coll_name: str, x: float = None, y: float = None, z: float = None) -> None:
    set_state(bpy.data.collections[coll_name].objects, True)
    bpy.ops.transform.translate(value=[x,y,z], orient_type='GLOBAL')  
    set_state(bpy.data.collections[coll_name].objects, False)

def make_new_coll(coll_name: str) -> str:
    collection = bpy.data.collections.new(coll_name)
    bpy.context.scene.collection.children.link(collection)
    print(f"Making new collection: {collection.name}")
    return collection.name

def get_objs_by_names(obj_names: list) -> list:
    objs = list()
    all_objs = get_objects()
    for obj in all_objs:
        if obj.name in obj_names:
            objs.append(obj)
    return objs
    
def get_object_location(obj_name: str) -> list:
    obj = bpy.data.objects[obj_name]
    loc = [obj.location.x, obj.location.y, obj.location.z]
    print(f"Location of {obj_name}: {loc}")
    return loc

def copy_collection(coll_name: str) -> str:
    new_coll_name = make_new_coll(coll_name)
    objs = bpy.data.collections[coll_name].objects
    for obj in objs:
        obj_copy = obj.copy()
        bpy.data.collections[new_coll_name].objects.link(obj_copy)
    return new_coll_name

def delete_collection(coll_name: str) -> None:
    collection = bpy.data.collections.get(coll_name)
    for obj in collection.objects:
        bpy.data.objects.remove(obj, do_unlink=True)
        
    bpy.data.collections.remove(collection)


def list_grid_collections(coll_name: str) -> list:
    coll_list = list()
    for coll in bpy.data.collections:
        if (coll_name in coll.name) & (".0" in coll.name) & (coll.objects.__len__() > 0):
            coll_list.append(coll.name)
            print(f"Found {coll.name}")
    return coll_list

def get_collection_rotation(coll_name: str):
    for obj in bpy.data.collections[coll_name].objects:
        print(obj.rotation_euler)
        #print(obj.rotation_quaternion)
        
def rotate_collection(coll_name: str, x: int = None, y: int = None, z: int = None) -> None:
    set_state(bpy.data.collections[coll_name].objects, True)
    
    if x is not None:
        print(f"Rotating X by {math.radians(x)}")
        bpy.ops.transform.rotate(value=math.radians(x), orient_axis='X', orient_type='GLOBAL')
                
    if y is not None:
        print(f"Rotating Y by {math.radians(y)}")
        bpy.ops.transform.rotate(value=math.radians(y), orient_axis='Y', orient_type='GLOBAL')
                
    if z is not None:
        print(f"Rotating Z by {math.radians(z)}")
        bpy.ops.transform.rotate(value=math.radians(z), orient_axis='Z', orient_type='GLOBAL')
        
    set_state(bpy.data.collections[coll_name].objects, False)
    
def reset_camera():
    bpy.data.objects['Camera'].location.x = 0.0
    bpy.data.objects['Camera'].location.y = 0.0
    bpy.data.objects['Camera'].location.z = 0.0
    bpy.data.objects['Camera'].rotation_euler.x = 0.0
    bpy.data.objects['Camera'].rotation_euler.y = 0.0
    bpy.data.objects['Camera'].rotation_euler.z = 0.0

def set_camera_pos(x: float, y: float, z: float) -> None:
    reset_camera()
    bpy.data.objects['Camera'].location.x = x
    bpy.data.objects['Camera'].location.y = y
    bpy.data.objects['Camera'].location.z = z
    bpy.data.objects['Camera'].rotation_euler.x = math.radians(90)
    bpy.data.objects['Camera'].rotation_euler.z = math.radians(+90)
    
    
def save(file_name: str, res_x: int, res_y: int) -> None:
    bpy.context.scene.render.image_settings.file_format = 'PNG'
    bpy.context.scene.render.film_transparent = True
    bpy.context.scene.render.resolution_x = res_x
    bpy.context.scene.render.resolution_y = res_y
#    bpy.context.scene.render.resolution_percentage
#    bpy.context.scene.render.pixel_aspect_x
#    bpy.context.scene.render.pixel_aspect_y
    bpy.context.scene.render.filepath = f"/home/patrick/Documents/projects/game_opengl/assets/{file_name}.png"
    bpy.ops.render.render(write_still = 1)
    
# make_grid_rotations("axe", axes=[1,2])
# #save("axe")



def resize_camera_view(x: float, y: float) -> None:
  bpy.context.scene.render.resolution_x = x
  bpy.context.scene.render.resolution_y = y



def make_new_object(coll_name: str, rot: list, move: list) -> None:
  """
  Take collection, align it, rotate and move
  """
  new_coll_name = copy_collection(coll_name)
  rotate_collection(new_coll_name, *rot)
  move_collection(new_coll_name, *move)



def make_objects_from_grid(coll_name: str, grid: grids.Grid) -> None:
  """
  Transforms collection based on the specified grid
  """
  print(f"Transforming {coll_name} by grid ID: {grid.id}")

  for t in grid.transforms:
    make_new_object(coll_name, t.rot, t.move)

  set_camera_pos(*grid.camera_pos)


  



