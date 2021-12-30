import bpy
import numpy as np
import math
#cam = bpy.context.scene.camera

def make_grid_pos(a: float, b: float) -> list:
    main = list()
    for y in np.arange(a, b, 1.0):
        for z in np.arange(a, b, 1.0):
            main.append([0.0,y,z])
    return main
            
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
            
def activate(obj_index) -> None:
    print(f"Activating object {objs[obj_index].name_full}")
    bpy.context.view_layer.objects.active = objs[obj_index]
    

def make_copy(objs: list) -> None:
    local_objs = objs
    set_state(local_objs, True)
    transform_dict={"value": (0.0, 1.0, 0.0), 
                    "constraint_axis": (True, True, True), 
                    "orient_type" :'NORMAL'}  
    bpy.ops.object.duplicate_move(TRANSFORM_OT_translate = transform_dict)
    set_state(local_objs, False)
    objs2 = get_objects()
    new_objs = list(set(objs2) - set(local_objs))
    print(f"New objects count: {new_objs.__len__()}")
    for nobj in new_objs:
        print(f"New object: {nobj.name_full}")
    
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
    
    
def objs_move_coll(objs_list: list, coll_name: str) -> None:
    for obj in objs_list:
        for coll in obj.users_collection:
            print(f"Unlinking {obj.name} from {coll.name}")
            bpy.data.collections[coll.name].objects.unlink(obj)
        bpy.data.collections[coll_name].objects.link(obj)
        print(f"Linking {obj.name} to {coll_name}")
        
def get_object_location(obj_name: str) -> list:
    obj = bpy.data.objects[obj_name]
    loc = [obj.location.x, obj.location.y, obj.location.z]
    print(f"Location of {obj_name}: {loc}")
    return loc


def copy_coll(coll_name: str, pos: list) -> None:
    new_coll_name = make_new_coll(coll_name)
    objs = bpy.data.collections[coll_name].objects
    obj_names = [obj.name for obj in objs]
    set_state(objs, True)
    transform_dict={"value": (pos[0], pos[1], pos[2]), 
                    "constraint_axis": (True, True, True), 
                    "orient_type" :'NORMAL'}  
    bpy.ops.object.duplicate_move(TRANSFORM_OT_translate = transform_dict)
    set_state(objs, False)
    objs2 = bpy.data.collections[coll_name].objects
    obj_names2 = [obj.name for obj in objs2]
    new_objs_names = list(set(obj_names2) - set(obj_names))
    new_objs = get_objs_by_names(new_objs_names)
    objs_move_coll(new_objs, new_coll_name)

def make_coll_grid(coll_name: str, n: float) -> None:
    pos = make_grid_pos(1.0, n+1.0)
    for p in pos:
        copy_coll("axe", p)


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
     
#bpy.data.collections.remove(bpy.data.collections.get('axe.004'))     
def make_grid_rotations(coll_name: str) -> None:
    rots = [-105, -90, -45, -25, 0, 30, 45, 60, 75]
    make_coll_grid(coll_name, 3)
    colls = list_grid_collections(coll_name)

    for col, rot in zip(colls, rots):
        rotate_collection(col, x=rot)
         
make_grid_rotations("axe")

