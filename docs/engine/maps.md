# Maps
Path: ./modules/maps.cpp   ./modules/maps.h


## Objects

### TileData
Map Tile data
```c++
struct TileData
```

### Nest
Map nest -> place to spawn mobs
```c++
struct Nest
```

### Door
Transition to another scene/level
```c++
struct Door
```

### MapNPCData
Maps NPC information (id, position)
```c++
struct MapNPCData
```

### MapData
Data about the whole map -> doors, nests, player starting point, dimensions etc.
```c++
struct MapData
```

### default_tile_width
Default with of the map tile
```c++
float default_tile_width
```

### default_tile_height
Default height of the map tile
```c++
float default_tile_height
```

### maps
Maps catalog -> map_id, [MapData](maps.md#MapData)
```c++
std::map<int, MapData> maps;
```

### tiles
Tiles catalog -> tile_id, [TileData](maps.md#tiles)
```c++
std::map<int, maps::TileData> tiles;
```

### MapQuads
Quads made out of the [tiles](maps.md#tiles). Vector of [quads](quads.md#QuadData)
```c++
std::vector<quads::QuadData> MapQuads;
```


## Functions
### init
Reads all [MapData](maps.md#MapData) to [maps](maps.md#maps)
```c++
void init();
```

### read_map_data
Reads and inserts single [MapData](maps.md#MapData) to [maps](maps.md#maps) by file name
```c++
void read_map_data(std::string name);
```

### generate_tile
Generate single tile out of position, texture_id and frame_id
```c++
maps::TileData generate_tile(float x, float y, int texture_id, int frame_id);
```

### load
Load tiles to [tiles](maps.md#tiles) based on map_id and [MapData](maps.md#MapData) from [maps](maps.md#maps)
```c++
void load(int map_id);
```

### init_map
Loads map using [load](maps.md#load), generate [navmesh](navmesh.md#navmesh) for the map and generate [pathmap](pathfinder.md#pathmap).
```c++
void init_map(int map_id);
```

### render
Populates [MapQuads](maps.md#MapQuads) from [tiles](maps.md#tiles)
```c++
void render();
```

### refresh
Resets the data read in from files in the game
```c++
void refresh();
```

### clear
Clears [MapQuads](maps.md#MapQuads), [Index](maps.md#Index), [tiles](maps.md#tiles)
```c++
void clear();
```


## Tests
-
