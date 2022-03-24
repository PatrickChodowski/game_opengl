

from numpy import delete
import bpy
import os 
from typing import List, Optional, Tuple, Dict
from mathutils import Vector
import math

ANIM_PATH = "/home/patrick/Documents/projects/game_opengl/blender/anims/"
ANIM_LIST = ['unarmed walk forward', 'standing idle']

def clearout() -> None:
  """
  Delete named empty, named camera etc.
  """
  delete_list = ['MainCamera', 'MainEmpty']
  bpy.ops.object.select_all(action='DESELECT')
  for obj in bpy.data.objects:
    if obj.name in delete_list:
      obj.select_set(True)
  bpy.ops.object.delete() 


def make_camera_loc_grid(distance: float = 3.5) -> Dict:
  """
  Make camera position grid
  :param distance: Distance for the Axis to form isosceles triangle with empty (rownoramienny)
  return dictionary grid
  """
  z = distance
  angle_distance = math.sqrt(math.pow(distance, 2)/2)
  grid = {"front": {"x": 0, "y": -distance, "z": z},
          "back":  {"x": 0, "y": distance, "z": z},
          "left":  {"x": distance, "y": 0, "z": z},
          "right": {"x": -distance, "y": 0, "z": z},
          "front_right": {"x": -angle_distance, "y": -angle_distance, "z": z},
          "front_left": {"x": angle_distance, "y": -angle_distance, "z": z},
          "back_left": {"x": angle_distance, "y": angle_distance, "z": z},
          "back_right": {"x": -angle_distance, "y": angle_distance, "z": z},
  }
  return grid





def create_camera(res_x: int = 480, res_y: int = 540) -> None:
  """ 
  Creates new camera. 
  """
  camera_data = bpy.data.cameras.new(name='MainCamera')
  camera_object = bpy.data.objects.new('MainCamera', camera_data)
  bpy.context.scene.collection.objects.link(camera_object)
  bpy.context.scene.camera = camera_object

  bpy.context.scene.render.image_settings.file_format = 'PNG'
  bpy.context.scene.render.film_transparent = True
  bpy.context.scene.render.resolution_x = res_x
  bpy.context.scene.render.resolution_y = res_y
  bpy.data.worlds["World"].node_tree.nodes["Background"].inputs[0].default_value = (1, 1, 1, 1)



def set_camera_location(x: Optional[float] = None, y: Optional[float] = None, z: Optional[float] = None) -> None:
  """
  Sets camera location
  """
  if x is not None:
    bpy.data.objects['MainCamera'].location.x = x
  if y is not None:
    bpy.data.objects['MainCamera'].location.y = y
  if z is not None:
    bpy.data.objects['MainCamera'].location.z = z


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
  o.name = "InsideEmpty"
  print(f"Empty created in {mid_x}, {mid_y}, {mid_z}")


def create_center_empty() -> None:
  """
  Creates empty in the middle... of coordinates system (0,0,0)
  """
  o = bpy.data.objects.new("empty", None)
  bpy.context.scene.collection.objects.link(o)
  o.name = "MainEmpty"
  o.empty_display_size = 2
  o.empty_display_type = 'PLAIN_AXES'   
  o.location.x = 0
  o.location.y = 0
  o.location.z = 0


def make_camera_track_empty() -> None:
    """
    Track the empty object that is supposed to be on 0,0,0 point
    """

    ttc = bpy.data.objects['MainCamera'].constraints.new(type='TRACK_TO')
    ttc.target = bpy.data.objects['MainEmpty']

    ttc.track_axis = 'TRACK_NEGATIVE_Z'
    ttc.up_axis = 'UP_Y'


def import_animation(anim_path: str, anim_name: str) -> str:
  '''
  Taunt, Unarmed Jump
  Returns armature object name
  '''

  pre_arms = list()
  post_arms = list()
  pre_obj_arms = list()
  post_obj_arms = list()
  for a in bpy.data.armatures:
      pre_arms.append(a.name)

  for o in bpy.data.objects:
    if "Armature" in o.name:
      pre_obj_arms.append(o.name)

  file_path = f"{anim_path}{anim_name}.fbx"
  bpy.ops.import_scene.fbx(filepath = file_path,
                           use_anim = True,
                           ui_tab="ARMATURE",
                           automatic_bone_orientation=True)
                           #ignore_leaf_bones

  
  for a in bpy.data.armatures:
      post_arms.append(a.name)
    
  for o in bpy.data.objects:
    if "Armature" in o.name:
      post_obj_arms.append(o.name)

  
  arm_name = list(set(post_arms) - set(pre_arms))[0]
  arm_obj_name = list(set(post_obj_arms) - set(pre_obj_arms))[0]

  #rename 
  for o in bpy.data.objects:
    if arm_obj_name == o.name:
      o.name = anim_name

  return arm_obj_name



def render(obj_name: str, anim_name: str, camera_pos: str) -> None:
  bpy.context.scene.render.engine = 'CYCLES'
  bpy.context.scene.cycles.device = 'GPU'
  bpy.context.scene.render.image_settings.file_format = 'PNG'
  bpy.context.scene.render.fps = 24
  parent_path = "/home/patrick/Documents/projects/game_opengl/blender/done"
  bpy.context.scene.render.filepath = f"{parent_path}/{obj_name}/{anim_name.replace(' ','_')}/{camera_pos}"

  frame_range = bpy.data.objects[anim_name].animation_data.action.frame_range

  bpy.context.scene.frame_step = 1
  bpy.context.scene.frame_start = int(frame_range[0])
  bpy.context.scene.frame_end = int(frame_range[1])
  bpy.ops.render.render(animation=True, write_still=True)



def _set_root_bone() -> None:
  """
  Sets root bone on target root bone (hips)
  """
  target_root_bone_name = 'c_root_master'
  for index, bone in enumerate(bpy.context.scene.bones_map):
    if target_root_bone_name in bone.name:
        bpy.context.scene.bones_map_index = index
        bone.set_as_root = True
        break

def retarget(anim_name: str, target_name: str = "rig") -> None:
  """
  Retargets animation to rig
  """

  bpy.context.scene.source_rig = anim_name
  bpy.context.scene.target_rig = target_name
  bpy.context.scene.arp_retarget_in_place = True
  bpy.ops.arp.auto_scale()
  bpy.ops.arp.build_bones_list()
  _set_root_bone()
  bpy.ops.arp.retarget()

