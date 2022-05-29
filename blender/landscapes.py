import bpy
from typing import Dict, List
import importlib.util

spec_utils = importlib.util.spec_from_file_location("utils", 
"/home/patrick/Documents/projects/game_opengl/blender/utils.py")
utils = importlib.util.module_from_spec(spec_utils)
spec_utils.loader.exec_module(utils)


# https://www.conciergerender.com/details?id=571777&filename=first_respectable_terrain.blend#

# one tile would be 1 blender meter and 1080 pixels?
RENDER = 'concierge_big'
RENDER_SETUPS = {'standard': {'tile_width': 10, 'tile_height': 10, 'camera_w': 1080, 'camera_h': 1080, 'camera_z': 14, 'focal_length': 50, 'lens_type': "PERSP", 'angle':False},
                 'detail': {'tile_width': 5, 'tile_height': 5, 'camera_w': 1080, 'camera_h': 1080, 'camera_z': 2, 'focal_length': 50, 'lens_type': "PERSP", 'angle':False},
                 'huge': {'tile_width': 20, 'tile_height': 20, 'camera_w': 4096, 'camera_h': 4096, 'camera_z': 2, 'focal_length': 50, 'lens_type': "PERSP", 'angle':True},
                 'small': {'tile_width': 1, 'tile_height': 1, 'camera_w': 256, 'camera_h': 256, 'camera_z': 2, 'focal_length': 50, 'lens_type': "ORTHO", 'angle':True},
                 'concierge': {'tile_width': 20, 'tile_height': 20, 'camera_w': 4096, 'camera_h': 3072, 'camera_z': 2, 'focal_length': 50, 'lens_type': "ORTHO", 'angle':True},
                 'concierge_big': {'tile_width': 20, 'tile_height': 20, 'camera_w': 4096*2, 'camera_h': 3072*2, 'camera_z': 2, 'focal_length': 50, 'lens_type': "ORTHO", 'angle':True},
                 }


def _setup_render() -> None:
  bpy.context.scene.render.engine = 'CYCLES'
  bpy.context.scene.cycles.feature_set = 'EXPERIMENTAL'
  bpy.context.scene.cycles.device = 'GPU'
  bpy.context.scene.cycles.adaptive_threshold = 0.01
  bpy.context.scene.cycles.samples = 100
  bpy.context.scene.world.cycles_visibility.glossy = False
  bpy.context.scene.cycles.use_preview_denoising = True
  bpy.context.scene.cycles.max_bounces = 2
  bpy.context.scene.cycles.diffuse_bounces = 2
  bpy.context.scene.cycles.glossy_bounces = 2
  bpy.context.scene.cycles.volume_bounces = 2
  bpy.context.scene.cycles.transmission_bounces = 2
  bpy.context.scene.cycles.transparent_max_bounces = 2
  bpy.context.scene.render.threads_mode = 'AUTO'
  bpy.context.scene.cycles.use_auto_tile = True
  bpy.context.scene.cycles.tile_size = 1024
  #bpy.context.scene.render.engine = 'BLENDER_EEVEE'


def _setup_camera(camera_name: str) -> None:
   bpy.data.cameras[camera_name].ortho_scale = RENDER_SETUPS[RENDER]['tile_height']*1.0
   bpy.data.cameras[camera_name].lens = 50.0
   bpy.data.cameras[camera_name].type = RENDER_SETUPS[RENDER]['lens_type']


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

def _get_positions(mmd: Dict) -> List[Dict]:
  ld_width = mmd['max_x'] - mmd['min_x']
  ld_height = mmd['max_y'] - mmd['min_y']

  tile_width = RENDER_SETUPS[RENDER]['tile_width']
  tile_height = RENDER_SETUPS[RENDER]['tile_height']

  pos_width = int(ld_width/tile_width)
  pos_height = int(ld_height/tile_height)
  poss = list()
  pos_id = 0
  # start in left top corner Need to put empty/plane object in the middle of tile
  for h in range(pos_height):
    for w in range(pos_width):
      d = dict()
      d['id'] = pos_id
      d['x'] = mmd['min_x'] + (tile_width/2) + tile_width*w
      d['y'] = mmd['max_y'] - (tile_height/2) - tile_height*h
      pos_id += 1
      poss.append(d)

  return poss






def generate_empties(lanscape_ob_name: str) -> List[Dict]:
  """
    Create empties based on the bounding box vertices
  """
  bb = _get_landscape_bounding_box(lanscape_ob_name)
  mmd = _get_min_max_bb(bb)
  empties = _get_positions(mmd)

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
          empty.location = (empty_d['x'], empty_d['y'], 0)
          empty_d['name'] = empty_name

  return empties



def generate_locations(lanscape_ob_name: str) -> List[Dict]:
  """
    Create location list for planes 
  """
  bb = _get_landscape_bounding_box(lanscape_ob_name)
  mmd = _get_min_max_bb(bb)
  positions = _get_positions(mmd)
  return positions


def run(map_name: str = 'lake_map') -> None:
  """
    Runs this function in the blender script
  """
  _setup_render()
  camera_name = 'Camera'
 
  utils.clear_cameras()
  utils.clear_empties()
  utils.clear_planes()

  locations = generate_locations('Landscape')
  utils.create_camera(camera_name, RENDER_SETUPS[RENDER]['camera_w'], RENDER_SETUPS[RENDER]['camera_h'])
  _setup_camera(camera_name)

  #for loc in locations[0:6]:
  for loc in locations:
    print(f" Rendering {map_name} render: {RENDER} part: {loc['id']}")
    plane_name = f"TRACK_PLANE_{loc['id']}"
    utils.create_plane(plane_name, x=0, y=0, z=0, size=RENDER_SETUPS[RENDER]['tile_width']) # it will catch a bit more than just plane
    utils.set_object_location(plane_name, loc['x'], loc['y'], 0.406) # Landscape Z
    utils.set_object_location(camera_name, x = loc['x'], y=loc['y'], z=RENDER_SETUPS[RENDER]['camera_z'])
    camera_loc = utils.set_camera_at(plane_name)

    if RENDER_SETUPS[RENDER]['angle']:
      new_camera_y = loc['y'] - camera_loc['z']
      utils.set_object_location(camera_name, x = camera_loc['x'], y=new_camera_y, z=camera_loc['z'])
      bpy.data.objects['Camera'].rotation_euler.x = 0.785398
      utils.set_camera_at(plane_name)
      utils.set_object_location(plane_name, x = -100, y=-100, z=RENDER_SETUPS[RENDER]['camera_z'])
    # utils.render_png(f"/home/patrick/Documents/projects/game_opengl/blender/done/{map_name}_{RENDER}/{loc['id']}.png")





# https://www.reddit.com/r/blender/comments/3aie50/the_blender_guide_to_focal_lengths/
