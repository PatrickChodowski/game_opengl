
import bpy
import bmesh
from mathutils import Vector
import math
from typing import List, Optional, Tuple, Dict



def create_camera(camera_name: str = 'Camera', res_x: int = 960, res_y: int = 1080) -> None:
  """ 
  Creates new camera
  """
  camera_data = bpy.data.cameras.new(name=camera_name)
  camera_object = bpy.data.objects.new(camera_name, camera_data)
  bpy.context.scene.collection.objects.link(camera_object)
  bpy.context.scene.camera = camera_object

  bpy.context.scene.render.image_settings.file_format = 'PNG'
  bpy.context.scene.render.film_transparent = True
  bpy.context.scene.render.resolution_x = res_x
  bpy.context.scene.render.resolution_y = res_y


  # Not working in landscapes?
  # bpy.data.worlds["World"].node_tree.nodes["Background"].inputs[0].default_value = (1, 1, 1, 1)


def render_png(filepath: str):
  """
    Renders and saves PNG file
  """
  #   bpy.context.scene.render.filepath = f"/home/patrick/Documents/projects/game_opengl/blender/done/{map_name}_{RENDER}/{empty_d['id']}.png"
  bpy.context.scene.render.filepath = filepath
  bpy.ops.render.render(write_still = 1)


def create_plane(plane_name: str, x: float, y: float, z: float, size: float = 1.0) -> None:
  """
    Creates empty plane
  """
  bpy.ops.mesh.primitive_plane_add(
      size=size,
      calc_uvs=True,
      enter_editmode=False,
      align='WORLD',
      location=(x,y,z),
      rotation=(0, 0, 0),
      scale=(0, 0, 0))
  current_name = bpy.context.selected_objects[0].name
  plane = bpy.data.objects[current_name]
  plane.name = plane_name
  plane.data.name = plane_name
  print(f" CREATED plane named: {plane_name}")


def set_object_location(object_name: str = 'Camera', 
                        x: Optional[float] = None, 
                        y: Optional[float] = None, 
                        z: Optional[float] = None) -> None:
  """
  Sets object location
  """
  if object_name in bpy.data.objects:
    if x is not None:
      bpy.data.objects[object_name].location.x = x
    if y is not None:
      bpy.data.objects[object_name].location.y = y
    if z is not None:
      bpy.data.objects[object_name].location.z = z
    print(f" SETTING object {object_name} location to ({x},{y},{z})")


def set_camera_at(object_name: str, 
                  offset_x: Optional[float] = None, 
                  offset_y: Optional[float] = None, 
                  offset_z: Optional[float] = None) -> Dict:
  """
  Sets camera at object by object name. Added optional offsets on each axis
  Returns list of camera x,y,z
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
  
  locs = dict()
  locs['x'] = bpy.data.objects['Camera'].location.x
  locs['y'] = bpy.data.objects['Camera'].location.y
  locs['z'] = bpy.data.objects['Camera'].location.z
  return locs


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
    kw_args['camera_name'] = 'Camera'
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


def make_camera_track_object(camera_name: str, 
                             object_name: str, 
                             offset_x: Optional[float] = None, 
                             offset_y: Optional[float] = None, 
                             offset_z: Optional[float] = None) -> None:

  ttc = bpy.data.objects[camera_name].constraints.new(type='TRACK_TO')
  ttc.target = bpy.data.objects[object_name]
  ttc.track_axis = 'TRACK_NEGATIVE_Z'
  ttc.up_axis = 'UP_Y'
  kw_args = dict()
  kw_args['camera_name'] = camera_name
  if offset_x is not None:
    kw_args["x"] = offset_x
  else:
    kw_args["x"] = bpy.data.objects[object_name].location.x

  if offset_y is not None:
    kw_args["y"] = offset_y
  else:
    kw_args["y"] = bpy.data.objects[object_name].location.y

  if offset_z is not None:
    kw_args["z"] = offset_z
  else:
    kw_args["z"] = bpy.data.objects[object_name].location.z

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
  camera_obj_name = 'Camera'
  base_camera_x = bpy.data.objects[camera_obj_name].location.x
  base_camera_y = bpy.data.objects[camera_obj_name].location.y
  # get the radius to the same height
  angles = [0, 45, 90, 135, 180]
  radius = math.sqrt((bpy.data.objects[camera_obj_name].location.x - bpy.data.objects['empty'].location.x)**2 + (bpy.data.objects[camera_obj_name].location.y - bpy.data.objects['empty'].location.y)**2)

  for angle in angles:
      _move_camera_by_angle(radius, angle)
      bpy.context.scene.render.filepath = f"/home/patrick/Documents/projects/game_opengl/blender/done/{object_file_name}_{angle}.png"
      bpy.ops.render.render(write_still = 1)
      set_object_location(object_name = camera_obj_name, x=base_camera_x, y=base_camera_y)

  
def get_materials() -> List[str]:
  materials = list()
  for m in bpy.data.materials:
    materials.append(m.name)
  return materials
  
  
def clear_animations():
  context = bpy.context
  for ob in bpy.data.objects:
    ob.animation_data_clear()
        
def clear_empties() -> None:
  """
    Remove all empties
  """
  bpy.ops.object.select_all(action='DESELECT')
  for ob in bpy.data.objects:
    if "Empty" in ob.name:
      ob.select_set(True)
  bpy.ops.object.delete()
  bpy.ops.object.select_all(action='DESELECT')


def clear_object_contrains(object_name: str) -> None:
  """
    Removes all constraints from the object
  """
  for ob in bpy.data.objects:
    if ob.name == object_name:
      for c in ob.constraints:
        ob.constraints.remove(c)


def delete_object(object_name: str) -> None:
  """
    Deletes object by its name
  """
  if object_name in bpy.data.objects:
    object_to_delete = bpy.data.objects[object_name]
    bpy.data.objects.remove(object_to_delete, do_unlink=True)
    print(f"DELETED {object_name}")



def clear_cameras() -> None:
  """
    Removes all cameras from the scene
  """
  camera_objects = list()
  for obj in bpy.data.objects:
    if "camera" in obj.name.lower():
      camera_objects.append(obj.name)    
  for cam_obj in camera_objects:
    delete_object(cam_obj)


def clear_planes() -> None:
  """
    Removes all planes from the scene
  """
  plane_objects = list()
  for obj in bpy.data.objects:
    if "plane" in obj.name.lower():
      plane_objects.append(obj.name)    
  for pl_obj in plane_objects:
    delete_object(pl_obj)



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

# target_name = [obj.name for obj in bpy.context.selected_objects][0]
# dupa.create_camera()
# dupa.empty_inside(target_name)
# dupa.make_camera_track_empty(offset_y = 3)
# dupa.set_camera_angle_circle(60, "y", "z")

# dupa.orbit_camera("viking_test")


