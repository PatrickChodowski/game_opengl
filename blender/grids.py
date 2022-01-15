# Data structure
# Contains set of rotation and movements of the objects, alongside camera center

from typing import List, Optional
from dataclasses import dataclass
import numpy as np
import math
import bpy
from mathutils import Vector

frame_map = {
      "side_face_down_105":   {"id": 0, "rot":[-105, 0, 0],  "move":[0,1,1]},
      "side_face_down_90":    {"id": 1, "rot":[-90,0, 0],    "move":[0,2,1]}, 
      "side_face_level_45":   {"id": 2, "rot":[-45,0, 0],    "move":[0,3,1]},
      "side_face_level_25":   {"id": 3, "rot":[-25,0, 0],    "move":[0,1,2]},
      "side_original":        {"id": 4, "rot":[0,0, 0],      "move":[0,2,2]},
      "side_face_up_30":      {"id": 5, "rot":[30,0, 0],     "move":[0,3,2]},
      "side_face_up_45":      {"id": 6, "rot":[45,0, 0],     "move":[0,1,3]},
      "side_face_behind_60":  {"id": 7, "rot":[60,0, 0],     "move":[0,2,3]},
      "side_face_behind_75":  {"id": 8, "rot":[75,0, 0],     "move":[0,3,3]},
      "up_face_down_105":     {"id": 9, "rot":[-105, 0, 90], "move":[0,1,4]},
      "up_face_down_90":      {"id": 10,"rot":[-90,0, 90],   "move":[0,2,4]},
      "up_face_level_45":     {"id": 11,"rot":[-45,0, 90],   "move":[0,3,4]},
      "up_face_level_25":     {"id": 12,"rot":[-25,0, 90],   "move":[0,1,5]},
      "up_original":          {"id": 13,"rot":[0,0, 90],     "move":[0,2,5]},
      "up_face_up_30":        {"id": 14,"rot":[30,0, 90],    "move":[0,3,5]},
      "up_face_up_45":        {"id": 15,"rot":[45,0, 90],    "move":[0,1,6]},
      "up_face_behind_60":    {"id": 16,"rot":[60,0, 90],    "move":[0,2,6]},
      "up_face_behind_75":    {"id": 17,"rot":[75,0, 90],    "move":[0,3,6]},
      "down_face_down_105":   {"id": 18,"rot":[-105, 0, -90],"move":[0,4,4]},
      "down_face_down_90":    {"id": 19,"rot":[-90,0, -90],  "move":[0,5,4]},
      "down_face_level_45":   {"id": 20,"rot":[-45,0, -90],  "move":[0,6,4]},
      "down_face_level_25":   {"id": 21,"rot":[-25,0, -90],  "move":[0,4,5]},
      "down_original":        {"id": 22,"rot":[0,0, -90],    "move":[0,5,5]},
      "down_face_up_30":      {"id": 23,"rot":[30,0, -90],   "move":[0,6,5]},
      "down_face_up_45":      {"id": 24,"rot":[45,0, -90],   "move":[0,4,6]},
      "down_face_behind_60":  {"id": 25,"rot":[60,0, -90],   "move":[0,5,6]},
      "down_face_behind_75":  {"id": 26,"rot":[75,0, -90],   "move":[0,6,6]}
    }


@dataclass
class Texture:
  id: int
  name: str
  frame_width: float
  frame_height: float

@dataclass
class Hook:
  object_name: Optional[str] 
  global_pos: List[float]
  vertex_id: Optional[int] = None

  def get_id(self) -> None:
    o = bpy.data.objects[self.object_name]
    mat = o.matrix_world
    for v in o.data.vertices:
      global_pos = mat @ v.co
      if (round(global_pos[0],2) == self.global_pos[0]) & \
         (round(global_pos[1],2) == self.global_pos[1]) & \
         (round(global_pos[2],2) == self.global_pos[2]):
        self.vertex_id = v.index
        print(f"found vertex id: {self.vertex_id}")

@dataclass
class Transform:
  id: int
  rot: List[float]
  move: List[float]
  tag: str
  coll_id: Optional[str] = None

@dataclass
class Animation:
  anim_id: int
  anim_type_id: int
  update_times: List[float]
  time_length: float 
  cyclical: bool
  breakable: bool 

  frame_id: Optional[List[int]] = None
  direction: Optional[List[int]] = None
  r: Optional[List[float]] = None
  g: Optional[List[float]] = None
  b: Optional[List[float]] = None
  a: Optional[List[float]] = None
  w: Optional[List[float]] = None
  h: Optional[List[float]] = None
  x: Optional[List[float]] = None
  y: Optional[List[float]] = None
  z: Optional[List[float]] = None



@dataclass
class Grid:
  id: int
  transforms: List[Transform]
  camera_pos: List[float]
  w: float
  h: float 
  type: str
  hook: Hook
  anims: List[Animation]
  max_y: int = 6
  max_z: int = 6

# define textures:
axe_txt = Texture(8,"axe", 100, 100)


# make transforms:
w_trans = list()
for k, v in frame_map.items():
  w_trans.append(Transform(id=v["id"], rot=v["rot"], move=v["move"],tag=k))



# Animations list
# TYPE_FRAME 0 COLOR 1 POSITION 2 SIZE 3 DIRECTION 4 TEXTURE 5 FRAME_DIRECTION 6 FRAME_POSITION 7
# DIRECTION LEFT 0 RIGHT 1
# ANIM IDLE 0 MOVE_LEFT 1 MOVE_RIGHT 2 MOVE_UP 3 MOVE_DOWN 4

w_anims = list()
w_anims.append(Animation(anim_id=0, # idle
                         anim_type_id=0, #frame
                         update_times=[0.16],
                         time_length=0.17,
                         cyclical=True,
                         breakable=True,
                         frame_id=[frame_map["side_original"]["id"]]))

w_anims.append(Animation(anim_id=1, # move_left
                         anim_type_id=7, # frame_position
                         update_times=[0.17, 0.34, 0.51, 0.68, 0.85, 1.02, 1.03],
                         time_length=1.02,
                         cyclical=False,
                         breakable=True,
                         frame_id=[frame_map["side_face_down_105"]["id"], 
                                   frame_map["side_face_down_90"]["id"], 
                                   frame_map["side_face_level_45"]["id"], 
                                   frame_map["side_face_level_25"]["id"], 
                                   frame_map["side_face_level_45"]["id"], 
                                   frame_map["side_face_down_90"]["id"]],
                         z = [0.2, 0.2, 0.2, 0.2, 0.2, 0.2]))

w_anims.append(Animation(anim_id=2, # move_right
                         anim_type_id=6, # frame_direction
                         update_times=[0.17, 0.34, 0.51, 0.68, 0.85, 1.02, 1.03],
                         time_length=1.02,
                         cyclical=False,
                         breakable=True,
                         frame_id=[frame_map["side_face_down_105"]["id"], 
                                   frame_map["side_face_down_90"]["id"], 
                                   frame_map["side_face_level_45"]["id"], 
                                   frame_map["side_face_level_25"]["id"], 
                                   frame_map["side_face_level_45"]["id"], 
                                   frame_map["side_face_down_90"]["id"]],
                         direction=[1, 1, 1, 1, 1, 1]
                         ))

