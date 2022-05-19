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



  void init_map(int map_id)
  {


  }




}
