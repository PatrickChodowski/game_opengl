#include <fstream>
#include <map>
#include <string>
#include <vector>
#include "quads.h"
#include "maps.h"
#include "utils.h"
#include "../dictionary.h"

namespace maps2
{
  std::map<int, maps2::TileData> tiles;
  std::vector<int> UsedTileIds = {};
  std::map<int, maps2::MapData> maps = {};
  float default_tile_width = 96;
  float default_tile_height = 96;
  std::vector<quads2::QuadData> MapQuads = {};


  void read_map_data(std::string name)
  {
    std::string data_path = "./data/maps/"+name+".json";
    maps2::MapData MD;
    std::string json_data = utils2::read_text_file(data_path);
    JS::ParseContext context(json_data);
    context.parseTo(MD);
    maps2::maps.insert({MD.id, MD});
  };

  void init()
  { 
    std::vector<std::string> maps_list = utils2::list_json_files("data/maps");
    for(int m=0; m < maps_list.size(); m++)
    {
      maps2::read_map_data(maps_list[m]);
    }
  };


  void load_map(std::string map_name, 
                int vertex_width, 
                int vertex_height, 
                int texture_id)
  {
    maps2::_clear_current_map();
    std::string file_path = "data/maps/" + map_name;
    std::ifstream in_file;
    in_file.open(file_path.c_str());
    int n_tiles = vertex_width*vertex_height;

    // read in the tile info
    if (in_file.is_open())
    {
      for (int r = 0; r < vertex_height; r++)
        {
          for (int c = 0; c < vertex_width; c++)
          {
            struct maps2::TileData tile;
            tile.id = maps2::_gen_tile_id();

            tile.x = c * maps2::default_tile_width;
            tile.y = r * maps2::default_tile_height;
            tile.w = maps2::default_tile_width;
            tile.h = maps2::default_tile_height;
            in_file >> tile.frame_id;

            tile.texture_id = texture_id;
            
            tile.camera_type = CAMERA_DYNAMIC;
            tile.is_clicked = false;
            tile.is_solid = false;

            // if frame_id is between 10 and 20, then its solid (11-19)
            if(tile.frame_id > 10 && tile.frame_id < 20)
            {
              tile.is_solid = true;
            } 

            maps2::tiles[tile.id] = tile;
          };
        } 
    }
    in_file.close();
  }

  void init_map(int map_id)
  {
    load_map(maps2::maps[map_id].name,
             maps2::maps[map_id].vertex_width,
             maps2::maps[map_id].vertex_height,
             maps2::maps[map_id].texture_id);

    // nav::init(maps::Catalog[map_id].name, 
    //           maps::Catalog[map_id].vertex_width, 
    //           maps::Catalog[map_id].vertex_height);
    // paths::make_path_map();

    // Make map quads:
    maps2::MapQuads.clear();
    maps2::MapQuads = quads2::make_quads(maps2::tiles, OBJECT_TYPE_MAP);

  };

  int _find_next_tile_id()
  {
    int n = maps2::UsedTileIds.size();
    for (int i = 0; i < n; i++)
    {
      if (maps2::UsedTileIds[i] > (i+1))
      {
        return i+1;
      }
    }
    return n+1;
  };

  int _gen_tile_id()
  {
    int next_tile_id = maps2::_find_next_tile_id();
    maps2::UsedTileIds.push_back(next_tile_id);
    return next_tile_id;
  };


  void _clear_current_map()
  {
    maps2::UsedTileIds.clear();
    maps2::tiles.clear();
  }


  void drop()
  {

    // for(int q = 0; q < maps::MapQuads.size(); q++)
    // {
    //   quads::delete_quad_id(maps::MapQuads[q].id);
    //   quads::delete_vertex_id(maps::MapQuads[q].a);
    //   quads::delete_vertex_id(maps::MapQuads[q].b);
    //   quads::delete_vertex_id(maps::MapQuads[q].c);
    //   quads::delete_vertex_id(maps::MapQuads[q].d);
    // }
    // maps::MapQuads.clear();
    // travel::TravelControl.clear();
    // nav::NavMesh.clear();
    // nav::NavMeshGraph.clear();
  }




}