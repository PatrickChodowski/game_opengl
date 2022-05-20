
#include <vector>
#include <string>
#include "quads.h"
#include "../dependencies/json_struct.h"
#include "../dependencies/parallel_hashmap/phmap.h"

#ifndef MODULES_MAPS2_H
#define MODULES_MAPS2_H

namespace maps2
{
  // Tile data for new maps system
  struct TileData
  {
    int id;
    int x;
    int y;

    int tile_x;
    int tile_y;

    JS_OBJ(id, x, y);
  };

  // Map[2] data struct for new maps system
  struct MapData
  {
    int map_id;
    int model_id;
    int tile_width;
    int tile_height;
    int tile_count_x;
    int tile_count_y;

    std::vector<maps2::TileData> _tiles;

    phmap::flat_hash_map<int, maps2::TileData> tiles;
    phmap::flat_hash_map<int, std::vector<int>> tile_map;

    std::string name;

    JS_OBJ(map_id, model_id, tile_width, tile_height, 
    tile_count_x, tile_count_y, 
    _tiles, name);
  };

  extern phmap::flat_hash_map<int, maps2::MapData> maps;
  extern phmap::flat_hash_map<int, quads::QuadData> tiles;
  extern int CURRENT_TILE_ID;
  extern int CURRENT_MAP_ID;
  extern int TILE_SCALE;

  // Initialize module
  void init();

  // Read map data to struct
  void read_data(std::string name);

  // Finds tile ID for given map and position
  int _get_tile_id(int map_id, int x, int y);

  // Update map tiles for render. Checks in which tile the player is and which tiles to render
  void update(int map_id);

  // Initialize map by the map id
  void init_map(int map_id);

  // Load tiles based on current map id and tile id
  void load_tiles(int map_id, int tile_id);

  // Generate single tile quad
  quads::QuadData generate_tile(int model_id, int tile_id);

  // Clear current tiles
  void clear();

  // Render tiles
  void render();

  // Refresh map data
  void refresh();




}

#endif