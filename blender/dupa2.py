
import bpy
import bmesh
import numpy as np
import math
import json
import importlib
from typing import List, Optional, Tuple
from mathutils import Vector
import os

spec_grids = importlib.util.spec_from_file_location("grids", 
"/home/patrick/Documents/projects/game_opengl/blender/grids.py")
grids = importlib.util.module_from_spec(spec_grids)
spec_grids.loader.exec_module(grids)


def create_camera(res_x: int = 960, res_y: int = 1080) -> None:
  """ 
  Creates new camera
  """
  camera_data = bpy.data.cameras.new(name='Camera')
  camera_object = bpy.data.objects.new('Camera', camera_data)
  bpy.context.scene.collection.objects.link(camera_object)
  bpy.context.scene.camera = camera_object

  bpy.context.scene.render.image_settings.file_format = 'PNG'
  bpy.context.scene.render.film_transparent = True
  bpy.context.scene.render.resolution_x = res_x
  bpy.context.scene.render.resolution_y = res_y


def set_camera_location(x: Optional[float] = None, y: Optional[float] = None, z: Optional[float] = None) -> None:
  """
  Sets camera location
  """
  if x is not None:
    bpy.data.objects['Camera'].location.x = x
  if y is not None:
    bpy.data.objects['Camera'].location.y = y
  if z is not None:
    bpy.data.objects['Camera'].location.z = z


def set_camera_at(object_name: str, 
                  offset_x: Optional[float] = None, 
                  offset_y: Optional[float] = None, 
                  offset_z: Optional[float] = None) -> None:
  """
  Sets camera at object by object name. Added optional offsets on each axis
  """
  bpy.data.objects[object_name].select_set(state=True)
  bpy.ops.view3d.camera_to_view_selected()
  bpy.data.objects[object_name].select_set(state=False)

  if offset_x is not None:
    bpy.data.objects['Camera'].location.x += offset_x

  if offset_y is not None:
    bpy.data.objects['Camera'].location.y += offset_y

  if offset_z is not None:
    bpy.data.objects['Camera'].location.z += offset_z



def get_object_bb(object_name: str) -> List[Vector]:
  """
  Retrieve objects bounding box (dimensions)
  """
  bb = bpy.data.objects[object_name].bound_box
  global_bb = [bpy.data.objects[object_name].matrix_world @ Vector(bbox_co[:]) for bbox_co in bb[:]] 
  return global_bb
  

def _get_min(bb: List[Vector]) -> Tuple[float]:
    """
    Get minimum coordinates of bounding box
    """
    min_x = min([bb[i][0] for i in range(0, 8)])
    min_y = min([bb[i][1] for i in range(0, 8)])
    min_z = min([bb[i][2] for i in range(0, 8)])
    return min_x, min_y, min_z

	
def _get_max(bb: List[Vector]) -> Tuple[float]:
    """
    Get maximum coordinates of bounding box
    """
    max_x = max([bb[i][0] for i in range(0, 8)])
    max_y = max([bb[i][1] for i in range(0, 8)])
    max_z = max([bb[i][2] for i in range(0, 8)])
    return max_x, max_y, max_z


def get_center_bb(object_name: str) -> Tuple[float]:
  """
  Get center of the object
  """
  gbb = get_object_bb(object_name)
  min_x, min_y, min_z = _get_min(gbb)
  max_x, max_y, max_z = _get_max(gbb)

  mid_x = (min_x + max_x)/2
  mid_y = (min_y + max_y)/2
  mid_z = (min_z + max_z)/2
  print(f"Center: {mid_x}, {mid_y}, {mid_z}")
  return mid_x, mid_y, mid_z


def empty_inside(object_name: str) -> None:
  """
  Creates empty plain axes object in the middle of the tracked object
  """
  mid_x, mid_y, mid_z = get_center_bb(object_name)
  o = bpy.data.objects.new("empty", None)
  bpy.context.scene.collection.objects.link(o)
  o.empty_display_size = 2
  o.empty_display_type = 'PLAIN_AXES'   
  o.location.x = mid_x
  o.location.y = mid_y
  o.location.z = mid_z
  print(f"Empty created in {mid_x}, {mid_y}, {mid_z}")


def make_camera_track_empty(offset_x: Optional[float] = None, 
                            offset_y: Optional[float] = None, 
                            offset_z: Optional[float] = None) -> None:

    ttc = bpy.data.objects['Camera'].constraints.new(type='TRACK_TO')
    ttc.target = bpy.data.objects['empty']

    ttc.track_axis = 'TRACK_NEGATIVE_Z'
    ttc.up_axis = 'UP_Y'

    kw_args = dict()

    if offset_x is not None:
      kw_args["x"] = offset_x
    else:
      kw_args["x"] = bpy.data.objects['empty'].location.x

    if offset_y is not None:
      kw_args["y"] = offset_y
    else:
      kw_args["y"] = bpy.data.objects['empty'].location.y

    if offset_z is not None:
      kw_args["z"] = offset_z
    else:
      kw_args["z"] = bpy.data.objects['empty'].location.z

    set_camera_location(**kw_args)



def get_name_of_selected_object() -> List[str]:
  """
  Gets name of selected object
  """
  l = [obj.name for obj in bpy.context.selected_objects]
  print(l)
  return l


def set_camera_angle_circle(angle: int = 45, camera_axis_name: str = "y", up_axis: str = "z") -> None:
  """
  Moves camera to the point so it looks at object at given angle
  by default it is set away on Y axis and will move on Z axis by 45 degrees
  We are using circle/triangle method to calculate new coordinates

  angle can be only 45 or 60
  """

  if angle not in [45, 60]:
    raise ValueError(f"Terrible angle {angle}")

  if up_axis != "z":
    raise ValueError(f"Terrible up_axis {up_axis}. Adjust the model so the Z is the vertical one")

  radius = None
  if camera_axis_name == "x":
    radius = math.sqrt((bpy.data.objects['Camera'].location.x - bpy.data.objects['empty'].location.y)**2)

  elif camera_axis_name == "y":
    radius = math.sqrt((bpy.data.objects['Camera'].location.y - bpy.data.objects['empty'].location.y)**2)
  
  elif camera_axis_name == "z":
    radius = math.sqrt((bpy.data.objects['Camera'].location.z - bpy.data.objects['empty'].location.z)**2)

  print(f"Radius from empty to camera: {radius}")

  new_x = radius*math.cos(math.radians(angle))
  new_y = radius*math.sin(math.radians(angle))
  print(f"new point for camera is {new_x}, {new_y}")

  kw_args = {camera_axis_name: new_x, up_axis: new_y}
  print(kw_args)
  set_camera_location(**kw_args)

  # magnificent programming
  if angle == 45:
    bpy.data.objects['empty'].location.z += 0.058
  elif angle == 60:
    bpy.data.objects['empty'].location.z += 0.13

def _move_camera_by_angle(radius: float, angle: int) -> None:
      new_y = bpy.data.objects['empty'].location.y + radius*math.cos(math.radians(angle))
      new_x = bpy.data.objects['empty'].location.x + radius*math.sin(math.radians(angle))
      print(f"new x: {new_x}, new y: {new_y}")
      set_camera_location(x=new_x, y=new_y)
  

def orbit_camera(object_file_name: str = "") -> None:
  """
  Take pictures of different camera angles of the same object
  """
  base_camera_x = bpy.data.objects['Camera'].location.x
  base_camera_y = bpy.data.objects['Camera'].location.y
  # get the radius to the same height
  angles = [0, 45, 90, 135, 180]
  radius = math.sqrt((bpy.data.objects['Camera'].location.x - bpy.data.objects['empty'].location.x)**2 + (bpy.data.objects['Camera'].location.y - bpy.data.objects['empty'].location.y)**2)

  for angle in angles:
      _move_camera_by_angle(radius, angle)
      bpy.context.scene.render.filepath = f"/home/patrick/Documents/projects/game_opengl/blender/done/{object_file_name}_{angle}.png"
      bpy.ops.render.render(write_still = 1)
      set_camera_location(x=base_camera_x, y=base_camera_y)

  



#### BACKUP ####

# import bpy
# import math
# import mathutils
# import os

# import importlib.util
# import bpy
# import bmesh
# spec_dupa = importlib.util.spec_from_file_location("dupa", 
# "/home/patrick/Documents/projects/game_opengl/blender/dupa2.py")
# dupa = importlib.util.module_from_spec(spec_dupa)
# spec_dupa.loader.exec_module(dupa)\


# target_name = 'Skin_1:body'
# #dupa.create_camera()
# #dupa.empty_inside(target_name)
# #dupa.make_camera_track_empty(offset_y = 3)
# #dupa.set_camera_angle_circle(60, "y", "z")


# dupa.orbit_camera("viking_test")



