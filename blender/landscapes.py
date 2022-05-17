import bpy
from typing import Dict, List

# one tile would be 1 blender meter and 1080 pixels?
TILE_WIDTH = 10
TILE_HEIGHT = 10

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
  # print(" Bounding Box: ")
  # print(bb)
  return bb


def _get_min_max_bb(bb: List[Dict]) -> Dict:
  """
    Finds minimum and maximum coordinates of each (x,y,z) dimension
  """
  x_s = list()
  y_s = list()
  z_s = list()
  for bb0 in bb:
    x_s.append(bb0['x'])
    y_s.append(bb0['y'])
    z_s.append(bb0['z'])
  d = dict()
  d['min_x'] = min(x_s)
  d['max_x'] = max(x_s)
  d['min_y'] = min(y_s)
  d['max_y'] = max(y_s)
  d['min_z'] = min(z_s)
  d['max_z'] = max(z_s)
  # print(" Min Max BB: ")
  # print(d)
  return d


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



def add_camera() -> None:
  """
    Adds new camera to the scene
  """
  camera_data = bpy.data.cameras.new(name='Camera')
  camera_object = bpy.data.objects.new('Camera', camera_data)
  bpy.context.scene.collection.objects.link(camera_object)


def _get_empty_positions(mmd: Dict) -> List[Dict]:
  ld_width = mmd['max_x'] - mmd['min_x']
  ld_height = mmd['max_y'] - mmd['min_y']
  empty_width = int(ld_width/TILE_WIDTH)
  empty_height = int(ld_height/TILE_HEIGHT)
  empties = list()
  e_id = 0
  # start in left top corner Need to put empty object in the middle of tile
  for h in range(empty_height):
    for w in range(empty_width):
      d = dict()
      d['id'] = e_id
      d['x'] = mmd['min_x'] + (TILE_WIDTH/2) + TILE_WIDTH*w
      d['y'] = mmd['max_y'] - (TILE_HEIGHT/2) - TILE_HEIGHT*h
      e_id += 1
      empties.append(d)
  # print(" Empties positons: ")
  # print(empties)

  return empties


def generate_empties(lanscape_ob_name: str) -> List[Dict]:
  """
    Create empties based on the bounding box vertices
  """
  bb = _get_landscape_bounding_box(lanscape_ob_name)
  mmd = _get_min_max_bb(bb)
  empties = _get_empty_positions(mmd)

  for ob in bpy.data.objects:
    if ob.name == lanscape_ob_name:
      obj = bpy.data.objects[lanscape_ob_name]

      if (obj is not None) & (empties.__len__() > 0):
        for empty_d in empties:
          empty = bpy.data.objects.new( f"Empty_{empty_d['id']}", None)  # Create new empty object
          obj.users_collection[0].objects.link(empty)  # Link empty to the current object's collection
          empty.empty_display_type = 'PLAIN_AXES'
          empty.parent = obj
          #empty.location = (bb0['x'], bb0['y'], bb0['z'])
          empty.location = (empty_d['x'], empty_d['y'], 0)
