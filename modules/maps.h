#include <vector>
#include <map>
#include <string>
#include "quads.h"
#include "collisions.h"
#include "../dependencies/json_struct.h"

#ifndef MODULES_MAPS_H
#define MODULES_MAPS_H

// Persistent data. Table of tiles and all operations on this table. Loaded on new level
namespace maps2
{
  struct TileData
  {
    int id;
    int texture_id;
    int frame_id;
    int camera_type;

    float x;
    float y;
    float h;
    float w;
    float mid_x;
    float mid_y;
    float diag;
  
    // scaled space
    float s_x;
    float s_y;
    float s_h;
    float s_w;
    float s_mid_x;
    float s_mid_y;
    float s_diag;


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

  // Door information
  struct Door
  {
    int door_id;
    int x;
    int y;
    int dest_map_id;
    int player_enter_x;
    int player_enter_y;

    JS_OBJ(door_id, x, y, dest_map_id, player_enter_x, player_enter_y);
  };

  struct MapData
  {
    int id;
    std::string name;
    int vertex_width;
    int vertex_height;
    int texture_id;
    int default_player_x;
    int default_player_y;
    std::vector<Door> doors;
    std::vector<Nest> nests;

    JS_OBJ(id, name, vertex_width, vertex_height, texture_id, default_player_x, default_player_y, doors, nests);
  };

  extern float default_tile_width;
  extern float default_tile_height;

  // MapID, MapData
  extern std::map<int, MapData> maps;

  //  TileID, TileData
  extern std::map<int, maps2::TileData> tiles;
  extern std::vector<int> UsedTileIds;
  extern std::vector<quads2::QuadData> MapQuads;


  // Reads json data by map name and stores it inside maps::maps
  void read_map_data(std::string name);

  // Reads list of maps from maps/data/ and for each map there performs 
  // read_map_data function. Thats reads json into maps::maps
  void init();

  // Loads map tiles from map file by map name to maps2::tiles
  void load_map(std::string map_name, 
                int vertex_width, 
                int vertex_height, 
                int texture_id);

  // Loads selected map to game
  void init_map(int map_id);

  // Finds next available tile id 
  int _find_next_tile_id();

  // Generate new tile id
  int _gen_tile_id();

  // clears the currently loaded map -> erases used tiles ids and tiles catalog
  void _clear_current_map();


}


#endif