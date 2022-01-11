# Data structure
# Contains set of rotation and movements of the objects, alongside camera center

from typing import List
from dataclasses import dataclass

@dataclass
class Texture:
  id: int
  name: str
  frame_width: float
  frame_height: float

@dataclass
class Transform:
  id: int
  rot: List[float]
  move: List[float]
  hook: List[float] = None
  tag: str = ""

@dataclass
class Grid:
  id: int
  transforms: List[Transform]
  camera_pos: List[float]
  w: float
  h: float 
  type: str
  hook: List[float]
  max_y: int = 6
  max_z: int = 6

  @staticmethod
  def _transform_point(p: list, rot: list, move: list) -> list:
    """
    Transform collections point (defined on the grind, selected by hand) by all transforms
    This can be hook, hand, head etc. any useful point on the texture
    """
    p2 = list()
    for i in range(0,3):
      #print(p[i] + move[i])
      p2.append(p[i] + move[i])
    return p2

  def generate_hooks(self) -> None:
    for t in self.transforms:
      t.hook = self._transform_point(self.hook, t.rot, t.move)


# define textures:
axe_txt = Texture(8,"axe", 96, 96)


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

# make grid:
weapons_grid = Grid(id=0, 
                    transforms=w_trans, 
                    camera_pos=[8,3.4,3.5], 
                    w=576,
                    h=576,
                    type="weapon",
                    hook=[-0.05, 0.22, -0.2])
weapons_grid.generate_hooks()

