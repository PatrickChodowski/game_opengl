import bpy
import bmesh
import numpy as np
import math
import json
import importlib
from typing import List
from mathutils import Vector
import os
from PIL import Image

spec_grids = importlib.util.spec_from_file_location("grids", 
"/home/patrick/Documents/projects/game_opengl/blender/grids.py")
grids = importlib.util.module_from_spec(spec_grids)
spec_grids.loader.exec_module(grids)
 
def get_objects() -> List:
  objs = list()
  objects = bpy.context.editable_objects
  ### get objects that are not Sun and Camera
  for obj in objects:
     if (obj.name_full != "Sun") & (obj.name_full != "Camera"): 
         print(f"Added {obj.name_full} to work objects list")
         objs.append(obj)
  return objs
         
def set_state(obj_list: List, state: bool) -> None:
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

def get_objs_by_names(obj_names: List) -> List:
    objs = list()
    all_objs = get_objects()
    for obj in all_objs:
        if obj.name in obj_names:
            objs.append(obj)
    return objs
    
def get_object_location(obj_name: str) -> List:
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


def list_grid_collections(coll_name: str) -> List:
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
    
def resize_camera_view(x: float, y: float) -> None:
  bpy.context.scene.render.resolution_x = x
  bpy.context.scene.render.resolution_y = y

def make_new_object(coll_name: str, rot: List, move: List) -> str:
  """
  Take collection, align it, rotate and move
  Returns ID -> 001, 002, 003 etc.
  """
  new_coll_name = copy_collection(coll_name)
  coll_id = new_coll_name.split('.')[1]
  rotate_collection(new_coll_name, *rot)
  move_collection(new_coll_name, *move)
  return coll_id

def make_objects_from_grid(coll_name: str, grid: grids.Grid) -> grids.Grid:
  """
  Transforms collection based on the specified grid
  Returns updated grid
  """
  print(f"Transforming {coll_name} by grid ID: {grid.id}")

  for t in grid.transforms:
    t.coll_id = make_new_object(coll_name, t.rot, t.move)
  set_camera_pos(*grid.camera_pos)
  return grid

def set_camera_topdown() -> None:
  # one of options: 30, 45, 60 degrees
  print("")

def get_object_bbox_center(obj_name: str) -> None:
  o = bpy.data.objects[obj_name]
  local_bbox_center = 0.125 * sum((Vector(b) for b in o.bound_box), Vector())
  global_bbox_center = o.matrix_world @ local_bbox_center
  print(global_bbox_center)

def get_global_obj_vertex_pos(obj_name: str, vertex_id: int) -> List[float]:
    o = bpy.data.objects[obj_name]
    mat = o.matrix_world
    pos = list()
    for v in o.data.vertices:
      if v.index == vertex_id:
        global_pos = mat @ v.co
        pos.append(global_pos[0])
        pos.append(global_pos[1])
        pos.append(global_pos[2])
        return pos



def generate_data(txt: grids.Texture, grid: grids.Grid) -> None:
  path = f"/home/patrick/Documents/projects/game_opengl/blender/{txt.id}_{txt.name}.json"
  d = dict()
  d["id"] = txt.id
  d["w"] = grid.w
  d["h"] = grid.h
  d["type"] = grid.type
  d["name"] = txt.name
  d["hook_vertex_id"] = grid.hook.vertex_id

  # frame transformations
  _frames = list()
  for t in grid.transforms:
    x_start = ((t.move[1]/grid.max_y)*grid.w) - txt.frame_width
    y_start = grid.h - (((t.move[2]/grid.max_z)*grid.h) - txt.frame_height) - txt.frame_height
    frame_d = dict()
    frame_d["frame_id"] = t.id
    frame_d["x"] = x_start
    frame_d["y"] = y_start
    frame_d["w"] = txt.frame_width
    frame_d["h"] = txt.frame_height
    frame_d["label"] = t.tag

    # add hook:
    hook_obj_name = grid.hook.object_name+"."+t.coll_id
    hook_pos = get_global_obj_vertex_pos(hook_obj_name, grid.hook.vertex_id)
    frame_d["hook_x"] = round((hook_pos[1] - 1)  * txt.frame_width) - x_start
    frame_d["hook_y"] = grid.h - (round((hook_pos[2] - 1)  * txt.frame_height)) - y_start

    _frames.append(frame_d)

  d["frames_list"] = _frames

  a_opts = ['frame_id',"direction","r","g","b","a","w","h","x","y","z"]
  _anims = list()
  # animations
  for a in grid.anims:
    anim_d = dict()
    anim_d["id"] = a.anim_id
    anim_d["anim_type_id"] = a.anim_type_id
    anim_d["update_times"] = a.update_times
    anim_d["time_length"] = a.time_length
    anim_d["next_anim_id"] = a.next_anim_id
    anim_d["breakable"] = a.breakable
    for ao in a_opts:
      if a.__dict__[ao] is not None:
        anim_d[ao] = a.__dict__[ao]
    _anims.append(anim_d)
    
  d["anim_list"] = _anims
  with open(path, "w") as outfile:
    json.dump(d, outfile)


def create_temp_plane(endf: float = 7.0) -> None:
    x= 0.0
    start = 1.0
    vert = [(x, start, endf), (x, endf, endf), (x, start, start), (x, endf, start)]
    fac = [(0, 1, 3, 2)]
    pl_data = bpy.data.meshes.new("TempPlane")
    pl_data.from_pydata(vert, [], fac)
    pl_obj = bpy.data.objects.new("TempPlane", pl_data)
    col = bpy.data.collections.get("Collection")
    col.objects.link(pl_obj)

def delete_temp_plane() -> None:
  bpy.data.objects.remove(bpy.data.objects["TempPlane"], do_unlink=True)

def focus_camera_on_grid() -> None:
  create_temp_plane()
  bpy.data.objects["TempPlane"].select_set(state=True)
  bpy.ops.view3d.camera_to_view_selected()
  bpy.data.objects["TempPlane"].select_set(state=False)
  delete_temp_plane()


def glue_spritesheet(name: str,
                     source_dir: str = "/home/patrick/Documents/projects/game_opengl/blender/tmp", 
                     dest_dir: str = "/home/patrick/Documents/projects/game_opengl/blender/done") -> None:
  """
  Takes input directory and glues all images inside together into single spritesheet
  """
  #image_dir = "/home/patrick/Documents/projects/game_opengl/blender/tmp"
  images = list()
  for img in os.listdir(source_dir):
      if img.endswith(".png"):
          images.append(Image.open(f"{source_dir}/{img}"))
  widths, heights = zip(*(i.size for i in images))
  total_width = sum(widths)
  max_height = max(heights)

  new_im = Image.new('RGBA', (total_width, max_height))
  x_offset = 0
  for im in images:
      new_im.paste(im, (x_offset, 0))
      x_offset += im.size[0]

  new_im.save(f'{dest_dir}/{name}.png')
