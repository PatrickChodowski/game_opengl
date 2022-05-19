
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

    std::vector<maps2::TileData> tiles;
    phmap::flat_hash_map<int, std::vector<int>> tile_map;

    std::string name;

    JS_OBJ(map_id, model_id, tile_width, tile_height, tile_count_x, tile_count_y, 
    tiles, name);
  };

  extern phmap::flat_hash_map<int, maps2::MapData> maps;
  extern phmap::flat_hash_map<int, quads::QuadData> tiles;

  // Initialize module
  void init();

  // Read map data to struct
  void read_data(std::string name);

  // Initialize map by the map id
  void init_map(int map_id);

  // Clear current tiles
  void clear();





}

#endif