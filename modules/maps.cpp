
#include <cmath>
#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "maps.h"
#include "navmesh.h"
#include "pathfinder.h"
#include "quads.h"
#include "textures.h"
#include "utils.h"
#include "../dictionary.h"

namespace maps
{
  std::map<int, maps::TileData> tiles;
  std::vector<int> Index = {};
  std::map<int, maps::MapData> maps = {};
  float default_tile_width = 96;
  float default_tile_height = 96;
  std::vector<quads::QuadData> MapQuads = {};


  void read_map_data(std::string name)
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
      maps::read_map_data(maps_list[m]);
    }
  };


  void load(int map_id)
  {
    maps::clear();
    std::string file_path = "data/maps/" + maps::maps[map_id].name;
    std::ifstream in_file;
    in_file.open(file_path.c_str());
    int n_tiles =  maps::maps[map_id].vertex_width*maps::maps[map_id].vertex_height;

    // read in the tile info
    if (in_file.is_open())
    {
      for (int r = 0; r < maps::maps[map_id].vertex_height; r++)
        {
          for (int c = 0; c < maps::maps[map_id].vertex_width; c++)
          {
            struct maps::TileData tile;
            tile.id = utils::generate_id(maps::Index);

            tile.x = c * maps::default_tile_width;
            tile.y = r * maps::default_tile_height;
            tile.w = maps::default_tile_width;
            tile.h = maps::default_tile_height;
            tile.diag = std::sqrt(std::pow((tile.w/2),2) + std::pow((tile.h/2),2));
            in_file >> tile.frame_id;

            tile.texture_id = maps::maps[map_id].texture_id;
            
            tile.camera_type = CAMERA_DYNAMIC;
            tile.is_clicked = false;
            tile.is_solid = false;

            tile.norm_x_start = textures::_get_normalized_frame_start(maps::maps[map_id].texture_id, tile.frame_id);
            tile.norm_x_end = textures::_get_normalized_frame_end(maps::maps[map_id].texture_id, tile.frame_id);

            // if frame_id is between 10 and 20, then its solid (11-19)
            if(tile.frame_id > 10 && tile.frame_id < 20)
            {
              tile.is_solid = true;
            } 

            maps::tiles[tile.id] = tile;
          };
        } 
    }
    in_file.close();
  }

  void init_map(int map_id)
  {
    maps::load(map_id);

    // Generate navmesh based on the map_id
    nav::init(map_id);

    // Generate Path map
    paths::make_path_map();
  };

  void render()
  {
    maps::MapQuads.clear();
    maps::MapQuads = quads::make_quads(maps::tiles, OBJECT_TYPE_MAP);
  }

  void clear()
  {
    maps::Index.clear();
    maps::tiles.clear();
    maps::MapQuads.clear();
  }


}