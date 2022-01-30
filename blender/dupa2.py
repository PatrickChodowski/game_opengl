
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


def create_camera() -> None:
  """ 
  Creates new camera
  """
  camera_data = bpy.data.cameras.new(name='Camera')
  camera_object = bpy.data.objects.new('Camera', camera_data)
  bpy.context.scene.collection.objects.link(camera_object)
  bpy.context.scene.camera = camera_object


def set_camera_location(x: float, y: float, z: float) -> None:
  """
  Sets camera location
  """
  bpy.data.objects['Camera'].location.x = x
  bpy.data.objects['Camera'].location.y = y
  bpy.data.objects['Camera'].location.z = z



def set_camera_at(object_name: str) -> None:
  """
  Sets camera at object by object name
  """
  bpy.data.objects[object_name].select_set(state=True)
  bpy.ops.view3d.camera_to_view_selected()
  bpy.data.objects[object_name].select_set(state=False)


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


def get_object_bb(object_name: str) -> None:
  """
  Retrieve objects bounding box (dimensions)
  """
  dimx = bpy.data.objects[object_name].dimensions.x
  dimy = bpy.data.objects[object_name].dimensions.y
  dimz = bpy.data.objects[object_name].dimensions.z

  locx = bpy.data.objects[object_name].location.x
  locy = bpy.data.objects[object_name].location.y
  locz = bpy.data.objects[object_name].location.z

  # local coordinates need to be multiplied by matrix_world to get global one
  bb = bpy.data.objects[object_name].bound_box
  global_bb = [bpy.context.object.matrix_world @ Vector(bbox_co[:]) for bbox_co in bb[:]] 
  print(f"Objects global bb: {global_bb}")
  print(f"Objects locs: {locx},{locy},{locz} ")
  print(f"Objects dims: {dimx},{dimy},{dimz} ")
  

def get_min(object_name: str) -> Tuple:
    """
    Get minimum coordinates of bounding box
    """
    bb = bpy.data.objects[object_name].bound_box
    min_x = min([bb[i][0] for i in range(0, 8)])
    min_y = min([bb[i][1] for i in range(0, 8)])
    min_z = min([bb[i][2] for i in range(0, 8)])
    return min_x, min_y, min_z

	
def get_max(object_name: str) -> Tuple:
    """
    Get maximum coordinates of bounding box
    """
    bb = bpy.data.objects[object_name].bound_box
    max_x = max([bb[i][0] for i in range(0, 8)])
    max_y = max([bb[i][1] for i in range(0, 8)])
    max_z = max([bb[i][2] for i in range(0, 8)])
    return max_x, max_y, max_z


def get_name_of_selected_object() -> List[str]:
  """
  Gets name of selected object
  """
  l = [obj.name for obj in bpy.context.selected_objects]
  print(l)
  return l

def create_temp_bb_plane(object_name: str, align_with: str = 'x') -> None:
  """
  Creates 2D temp plane around selected object dimensions
  :param object_name: Name of the object the BB we want to plane
  :align_with: name of the  axis we want to put the plane (vertically on)
  """
  x= 0.0
  start = 1.0
  vert = [(x, start, endf), (x, endf, endf), (x, start, start), (x, endf, start)]
  fac = [(0, 1, 3, 2)]

  pl_data = bpy.data.meshes.new("TempPlane")
  pl_data.from_pydata(vert, [], fac)
  pl_obj = bpy.data.objects.new("TempPlane", pl_data)
  col = bpy.data.collections.get("Collection")
  col.objects.link(pl_obj)



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

