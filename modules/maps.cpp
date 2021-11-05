#include <vector>
#include <map>
#include <string>
#include <fstream>
#include "maps.h"
#include "quads.h"
#include "utils.h"

namespace maps2
{
  std::vector<quads2::QuadData> TileQuads;
  std::map<int, maps2::TileData> tiles;
  std::vector<int> UsedTileIds = {};
  std::map<int, maps2::MapData> maps = {};


  void read_map_data(std::string name)
  {
    std::string data_path = "./maps/data/"+name+".json";
    maps2::MapData MD;
    std::string json_data = utils2::read_text_file(data_path);
    JS::ParseContext context(json_data);
    context.parseTo(MD);
    maps2::maps.insert({MD.id, MD});
  };

  void init()
  { 
    std::vector<std::string> maps_list = utils2::list_json_files("maps/data/");
    for(int m=0; m < maps_list.size(); m++)
    {
      maps2::read_map_data(maps_list[m]);
    }
  };


  std::vector<quads2::QuadData> load_map_from_file(std::string map_name, 
                                                   int vertex_width, 
                                                   int vertex_height, 
                                                   int texture_id)

  {
    std::string file_path = "maps/" + map_name;
    std::vector<quads2::QuadData> tile_map = {};
    std::ifstream in_file;
    in_file.open(file_path.c_str());

    int n_tiles = vertex_width*vertex_height;

    // yes no solid array rowsxcols
    bool solid_array[vertex_height][vertex_width];

    // read in the tile info
    if (in_file.is_open())
    {
      for (int r = 0; r < vertex_height; r++)
        {
          for (int c = 0; c < vertex_width; c++)
          {
            struct quads::Quad quad;
            quad.id = quads::gen_quad_id();
            quad.x = c * camera::tile_dim;
            quad.y = r * camera::tile_dim;
            quad.w = camera::tile_dim;
            quad.h = camera::tile_dim;
            in_file >> quad.frame_id; // value of tile in text file
            quad.s_x = c * camera::tile_dim;
            quad.s_y = r * camera::tile_dim;
            quad.s_w = camera::tile_dim;
            quad.s_h = camera::tile_dim;
  
            quad.texture_id = texture_id;
            quad.is_clicked = 0.0;
            quad.type_id = QUAD_TYPE_MAP;
            quad.is_static = 0.0f;

            quad.solid = false;
            quad.coll = false;
            solid_array[r][c] = false;

            // if frame_id is between 10 and 20, then its solid
            if(quad.frame_id > 10 && quad.frame_id < 20){
              // 11-19
              quad.solid = true;
              quad.coll = true;
              solid_array[r][c] = true;
            } 

            quad.entity_type_id = ENTITY_TYPE_ID_NA;
            quad.alive = false;
            quad.r_col = 0.5;
            quad.g_col = 0.5;
            quad.b_col = 0.5;
            quad.a_col = 0.5;

            tile_map.push_back(quad);
          };
        } 
    }
    in_file.close();
    return tile_map;
  }


  void init_map(int map_id)
  {

  };








}