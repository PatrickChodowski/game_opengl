# Data structure
# Contains set of rotation and movements of the objects, alongside camera center

from typing import List
from dataclasses import dataclass

@dataclass
class Transform:
  id: int
  rot: List[float]
  move: List[float]
  tag: str = ""

@dataclass
class Grid:
  id: int
  transforms: List[Transform]
  camera_pos: List[float]


# make transforms:
w_trans = list()
w_trans.append(Transform(id=0, rot=[-105, 0, 0], move=[0,1,1]))
w_trans.append(Transform(id=1, rot=[-90,0, 0], move=[0,2,1]))
w_trans.append(Transform(id=2, rot=[-45,0, 0], move=[0,3,1]))

w_trans.append(Transform(id=3, rot=[-25,0, 0], move=[0,1,2]))
w_trans.append(Transform(id=4, rot=[0,0, 0], move=[0,2,2]))
w_trans.append(Transform(id=5, rot=[30,0, 0], move=[0,3,2]))

w_trans.append(Transform(id=6, rot=[45,0, 0], move=[0,1,3]))
w_trans.append(Transform(id=7, rot=[60,0, 0], move=[0,2,3]))
w_trans.append(Transform(id=8, rot=[75,0, 0], move=[0,3,3]))

# facing "up"
w_trans.append(Transform(id=0, rot=[-105, 0, 90], move=[0,1,4]))
w_trans.append(Transform(id=1, rot=[-90,0, 90], move=[0,2,4]))
w_trans.append(Transform(id=2, rot=[-45,0, 90], move=[0,3,4]))

w_trans.append(Transform(id=3, rot=[-25,0, 90], move=[0,1,5]))
w_trans.append(Transform(id=4, rot=[0,0, 90], move=[0,2,5]))
w_trans.append(Transform(id=5, rot=[30,0, 90], move=[0,3,5]))

w_trans.append(Transform(id=6, rot=[45,0, 90], move=[0,1,6]))
w_trans.append(Transform(id=7, rot=[60,0, 90], move=[0,2,6]))
w_trans.append(Transform(id=8, rot=[75,0, 90], move=[0,3,6]))

# facing "down"
w_trans.append(Transform(id=0, rot=[-105, 0, -90], move=[0,4,4]))
w_trans.append(Transform(id=1, rot=[-90,0, -90], move=[0,5,4]))
w_trans.append(Transform(id=2, rot=[-45,0, -90], move=[0,6,4]))

w_trans.append(Transform(id=3, rot=[-25,0, -90], move=[0,4,5]))
w_trans.append(Transform(id=4, rot=[0,0, -90], move=[0,5,5]))
w_trans.append(Transform(id=5, rot=[30,0, -90], move=[0,6,5]))

w_trans.append(Transform(id=6, rot=[45,0, -90], move=[0,4,6]))
w_trans.append(Transform(id=7, rot=[60,0, -90], move=[0,5,6]))
w_trans.append(Transform(id=8, rot=[75,0, -90], move=[0,6,6]))


# make grid:
weapons_grid = Grid(id=0, transforms=w_trans, camera_pos=[8,3.4,3.5])

