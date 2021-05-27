#ifndef LEVELS_H
#define LEVELS_H

// Level is the same thing as "scene"

namespace levels
{

  std::vector<quads::Quad> LevelQuads;


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
  void init(int map_id, int player_x, int player_y)
  {
    // Load quads for the map first
    LevelQuads = load_map_from_file(maps::Catalog[map_id].name, 
                                    maps::Catalog[map_id].vertex_width, 
                                    maps::Catalog[map_id].vertex_height, 
                                    maps::Catalog[map_id].texture_id);

    std::vector<quads::Quad> quads = quads::assign_vertices(LevelQuads);
    buffer::init(quads);
  }

  std::vector<int> make_texture_sampler()
  {
    // for now mock logic
    std::vector<int> sampler = {0,1};
    return sampler;
  }


  void update()
  {
    std::vector<quads::Quad> quads = quads::assign_vertices(LevelQuads);
    buffer::update(quads);

    glClear(GL_COLOR_BUFFER_BIT);
    std::vector<int> sampler_v = make_texture_sampler();
    // int* sampler = &sampler_v[0];

    int sampler[1] = {0};
    //glUniform2f(glGetUniformLocation(shaders::Catalog[0], "LightCoord"), light_coords[0], light_coords[1]);

    // send texture sampler to shader
    
    glUniform1iv(glGetUniformLocation(shaders::Catalog[0].gl_shader_id, "textures"), 1, sampler);

    glUniform1f(glGetUniformLocation(shaders::Catalog[0].gl_shader_id, "red_color"), 0.3f);

    glm::mat4 MVP = camera::generate_mvp(camera::zoom, -camera::x, camera::y);
    glUniformMatrix4fv(glGetUniformLocation(shaders::Catalog[0].gl_shader_id, "mvp"), 1, GL_FALSE, glm::value_ptr(MVP));
    glUseProgram(shaders::Catalog[0].gl_shader_id);
    textures::bind(0, 0);
    glBindVertexArray(buffer::VAO); 
    glDrawElements(GL_TRIANGLES, quads.size()*6, GL_UNSIGNED_INT, nullptr);
  }



  void drop()
  {

  }

}
  #endif