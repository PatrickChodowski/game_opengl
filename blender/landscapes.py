import bpy
from typing import Dict, List
import importlib.util

spec_utils = importlib.util.spec_from_file_location("utils", 
"/home/patrick/Documents/projects/game_opengl/blender/utils.py")
utils = importlib.util.module_from_spec(spec_utils)
spec_utils.loader.exec_module(utils)

# one tile would be 1 blender meter and 1080 pixels?
RENDER = 'detail'
RENDER_SETUPS = {'standard': {'tile_width': 10, 'tile_height': 10, 'camera_w': 1080, 'camera_h': 1080, 'camera_z': 14, 'focal_length': 50, 'lens_type': "PERSP"}}
RENDER_SETUPS = {'detail': {'tile_width': 5, 'tile_height': 5, 'camera_w': 1080, 'camera_h': 1080, 'camera_z': 7.5, 'focal_length': 50, 'lens_type': "PERSP"}}


def _setup_render() -> None:
  bpy.context.scene.render.engine = 'CYCLES'
  bpy.context.scene.cycles.feature_set = 'EXPERIMENTAL'
  bpy.context.scene.cycles.device = 'GPU'
  bpy.context.scene.cycles.use_denoising = True
  bpy.context.scene.cycles.adaptive_threshold = 0.01
  bpy.context.scene.cycles.samples = 500
  bpy.context.scene.world.cycles_visibility.glossy = False


  # camera ZLE:
  # bpy.context.object.data.lens = RENDER_SETUPS[RENDER]['focal_length']
  # bpy.context.object.data.type = RENDER_SETUPS[RENDER]['lens_type']




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

def _get_empty_positions(mmd: Dict) -> List[Dict]:
  ld_width = mmd['max_x'] - mmd['min_x']
  ld_height = mmd['max_y'] - mmd['min_y']

  tile_width = RENDER_SETUPS[RENDER]['tile_width']
  tile_height = RENDER_SETUPS[RENDER]['tile_height']

  empty_width = int(ld_width/tile_width)
  empty_height = int(ld_height/tile_height)
  empties = list()
  e_id = 0
  # start in left top corner Need to put empty object in the middle of tile
  for h in range(empty_height):
    for w in range(empty_width):
      d = dict()
      d['id'] = e_id
      d['x'] = mmd['min_x'] + (tile_width/2) + tile_width*w
      d['y'] = mmd['max_y'] - (tile_height/2) - tile_height*h
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
          empty_name = f"Empty_{empty_d['id']}"
          empty = bpy.data.objects.new(empty_name, None)  # Create new empty object
          obj.users_collection[0].objects.link(empty)  # Link empty to the current object's collection
          empty.empty_display_type = 'PLAIN_AXES'
          empty.empty_display_size = (RENDER_SETUPS[RENDER]['tile_width']/2)
          empty.parent = obj
          #empty.location = (bb0['x'], bb0['y'], bb0['z'])
          empty.location = (empty_d['x'], empty_d['y'], 0)
          empty_d['name'] = empty_name

  return empties



def run(map_name: str = 'lake_map'):
  """
    Runs this function in the blender script
  """
  _setup_render()
  camera_name = 'Camera'
  utils.clear_cameras()
  utils.clear_empties()
  empties = generate_empties('Landscape')
  utils.create_camera(camera_name, RENDER_SETUPS[RENDER]['camera_w'], RENDER_SETUPS[RENDER]['camera_h'])
  # for empty_d in empties:
  #   print(f" Rendering {map_name} render: {RENDER} part: {empty_d['id']}")
  #   utils.set_camera_location(camera_name, x = empty_d['x'], y=empty_d['y'], z=RENDER_SETUPS[RENDER]['camera_z'])
  #   bpy.context.scene.render.filepath = f"/home/patrick/Documents/projects/game_opengl/blender/done/{map_name}_{RENDER}/{empty_d['id']}.png"
  #   bpy.ops.render.render(write_still = 1)




# https://www.reddit.com/r/blender/comments/3aie50/the_blender_guide_to_focal_lengths/