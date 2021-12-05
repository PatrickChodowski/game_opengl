# Navmesh
Path: ./modules/navmesh.cpp   ./modules/navmesh.h


## Objects

### NavTileData
Data of sinble navmesh tile
```c++
struct NavTileData
```

### NavPolygonData
Data of single navmesh polygon
```c++
struct NavPolygonData
```

### NavGateData
Data of single navgate (connection between [NavPolygonData](navmesh.md#NavPolygonData))
```c++
struct NavGateData
```

### NavNodeData
Data of single navmesh node
```c++
struct NavNodeData
```

### navnodes
Catalog of  [NavNodeData](navmesh.md#NavNodeData)
```c++
std::map<int, NavNodeData> navnodes;
```

### navpolygons
Catalog of [NavPolygonData](navmesh.md#NavPolygonData)
```c++
std::map<int, NavPolygonData> navpolygons;
```

### navtiles
Catalog of [NavTileData](navmesh.md#NavTileData)
```c++
std::map<int, NavTileData> navtiles;
```

### navmesh
Navmesh graph
```c++
std::vector<std::vector<int>> navmesh;
```

### MAX_TILE_ID
For node calculation only
```c++
int MAX_TILE_ID;
```

### MAX_ROW
For node calculation only
```c++
int MAX_COL;
```

### MAX_ROW
For node calculation only
```c++
int MAX_ROW;
```



## Functions
### init
Create [navmesh](navmesh.md#navmesh) for selected map_id
```c++
void init(int map_id);
```

### clear
Remove data from [navmesh](navmesh.md#navmesh), [navtiles](navmesh.md#navtiles), [navpolygons](navmesh.md#navpolygons), [navnodes](navmesh.md#navnodes). Sets [MAX_ROW](navmesh.md#MAX_ROW), [MAX_COL](navmesh.md#MAX_COL) and [MAX_TILE_ID](navmesh.md#MAX_TILE_ID) to 0
```c++
void clear();
```

### render
Debug render squares around nav nodes for debugging mode
```c++
void render();
```

### _load_navtiles
Loads [navtiles](navmesh.md#navtiles) based on the map data
```c++
nav::_load_navtiles(map_id);
```

### _load_navpolygons
Creates [navpolygons](navmesh.md#navpolygons) based on the [navtiles](navmesh.md#navtiles) data.
```c++
nav::_load_navpolygons(map_id);
```

### _get_polygon_id_if_belongs
Check if given tile matches the range of any of already created polygons
```c++
int _get_polygon_id_if_belongs(nav::NavTileData t);
```

### _load_navnodes
Creates [navnodes](navmesh.md#navnodes) out of [navpolygons](navmesh.md#navpolygons)
```c++
void _load_navnodes(nav::NavTileData t);
```

### _join_navnodes
Checks for intersections between [navnodes](navmesh.md#navnodes) and fills out the edges ([NavGateData](navmesh.md#NavGateData) objects)
```c++
void _join_navnodes();
```

### _aabb_intersect
Function testing if 2 AABB boxes intersect
```c++
bool _aabb_intersect(nav::NavNodeData a, nav::NavNodeData b);
```

### _load_navmesh
Creates navmesh graph out of [navnodes](navmesh.md#navnodes)
```c++
void _load_navmesh();
```



## Tests
-
