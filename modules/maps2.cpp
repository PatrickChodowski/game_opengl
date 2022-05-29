#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "hero.h"
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
  float TILE_SCALE = 1.5;

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
    for(int i=0; i<MD._tiles.size(); i++)
    {
      MD._tiles[i].tile_x = MD._tiles[i].x*MD.tile_width;
      MD._tiles[i].tile_y = MD._tiles[i].y*MD.tile_height;
      MD.tiles.insert({MD._tiles[i].id, MD._tiles[i]});
    }
    // 0 1 2 3 
    // 4 5 6 7
    // 8 9 10 11
    // 12 13 14 15

    const int n_dists = 1;

    // Create tile map (for each tile find the one that need to be rendered)
    for(int i=0; i<MD._tiles.size(); i++)
    {
      std::vector<int> tiles_to_render = {};
      int id = MD._tiles[i].id;
      int x = MD._tiles[i].x;
      int y = MD._tiles[i].y;

      // Main tile ID
      tiles_to_render.push_back(MD._tiles[i].id);

      for(int n=1; n<=n_dists; n++)
      {
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
          tiles_to_render.push_back(id + MD.tile_count_x);
        }

        // check one up
        if((y-n) >= 0){
          tiles_to_render.push_back(id - MD.tile_count_x);
        }

        // check top right
        if(((x+1) < MD.tile_count_x) & ((y-1) >= 0)){
          tiles_to_render.push_back(id - MD.tile_count_x + n);
        }

        // check top left
        if(((x-n) >= 0) & ((y-n) >= 0)){
          tiles_to_render.push_back(id - MD.tile_count_x - n);
        }

        // check bottom right
        if(((x+n) < MD.tile_count_x) & ((y+n) < MD.tile_count_y)){
          tiles_to_render.push_back(id + MD.tile_count_x + n);
        }

        // check bottom left
        if(((x-n) >= 0) & ((y+n) < MD.tile_count_y)){
          tiles_to_render.push_back(id + MD.tile_count_x - n);
        }
        MD.tile_map.insert({id, tiles_to_render});
      }

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

    for(int t=0; t < maps2::maps.at(map_id)._tiles.size(); t++)
    {
      maps2::TileData tdd = maps2::maps.at(map_id)._tiles[t];
      if((x >= tdd.tile_x*TILE_SCALE) & (x <= (tdd.tile_x*TILE_SCALE + maps2::maps.at(map_id).tile_width*TILE_SCALE)) & 
         (y >= tdd.tile_y*TILE_SCALE) & (y <= (tdd.tile_y*TILE_SCALE + maps2::maps.at(map_id).tile_height*TILE_SCALE))){
        tile_id = tdd.id;
        break;
      }
    }
    return tile_id;
  }

  void update(int map_id)
  {
    if(map_id > -1){
      int hero_x = ecs::positions.at(hero::HERO_ENTITY_ID).x;
      int hero_y = ecs::positions.at(hero::HERO_ENTITY_ID).y;

      int tile_id = maps2::_get_tile_id(map_id, hero_x, hero_y);
      if(maps2::CURRENT_TILE_ID != tile_id){
        std::cout << " [MAPS] Updating current tile ID from " << maps2::CURRENT_TILE_ID << " to " << tile_id << std::endl;
        maps2::load_tiles(map_id, tile_id);
        maps2::CURRENT_TILE_ID = tile_id;
      };
    }
  }

  void load_tiles(int map_id, int tile_id)
  { 
    maps2::tiles.clear();
    std::vector<int> tiles_to_render = maps2::maps.at(map_id).tile_map.at(tile_id);
    for(int t=0; t < tiles_to_render.size(); t++)
    {
      std::cout << " Adding tile to render: " <<  tiles_to_render[t] << std::endl;
      quads::QuadData tile_quad = maps2::generate_tile(map_id, tiles_to_render[t]);
      maps2::tiles.insert({tiles_to_render[t], tile_quad});
    }
    std::cout << " [MAPS] Loaded tiles, size: " <<  maps2::tiles.size() << std::endl;
  };

  quads::QuadData generate_tile(int map_id, int tile_id)
  {
    // std::cout << " [MAPS] Generating tile " << tile_id << " for map ID: " << map_id << std::endl;
    maps2::MapData MDD = maps2::maps.at(map_id);
    maps2::TileData TDD = maps2::maps.at(map_id).tiles.at(tile_id);

    // is tile an entity? :D
    quads::QuadData tile;
    tile.model_id = MDD.model_id;
    tile.frame_id = tile_id;

    if(tile.model_id > -1){
      tile.texture_id = models::ModelTextureMap.at(tile.model_id);
      models::ModelFrameData MFD =  models::models.at(tile.model_id).frames.at(tile.frame_id);
      tile.norm_x_start = MFD.norm_x_start;
      tile.norm_x_end = MFD.norm_x_end;
      tile.norm_y_start = MFD.norm_y_start;
      tile.norm_y_end = MFD.norm_y_end;
    }

    tile.entity_type_id = ENTITY_TYPE_MAP;
    tile.camera_type = CAMERA_DYNAMIC;
    tile.r = 0.5f;
    tile.g = 0.5f;
    tile.b = 0.5f;
    tile.a = 1.0f;
    tile.x = TDD.tile_x*TILE_SCALE;
    tile.y = TDD.tile_y*TILE_SCALE;

    tile.z = 0.1f;
    tile.w = (MDD.tile_width+1) *TILE_SCALE;
    tile.h = (MDD.tile_height+1)*TILE_SCALE;

    tile.window_x = tile.x;
    tile.window_y = tile.y;
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
      maps2::CURRENT_MAP_ID = map_id;
      int model_id = maps2::maps.at(map_id).model_id;
      models::load(model_id);

      // Initializing map with position 0,0?
      maps2::update(map_id);



      // doors
      // navmesh
      // paths

    }
  }

  void clear()
  {
    maps2::tiles.clear();
    maps2::CURRENT_TILE_ID = -1;
    maps2::CURRENT_MAP_ID = -1;
  }

  void render()
  {
    for (auto const& [tile_id, tile_quad]: maps2::tiles)
    {
      quads::AllQuads.push_back(tile_quad);
    }
  }

  void refresh()
  {
    maps2::tiles.clear();
    maps2::maps.clear();
    maps2::CURRENT_MAP_ID = -1;
    maps2::CURRENT_TILE_ID = -1;
  }

}
