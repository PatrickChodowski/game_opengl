#ifndef LEVELS_H
#define LEVELS_H


namespace levels
{

  struct LevelCache
  {
   int level_id;
   int map_id;

   std::vector<mobs::Mob> level_mobs;
   std::vector<items::Item> level_items_on_ground;

   std::vector<int> required_textures;

  };

  std::vector<quads::Quad> load_map_from_file(std::string map_name, int vertex_width, int vertex_height, int map_texture_id)
  {
    std::vector<quads::Quad> tile_map = {};
    std::string file_path = "maps/" + map_name;
    std::ifstream in_file;
    in_file.open(file_path.c_str());

    int n_tiles = vertex_width*vertex_height;
    int TILE_COUNTER = 1;

      // read in the tile info
    if (in_file.is_open())
    {
      for (int r = 0; r < vertex_height; r++)
        {
          for (int c = 0; c < vertex_width; c++)
          {
            struct quads::Quad quad;
            quad.x = c * TILE_DIM;
            quad.y = r * TILE_DIM;
            in_file >> quad.frame_id;
            quad.id = TILE_COUNTER;
            quad.texture_id = map_texture_id;
            tile_map.push_back(quad);
            TILE_COUNTER += 1;
          };
        } 
    }
    in_file.close();

    return tile_map;
  }




  // map_id
  // default player x, y
  // it is supposed to load everything for given level
  // also needs some  cache struct data for mobs situation, items on the ground etc.
  // load needed textures, load vertices etc.
  void load(int map_id, int player_x, int player_y)
  {
    // Load quads for the map first
    std::vector<quads::Quad> level_quads = load_map_from_file(maps::Catalog[map_id].name, 
                                                        maps::Catalog[map_id].vertex_width, 
                                                        maps::Catalog[map_id].vertex_height, 
                                                        maps::Catalog[map_id].texture_id);

    std::vector<quads::Quad> quads = quads::assign_vertices(level_quads);
    buffer::init(quads);
  }



  void unload()
  {

  }

}
  #endif