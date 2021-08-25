import numpy as np
from typing import Optional, Dict
from itertools import count

file_name = 'map_solid_array3.txt'

arr = np.genfromtxt(file_name, delimiter=' ')
MAX_ROW = (arr.shape[0] - 1)
MAX_COL = (arr.shape[1] - 1)


class Polygon:
    _ids = count(1)

    def __init__(self, min_x: int, min_y: int) -> None:
        self.id = next(self._ids)
        self.tiles = list()
        self.min_x = min_x
        self.min_y = min_y

        self.max_x = MAX_COL
        self.max_y = MAX_ROW


class Tile:
    def __init__(self, x: int, y: int, checked: bool, solid: bool) -> None:
        self.x = x
        self.y = y
        self.checked = checked
        self.solid = solid
        self.id = str(self.x) + "_" + str(self.y)

        self.polygon_id: int
        self.tile_id_right: Optional[str]
        self.tile_id_down: Optional[str]
        self.tile_id_rightdown: Optional[str]
        self.check_tiles_around()

    def check_tiles_around(self) -> None:
        if (self.x < MAX_COL) & (self.y < MAX_ROW):
            self.tile_id_right = f"{self.x + 1}_{self.y}"
            self.tile_id_rightdown = f"{self.x + 1}_{self.y + 1}"
            self.tile_id_down = f"{self.x}_{self.y + 1}"
        elif (self.x < MAX_COL) & (self.y == MAX_ROW):
            self.tile_id_right = f"{self.x + 1}_{self.y}"
            self.tile_id_rightdown = None
            self.tile_id_down = None
        elif (self.x == MAX_COL) & (self.y < MAX_ROW):
            self.tile_id_right = None
            self.tile_id_rightdown = None
            self.tile_id_down = f"{self.x}_{self.y + 1}"
        elif (self.x == MAX_COL) & (self.y == MAX_ROW):
            self.tile_id_right = None
            self.tile_id_rightdown = None
            self.tile_id_down = None

    def get_polygon_id_if_belongs(self, lp: Dict[int, Polygon]) -> Optional[int]:
        polygon_id = 0
        for _poly_id, poly in lp.items():
            if (self.x >= poly.min_x) & (self.x <= poly.max_x) & (self.y >= poly.min_y) & (self.y <= poly.max_y):
                polygon_id = _poly_id
                break
        return polygon_id

TILES = dict()
POLYGONS = dict()
IN_POLYGON = False

# make list of tile objects based on array:
for index_row, row in enumerate(arr):
    for index_col, col in enumerate(row):
        t = Tile(index_col, index_row, False, col)
        TILES[t.id] = t

for tile_id, t in TILES.items():
    if not t.checked:
        if t.solid:
            if tile_id == f'{MAX_COL}_{MAX_ROW}':
                POLYGONS[cp.id] = cp
            continue
        else:
            if not IN_POLYGON:
                # start first polygon
                cp = Polygon(min_x=t.x, min_y=t.y)
                IN_POLYGON = True
                cp.tiles.append(t)
            else:
                poly_id = t.get_polygon_id_if_belongs(lp=POLYGONS)
                print(f"Assigning {tile_id}")

                # if fits to current polygon
                if poly_id > 0:
                    METHOD = 1
                    print(f" [1] Tile {tile_id} belongs to  {poly_id}")
                    POLYGONS[poly_id].tiles.append(t)
                    if t.tile_id_right is not None:
                        if TILES[t.tile_id_right].solid:
                            POLYGONS[poly_id].max_x = t.x

                    if t.tile_id_down is not None:
                        if TILES[t.tile_id_down].solid:
                            POLYGONS[poly_id].max_y = t.y


                else:
                    # if fits to any of already started polygons
                    if (t.x >= cp.min_x) & (t.x <= cp.max_x) & (t.y >= cp.min_y) & (t.y <= cp.max_y):
                        print(f" [2] Tile {tile_id} belongs to  {cp.id}")
                        cp.tiles.append(t)
                    else:
                        POLYGONS[cp.id] = cp
                        cp = Polygon(min_x=t.x, min_y=t.y)
                        print(f" [3] Tile {tile_id} belongs to {cp.id}")
                        cp.tiles.append(t)

                        # here, check if next tile belongs to previously created (single column rule)
                    if t.tile_id_right is not None:
                        nex_tile_polygon_id = 0
                        for _poly_id, poly in POLYGONS.items():
                            if (TILES[t.tile_id_right].x >= poly.min_x) & \
                                    (TILES[t.tile_id_right].x <= poly.max_x) & \
                                    (TILES[t.tile_id_right].y >= poly.min_y) & \
                                    (TILES[t.tile_id_right].y <= poly.max_y):
                                nex_tile_polygon_id = _poly_id
                                break
                        if nex_tile_polygon_id > 0:
                            cp.max_x = t.x

                    # check neighbouring tiles:
                    if t.tile_id_right is not None:
                        if TILES[t.tile_id_right].solid:
                            cp.max_x = t.x
                    else:
                        cp.max_x = t.x

                    if t.tile_id_down is not None:
                        if TILES[t.tile_id_down].solid:
                            cp.max_y = t.y
                    else:
                        cp.max_y = t.y

    if tile_id == f'{MAX_COL}_{MAX_ROW}':
        POLYGONS[cp.id] = cp

p_array = np.zeros((MAX_ROW + 1, MAX_COL + 1), dtype=int, order='C')
for pid, p in POLYGONS.items():
    for t in p.tiles:
        p_array[t.y][t.x] = p.id

print(p_array)
