#include <vector>
#include <string>
#include "quads.h"
#include "../dependencies/json_struct.h"
#include "../dependencies/parallel_hashmap/phmap.h"

#ifndef MODULES_MAPS_H
#define MODULES_MAPS_H

// Persistent data. Table of tiles and all operations on this table. Loaded on new level
namespace maps
{
  // Nest for spawning mobs
  struct Nest
  {
    float x;
    float y;
    int n;

    JS_OBJ(x, y, n);
  };

  // Door information
  struct Door
  {
    int door_id;
    int dest_scene_id;
    float x;
    float y;
    float player_enter_x;
    float player_enter_y;

    JS_OBJ(door_id, dest_scene_id, x, y, player_enter_x, player_enter_y);
  };

  struct MapData
  {
    int id;
    int vertex_width;
    int vertex_height;
    int model_id;
    int default_player_x;
    int default_player_y;

    std::vector<maps::Door> _doors;
    phmap::flat_hash_map<int, maps::Door> doors;
    std::vector<maps::Nest> nests;

    std::string name;

    JS_OBJ(id, name, vertex_width, vertex_height, model_id, default_player_x, default_player_y, _doors, nests);
  };

  extern float default_tile_width;
  extern float default_tile_height;

  // MapID, MapData
  extern phmap::flat_hash_map<int, MapData> maps;

  //  TileID, TileData
  extern phmap::flat_hash_map<int, quads::QuadData> tiles;

  // Generate single tile out of position, texture_id and frame_id
  quads::QuadData generate_tile(float x, float y, int model_id, int frame_id);

  // Reads json data by map name and stores it inside maps::maps
  void read_map_data(std::string name);

  // Reads list of maps from maps/data/ and for each map there performs 
  // read_map_data function. Thats reads json into maps::maps
  void init();

  // Loads map tiles from map file by map id
  void load(int map_id);

  // Loads selected map to game
  void init_map(int map_id);

  // Make map quads from maps::tiles
  void render();

  // clears the currently loaded map -> erases used tiles ids and tiles catalog
  void clear();

  // Clears all data
  void refresh();

}


#endif