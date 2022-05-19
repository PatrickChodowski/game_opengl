#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "ecs.h"
#include "maps2.h"
#include "models.h"
#include "navmesh.h"
#include "pathfinder.h"
#include "quads.h"
#include "utils.h"
#include "../dictionary.h"
#include "../dependencies/parallel_hashmap/phmap.h"

namespace maps2
{
  phmap::flat_hash_map<int, maps2::MapData> maps = {};
  phmap::flat_hash_map<int, quads::QuadData> tiles = {};
  int CURRENT_TILE_ID = -1;
  int CURRENT_MAP_ID = -1;

  void init()
  {
    std::vector<std::string> maps_list = utils::list_json_files("data/maps2");
    for(int m=0; m < maps_list.size(); m++)
    {
      maps2::read_data(maps_list[m]);
    }
    std::cout << " [Maps2] Initialized" << std::endl;
  }

  void read_data(std::string name)
  {
    std::string data_path = "./data/maps2/"+name+".json";
    maps2::MapData MD;
    std::string json_data = utils::read_text_file(data_path);
    JS::ParseContext context(json_data);
    context.parseTo(MD);

    // Handle tile real coordinates within map
    for(int i=0; i<MD.tiles.size(); i++)
    {
      MD.tiles[i].tile_x = MD.tiles[i].x*MD.tile_width;
      MD.tiles[i].tile_y = MD.tiles[i].y*MD.tile_height;
    }
    // 0 1 2 3 
    // 4 5 6 7
    // 8 9 10 11
    // 12 13 14 15

    int n = 1;
    // Create tile map (for each tile find the one that need to be rendered)
    for(int i=0; i<MD.tiles.size(); i++)
    {
      std::vector<int> tiles_to_render = {};
      int id = MD.tiles[i].id;
      int x = MD.tiles[i].x;
      int y = MD.tiles[i].y;

      // Main tile ID
      tiles_to_render.push_back(MD.tiles[i].id);

      // check one to the right
      if((x+n) < MD.tile_count_x){
        tiles_to_render.push_back(id + n);
      }

      // check one to the left
      if((x-n) >= 0){
        tiles_to_render.push_back(id - n);
      }

      // check one down
      if((y+n) < MD.tile_count_y){
        tiles_to_render.push_back(id + MD.tile_count_y);
      }

      // check one up
      if((y-n) >= 0){
        tiles_to_render.push_back(id - MD.tile_count_y);
      }

      // check top right
      if(((x+1) < MD.tile_count_x) & ((y-1) >= 0)){
        tiles_to_render.push_back(id - MD.tile_count_y + n);
      }

      // check top left
      if(((x-n) >= 0) & ((y-n) >= 0)){
        tiles_to_render.push_back(id - MD.tile_count_y - n);
      }

      // check bottom right
      if(((x+n) < MD.tile_count_x) & ((y+n) < MD.tile_count_y)){
        tiles_to_render.push_back(id + MD.tile_count_y + n);
      }

      // check bottom left
      if(((x-n) >= 0) & ((y+n) < MD.tile_count_y)){
        tiles_to_render.push_back(id + MD.tile_count_y - n);
      }
      MD.tile_map.insert({id, tiles_to_render});

      // std::cout << " tile ID: " << id << std::endl;
      // for(int t=0; t<tiles_to_render.size(); t++)
      // {
      //   std::cout << tiles_to_render[t] << ", ";
      // }
      // std::cout << std::endl;
    }

    maps2::maps.insert({MD.map_id, MD});
  };

  int _get_tile_id(int map_id, int x, int y)
  { 
    int tile_id = -1;
    // check in which tile the player is

    for(int t=0; t < maps2::maps.at(map_id).tiles.size(); t++)
    {
      maps2::TileData tdd = maps2::maps.at(map_id).tiles[t];
      if((x >= tdd.tile_x) & (x <= (tdd.tile_x + maps2::maps.at(map_id).tile_width)) & 
         (y >= tdd.tile_y) & (y <= (tdd.tile_y + maps2::maps.at(map_id).tile_height))){
        tile_id = tdd.id;
        break;
      }
    }
    return tile_id;
  }

  void update(int map_id, int x, int y)
  {
    int tile_id = maps2::_get_tile_id(map_id, x, y);
    if(maps2::CURRENT_TILE_ID != tile_id){
      maps2::load_tiles(map_id, tile_id);
      maps2::CURRENT_TILE_ID = tile_id;
    };
  }

  void load_tiles(int map_id, int tile_id)
  { 
    maps2::tiles.clear();
    std::vector<int> tiles_to_render = maps2::maps.at(map_id).tile_map.at(tile_id);
    for(int t=0; t < tiles_to_render.size(); t++)
    {
      quads::QuadData tile = maps2::generate_tile();
      maps2::tiles.insert({tile_id, tile});
    }
  };


  quads::QuadData generate_tile(int model_id, int frame_id)
  {
    // is tile an entity? :D
    quads::QuadData tile;
    tile.model_id = model_id;
    tile.frame_id = frame_id;
    if(tile.model_id > -1){
      tile.texture_id = models::ModelTextureMap.at(tile.model_id);
      tile.norm_x_start = models::models.at(tile.model_id).frames.at(tile.frame_id).norm_x_start;
      tile.norm_x_end = models::models.at(tile.model_id).frames.at(tile.frame_id).norm_x_end;
      tile.norm_y_start = models::models.at(tile.model_id).frames.at(tile.frame_id).norm_y_start;
      tile.norm_y_end = models::models.at(tile.model_id).frames.at(tile.frame_id).norm_y_end;
    }
    tile.entity_type_id = ENTITY_TYPE_MAP;
    tile.camera_type = CAMERA_DYNAMIC;
    tile.r = 0.5f;
    tile.g = 0.5f;
    tile.b = 0.5f;
    tile.a = 1.0f;
    tile.x = x;
    tile.y = y;
    tile.z = 0.1f;
    tile.w = maps::default_tile_width;
    tile.h = maps::default_tile_height;
    tile.window_x = x;
    tile.window_y = y;
    tile.window_h = tile.h;
    tile.window_w = tile.w;
    tile.camera_type = CAMERA_DYNAMIC;
    tile.is_clicked = false;

    return tile;

  }

  


  void init_map(int map_id)
  {
    if(map_id > -1)
    {
      std::cout << " [MAPS] Started initializing map id: " << map_id << std::endl;
      // maps2::load(map_id);

      // doors
      // navmesh
      // paths




    }


  }


  void clear()
  {
    maps2::tiles.clear();
  }

  void render()
  {
    for (auto const& [tile_id, tile_quad]: maps2::tiles)
    {
      quads::AllQuads.push_back(tile_quad);
    }
  }



}
