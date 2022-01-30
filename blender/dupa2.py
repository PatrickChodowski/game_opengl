
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


def set_camera_location(x: float, y: float, z: float) -> None:
  """
  Sets camera location
  """
  bpy.data.objects['Camera'].location.x = x
  bpy.data.objects['Camera'].location.y = y
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
  global_bb = [bpy.context.object.matrix_world @ Vector(bbox_co[:]) for bbox_co in bb[:]] 
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


def make_camera_track_empty() -> None:
    ttc = bpy.data.objects['Camera'].constraints.new(type='TRACK_TO')
    ttc.target = bpy.data.objects['empty']

    ttc.track_axis = 'TRACK_NEGATIVE_Z'
    ttc.up_axis = 'UP_Y'

    set_camera_location(bpy.data.objects['empty'].location.x, 
                        -3,
                        bpy.data.objects['empty'].location.z)



def get_name_of_selected_object() -> List[str]:
  """
  Gets name of selected object
  """
  l = [obj.name for obj in bpy.context.selected_objects]
  print(l)
  return l




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

# dupa.create_camera()

# #target_name = dupa.get_name_of_selected_object()[0]

# target_name = 'Skin_1:body'
# dupa.empty_inside(target_name)
# dupa.make_camera_track_empty()
# dupa.set_camera_at(target_name, offset_y=-0.2)

#dupa.set_camera_location(0, -3, 1)

#target_name = 'Skin_1:body'
##dupa.set_camera_at(target_name)

#target = bpy.data.objects[target_name]
#cam = bpy.data.objects['Camera']
#t_loc_x = target.location.x
#t_loc_y = target.location.y
#cam_loc_x = cam.location.x
#cam_loc_y = cam.location.y


##radius_range = range(2)
#R = (target.location.xy-cam.location.xy).length # Radius

##print(f"Target location xy : {target.location.xy}")
##print(f"Cam location xy : {cam.location.xy}")
#print(f"Radius : {R}")

#init_angle  = (1-2*bool((cam_loc_y-t_loc_y)<0))*math.acos((cam_loc_x-t_loc_x)/R)-2*math.pi*bool((cam_loc_y-t_loc_y)<0) # 8.13 degrees
#target_angle = (math.pi/2 - init_angle) # Go 90-8 deg more
#num_steps = 10 #how many rotation steps

#for r in radius_range:
#    for x in range(num_steps):
#        alpha = init_angle + (x)*target_angle/num_steps
#        cam.rotation_euler[2] = math.pi/2 + alpha #
#        cam.location.x = t_loc_x+math.cos(alpha)*r
#        cam.location.y = t_loc_y+math.sin(alpha)*r

#        # Define SAVEPATH and output filename
#        file = os.path.join('/home/patrick/Documents/projects/game_opengl/blender/done', str(x)+'_'+ str(r)+'_'+str(round(alpha,2))+'_'+str(round(cam.location.x, 2))+'_'+str(round(cam.location.y, 2)))

#        # Render
#        bpy.context.scene.render.filepath = file
#        bpy.ops.render.render(write_still=True)

