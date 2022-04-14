
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "maps.h"
#include "models.h"
#include "navmesh.h"
#include "pathfinder.h"
#include "quads.h"
#include "utils.h"
#include "../dictionary.h"
#include "../dependencies/parallel_hashmap/phmap.h"

namespace maps
{
  phmap::flat_hash_map<int, quads::QuadData> tiles;
  phmap::flat_hash_map<int, maps::MapData> maps = {};
  float default_tile_width = 100;
  float default_tile_height = 100;

  void read_data(std::string name)
  {
    std::string data_path = "./data/maps/"+name+".json";
    maps::MapData MD;
    std::string json_data = utils::read_text_file(data_path);
    JS::ParseContext context(json_data);
    context.parseTo(MD);
    maps::maps.insert({MD.id, MD});
  };

  void init()
  { 
    std::vector<std::string> maps_list = utils::list_json_files("data/maps");
    for(int m=0; m < maps_list.size(); m++)
    {
      maps::read_data(maps_list[m]);
    }
    std::cout << "Maps Initialized" << std::endl;
  };

  void clear()
  {
    maps::tiles.clear();
  }

  void refresh()
  {
    maps::tiles.clear();
    maps::maps.clear();
    maps::init();
  }

  quads::QuadData generate_tile(float x, float y, int model_id, int frame_id)
  {
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

    //tile.is_solid = models::models.at(tile.model_id).frames.at(tile.frame_id).is_solid;
    return tile;
  }

  void load(int map_id)
  {
    maps::tiles.clear();
    std::string file_path = "data/maps/" + maps::maps[map_id].name;
    std::ifstream in_file;
    in_file.open(file_path.c_str());
    int n_tiles =  maps::maps[map_id].vertex_width*maps::maps[map_id].vertex_height;
    int model_id = maps::maps[map_id].model_id;
    models::load(model_id);
    int tile_id = 50000;

    if (in_file.is_open()){
      for (int r = 0; r < maps::maps[map_id].vertex_height; r++)
        {
          for (int c = 0; c < maps::maps[map_id].vertex_width; c++)
          {
            int frame_id;
            in_file >> frame_id;
            quads::QuadData tile = maps::generate_tile(c * maps::default_tile_width, 
                                                       r * maps::default_tile_height, 
                                                       model_id, 
                                                       frame_id);
            tile.entity_id = tile_id;
            tile_id += 1;
            maps::tiles.insert(std::pair<int, quads::QuadData>{tile.entity_id, tile});
          };
        } 
    }
    in_file.close();
    std::cout << " [MAPS] Loaded map ID: " <<  map_id << std::endl;
  }

  void init_map(int map_id)
  { 
    if(map_id > -1)
    {
      maps::load(map_id);
      nav::init(map_id);
      paths::make_path_map();
    }
  };

  void render()
  {
    for (auto const& [tile_id, tile_quad]: maps::tiles)
    {
      quads::AllQuads.push_back(tile_quad);
    }
  }
}
