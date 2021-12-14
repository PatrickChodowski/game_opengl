#include <vector>
#include <map>
#include <string>
#include "quads.h"
#include "collisions.h"
#include "../dependencies/json_struct.h"

#ifndef MODULES_MAPS_H
#define MODULES_MAPS_H

// Persistent data. Table of tiles and all operations on this table. Loaded on new level
namespace maps
{
  struct TileData
  {
    int id;
    int texture_id;
    int frame_id;
    int camera_type;

    float mid_x;
    float mid_y;
    float diag;

    quads::Position pos;
    quads::Color color;
    quads::Dims dims;
    quads::Norm norm;
    // collision boxes
    std::map<int, collisions::AABB> abs;

    bool is_clicked;
    bool is_solid;

  };

  // Nest for spawning mobs
  struct Nest
  {
    float x;
    float y;
    int n;

    JS_OBJ(x, y, n);
  };

  // Npc assigned to map information
  struct MapNPCData
  {
    int id;
    float x;
    float y;

    JS_OBJ(id, x, y);
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
    int texture_id;
    int default_player_x;
    int default_player_y;
    std::vector<Door> doors;
    std::vector<Nest> nests;
    std::vector<MapNPCData> npcs;
    std::string name;

    JS_OBJ(id, name, vertex_width, vertex_height, texture_id, default_player_x, default_player_y, doors, nests, npcs);
  };

  extern float default_tile_width;
  extern float default_tile_height;

  // MapID, MapData
  extern std::map<int, MapData> maps;

  //  TileID, TileData
  extern std::map<int, maps::TileData> tiles;

  // Generate single tile out of position, texture_id and frame_id
  maps::TileData generate_tile(float x, float y, int texture_id, int frame_id);

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