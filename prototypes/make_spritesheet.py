import bpy
import numpy as np
import math

def make_grid_pos(a: float, b: float, axes: list) -> list:
    # x,y,z -> 0,1,2
    main = list()
    if axes.__len__() == 2:
        for y in np.arange(a, b, 1.0):
            for z in np.arange(a, b, 1.0):
                l = [0.0, 0.0, 0.0]
                l[axes[0]] = y
                l[axes[1]] = z
                main.append(l)
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


def copy_coll_objects(coll_name: str) -> str:
    new_coll_name = make_new_coll(coll_name)
    objs = bpy.data.collections[coll_name].objects
    for obj in objs:
        obj_copy = obj.copy()
        bpy.data.collections[new_coll_name].objects.link(obj_copy)
    return new_coll_name

def make_coll_grid(coll_name: str, n: float, axes: list) -> list:
    pos = make_grid_pos(1.0, n+1.0, axes)
    for p in pos:
        new_coll_name = copy_coll_objects(coll_name)
        move_collection(new_coll_name, *p)
    return pos

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
    
def find_grid_center(pos: list) -> tuple:
    xx = list()
    yy = list()
    zz = list()
    adj_y = 2
    adj_z = 2.9
    for p in pos:
        xx.append(p[0])
        yy.append(p[1])
        zz.append(p[2])
    return (max(xx) - min(xx))/2, ((max(yy)+adj_y) - min(yy))/2, ((max(zz)+adj_z) - min(zz))/2 
        
    
#bpy.data.collections.remove(bpy.data.collections.get('axe.004'))     


def reset_camera():
    bpy.data.objects['Camera'].location.x = 0.0
    bpy.data.objects['Camera'].location.y = 0.0
    bpy.data.objects['Camera'].location.z = 0.0
    bpy.data.objects['Camera'].rotation_euler.x = 0.0
    bpy.data.objects['Camera'].rotation_euler.y = 0.0
    bpy.data.objects['Camera'].rotation_euler.z = 0.0

def align_camera_with_grid(x: float, y: float, z: float, align_on: str = "x"):
    #  further on X axis, Y and Z the same
    reset_camera()
    if align_on == "x":
        x = 2.0

    if align_on == "y":
        y = 2.0

    if align_on == "z":
        z = 2.0

    bpy.data.objects['Camera'].location.x = x
    bpy.data.objects['Camera'].location.y = y
    bpy.data.objects['Camera'].location.z = z
    bpy.data.objects['Camera'].rotation_euler.x = math.radians(90)
    bpy.data.objects['Camera'].rotation_euler.z = math.radians(+90)
    
    
def make_grid_rotations(coll_name: str, axes: list) -> None:
    rots = [-105, -90, -45, -25, 0, 30, 45, 60, 75]
    pos = make_coll_grid(coll_name, 3, axes)
    colls = list_grid_collections(coll_name)
    t = find_grid_center(pos)

    for col, rot in zip(colls, rots):
        rotate_collection(col, x=rot)
    
    align_camera_with_grid(*t) 
    
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
    
make_grid_rotations("axe", axes=[1,2])
#save("axe")