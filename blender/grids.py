# Data structure
# Contains set of rotation and movements of the objects, alongside camera center




from tkinter import W, Y
from typing import List, Optional
from dataclasses import dataclass
import numpy as np
import math
import bpy
from mathutils import Vector

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
  anim_id: int # 
  anim_type_id: int # those types that define what to update
  frame_names: List[str]
  frame_id: List[int]
  direction: List[int]
  r: List[float]
  g: List[float]
  b: List[float]
  a: List[float]
  w: List[float]
  h: List[float]
  x: List[float]
  y: List[float]
  z: List[float]
  update_times: List[float]
  time_length: float 
  cyclical: bool # this kinda has to be defined on the main anim definiton?
  breakable: bool 



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
w_trans.append(Transform(id=0, rot=[-105, 0, 0], move=[0,1,1],    tag="side_face_down_105"))
w_trans.append(Transform(id=1, rot=[-90,0, 0], move=[0,2,1],      tag="side_face_down_90"))
w_trans.append(Transform(id=2, rot=[-45,0, 0], move=[0,3,1],      tag="side_face_level_45"))

w_trans.append(Transform(id=3, rot=[-25,0, 0], move=[0,1,2],      tag="side_face_level_25"))
w_trans.append(Transform(id=4, rot=[0,0, 0], move=[0,2,2],        tag="side_original"))
w_trans.append(Transform(id=5, rot=[30,0, 0], move=[0,3,2],       tag="side_face_up_30"))

w_trans.append(Transform(id=6, rot=[45,0, 0], move=[0,1,3],       tag="side_face_up_45"))
w_trans.append(Transform(id=7, rot=[60,0, 0], move=[0,2,3],       tag="side_face_behind_60"))
w_trans.append(Transform(id=8, rot=[75,0, 0], move=[0,3,3],       tag="side_face_behind_75"))

# facing "up"
w_trans.append(Transform(id=9, rot=[-105, 0, 90], move=[0,1,4],   tag="up_face_down_105" ))
w_trans.append(Transform(id=10, rot=[-90,0, 90], move=[0,2,4],    tag="up_face_down_90"  ))
w_trans.append(Transform(id=11, rot=[-45,0, 90], move=[0,3,4],    tag="up_face_level_45" ))

w_trans.append(Transform(id=12, rot=[-25,0, 90], move=[0,1,5],    tag="up_face_level_25" ))
w_trans.append(Transform(id=13, rot=[0,0, 90], move=[0,2,5],      tag="up_original"      ))
w_trans.append(Transform(id=14, rot=[30,0, 90], move=[0,3,5],     tag="up_face_up_30"    ))

w_trans.append(Transform(id=15, rot=[45,0, 90], move=[0,1,6],     tag="up_face_up_45"    ))
w_trans.append(Transform(id=16, rot=[60,0, 90], move=[0,2,6],     tag="up_face_behind_60"))
w_trans.append(Transform(id=17, rot=[75,0, 90], move=[0,3,6],     tag="up_face_behind_75"))

# facing "down"
w_trans.append(Transform(id=18, rot=[-105, 0, -90], move=[0,4,4], tag="down_face_down_105" ))
w_trans.append(Transform(id=19, rot=[-90,0, -90], move=[0,5,4],   tag="down_face_down_90"  ))
w_trans.append(Transform(id=20, rot=[-45,0, -90], move=[0,6,4],   tag="down_face_level_45" ))

w_trans.append(Transform(id=21, rot=[-25,0, -90], move=[0,4,5],   tag="down_face_level_25" ))
w_trans.append(Transform(id=22, rot=[0,0, -90], move=[0,5,5],     tag="down_original"      ))
w_trans.append(Transform(id=23, rot=[30,0, -90], move=[0,6,5],    tag="down_face_up_30"    ))

w_trans.append(Transform(id=24, rot=[45,0, -90], move=[0,4,6],    tag="down_face_up_45"    ))
w_trans.append(Transform(id=25, rot=[60,0, -90], move=[0,5,6],    tag="down_face_behind_60"))
w_trans.append(Transform(id=26, rot=[75,0, -90], move=[0,6,6],    tag="down_face_behind_75"))



