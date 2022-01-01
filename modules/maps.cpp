
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "maps.h"
#include "navmesh.h"
#include "pathfinder.h"
#include "quads.h"
#include "textures.h"
#include "utils.h"
#include "../dictionary.h"
#include "../dependencies/parallel_hashmap/phmap.h"

namespace maps
{
  phmap::flat_hash_map<int, maps::TileData> tiles;
  phmap::flat_hash_map<int, maps::MapData> maps = {};
  float default_tile_width = 96;
  float default_tile_height = 96;

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
    std::cout << "Maps Initialized" << std::endl;
  };

  void refresh()
  {
    maps::maps.clear();
  }


  maps::TileData generate_tile(float x, float y, int texture_id, int frame_id)
  {
    maps::TileData tile;
    tile.pos.x = x;
    tile.pos.y = y;
    tile.pos.z = 1.0f;
    tile.dims.w = maps::default_tile_width;
    tile.dims.h = maps::default_tile_height;
    tile.diag = std::sqrt(std::pow((tile.dims.w/2),2) + std::pow((tile.dims.h/2),2));
    tile.frame_id = frame_id;
    tile.texture_id = texture_id;
    tile.camera_type = CAMERA_DYNAMIC;
    tile.is_clicked = false;
    tile.is_solid = false;

    tile.color.r = 0.5f;
    tile.color.g = 0.5f;
    tile.color.b = 0.5f;
    tile.color.a = 1.0f;

    tile.norm.x_start = textures::_get_normalized_frame_x_start(tile.texture_id, tile.frame_id);
    tile.norm.x_end = textures::_get_normalized_frame_x_end(tile.texture_id, tile.frame_id);
    tile.norm.y_start = textures::_get_normalized_frame_y_start(tile.texture_id, tile.frame_id);
    tile.norm.y_end = textures::_get_normalized_frame_y_end(tile.texture_id, tile.frame_id);

    // if frame_id is between 10 and 20, then its solid (11-19)
    if(tile.frame_id > 10 && tile.frame_id < 20)
    {
      tile.is_solid = true;
    } 
    return tile;
  }

  void load(int map_id)
  {
    maps::clear();
    std::string file_path = "data/maps/" + maps::maps[map_id].name;
    std::ifstream in_file;
    in_file.open(file_path.c_str());
    int n_tiles =  maps::maps[map_id].vertex_width*maps::maps[map_id].vertex_height;
    int texture_id = maps::maps[map_id].texture_id;

    int tile_id = 0;
    //std::cout << "tile count: " << n_tiles << std::endl;  
    // read in the tile info
    if (in_file.is_open())
    {
      for (int r = 0; r < maps::maps[map_id].vertex_height; r++)
        {
          for (int c = 0; c < maps::maps[map_id].vertex_width; c++)
          {
            //std::cout << "reading tile " << r << ", " << c << std::endl;
            int frame_id;
            in_file >> frame_id;
            maps::TileData tile = maps::generate_tile(c * maps::default_tile_width, 
                                                      r * maps::default_tile_height, 
                                                      texture_id, 
                                                      frame_id);
            tile.id = tile_id;
            tile_id += 1;
            maps::tiles[tile.id] = tile;
            //std::cout << " done tile " << r << ", " << c << std::endl;
          };
        } 
    }
    in_file.close();
  }

  void init_map(int map_id)
  { 
    //std::cout <<" started init map " << std::endl;
    if(map_id > -1)
    {
      maps::load(map_id);
      //std::cout <<" finished map load " << std::endl;
      // Generate navmesh based on the map_id
      nav::init(map_id);

      //std::cout <<" finished navmesh " << std::endl;

      // Generate Path map
      paths::make_path_map();

      //std::cout <<" finished path map " << std::endl;
    }
    //std::cout <<" finished init map " << std::endl;
  };

  void render()
  {
    quads::add_quads(maps::tiles, OBJECT_TYPE_MAP);
  }

  void clear()
  {
    maps::tiles.clear();
  }


}
