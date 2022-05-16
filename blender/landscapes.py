import bpy
from typing import Dict, List

def print_objects():
  for ob in bpy.data.objects:
      print(ob.name)




def _get_landscape_bounding_box(lanscape_ob_name: str) -> List[Dict]:
  """
    Retrieve landscape object bouding box coordinates
  """
  bb = list()
  for ob in bpy.data.objects:
    if ob.name == lanscape_ob_name:
      for bb0 in ob.bound_box:
        bb_dict = dict()
        bb_dict['x'] = bb0[0]
        bb_dict['y'] = bb0[1]
        bb_dict['z'] = bb0[2]
        bb.append(bb_dict)
  return bb


def clear_cameras() -> None:
  """
    Removes all cameras from the scene
  """
  if bpy.context.object.mode != 'OBJECT':
    bpy.ops.object.mode_set(mode='OBJECT')
  bpy.ops.object.select_all(action='DESELECT')
  for ob in bpy.data.objects:
    if "Camera" in ob.name:
      ob.select_set(True)
  bpy.ops.object.delete()
  bpy.ops.object.select_all(action='DESELECT')


def clear_empties(obj_name: str) -> None:
  """
    Remove all empties linked to the object
  """



def add_camera() -> None:
  """
    Adds new camera to the scene
  """
  camera_data = bpy.data.cameras.new(name='Camera')
  camera_object = bpy.data.objects.new('Camera', camera_data)
  bpy.context.scene.collection.objects.link(camera_object)


def generate_empties(lanscape_ob_name: str) -> List[Dict]:
  """
    Create empties based on the bounding box vertices
  """
  bb = _get_landscape_bounding_box(lanscape_ob_name)

  for ob in bpy.data.objects:
    if ob.name == lanscape_ob_name:
      obj = bpy.data.objects[lanscape_ob_name]

      if (obj is not None) & (bb.__len__() > 0):
        for index, bb0 in enumerate(bb):
          empty = bpy.data.objects.new( f"Empty_{index}", None)  # Create new empty object
          obj.users_collection[0].objects.link(empty)  # Link empty to the current object's collection
          empty.empty_display_type = 'PLAIN_AXES'
          empty.parent = obj
          empty.location = (bb0['x'], bb0['y'], bb0['z'])
