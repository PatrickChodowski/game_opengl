# Pathfinder
Path: ./modules/pathfinder.cpp   ./modules/pathfinder.h


## Objects

### DjikstraStep
Struct used in Djikstra algorithm
```c++
struct DjikstraStep
```

### PathMap
Precalculated paths between [navnodes](navmesh.md#navnodes)
```c++
std::map<int, std::map<int, DjikstraStep>> PathMap;
```

## Functions
### get_navnode_id
Get [NavNode ID](navmesh.md#navnodes) based on the X,Y position
```c++
int get_navnode_id(float x, float y);
```

### get_min_distance
Get shortest distance between polygons
```c++
int get_min_distance(float x, float y);
```

### find_paths_djikstra
Find paths from [NavNode ID](navmesh.md#navnodes) to all other [navnodes](navmesh.md#navnodes) using djikstra algorithm. Results is saved to [PathMap](pathfinder.md#PathMap)
```c++
std::map<int, DjikstraStep> find_paths_djikstra(int node_id);
```

### make_path_map
Fills out the [PathMap](pathfinder.md#PathMap) based on the [navnodes](navmesh.md#navmesh) graph
```c++
void make_path_map();
```

### find_path
Find path between start node and end node using [PathMap](pathfinder.md#PathMap)
```c++
std::vector<int> find_path(int start_node_id, int end_node_id);
```

## Tests
- if really finds the paths
