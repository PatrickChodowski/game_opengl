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

  std::vector<quads::Quad> load_map_from_file(std::string map_name, 
                                              int vertex_width, 
                                              int vertex_height, 
                                              int texture_id)
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
            quad.texture_id = texture_id;
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
    std::cout << "Initializing map id: " << map_id
              << " Map name: " << maps::Catalog[map_id].name
              << " Vertex width: " << maps::Catalog[map_id].vertex_width
              << " Vertex height: " << maps::Catalog[map_id].vertex_height
              << " Map texture id: " << maps::Catalog[map_id].texture_id << std::endl;

    LevelQuads = load_map_from_file(maps::Catalog[map_id].name, 
                                    maps::Catalog[map_id].vertex_width, 
                                    maps::Catalog[map_id].vertex_height, 
                                    maps::Catalog[map_id].texture_id);

    std::vector<quads::Quad> quads = quads::assign_vertices(LevelQuads);
    buffer::init(quads);
  }


  void update()
  {
    textures::bind_all();
    std::vector<quads::Quad> quads = quads::assign_vertices(LevelQuads);
    buffer::update(quads);
    glClear(GL_COLOR_BUFFER_BIT);

    // sampler array creation
    int sampler_size = (textures::BoundTextures.size() + 1); //3
    int sampler[sampler_size]; 
    sampler[0] = 0;
    for (int i = 0; i < textures::BoundTextures.size(); i++) {
      sampler[(i+1)] = textures::BoundTextures[i];
    }
    //logger::print("sampler:");
    //std::cout << sampler[0] << ", " << sampler[1] <<  ", " << sampler[2] <<  ", " << sampler[3] << std::endl;

    // set uniforms
    glUniform1iv(glGetUniformLocation(shaders::Catalog[0].gl_shader_id, "textures"), sampler_size, sampler);
    glm::mat4 MVP = camera::generate_mvp(camera::zoom, -camera::x, camera::y);
    glUniformMatrix4fv(glGetUniformLocation(shaders::Catalog[0].gl_shader_id, "mvp"), 1, GL_FALSE, glm::value_ptr(MVP));
    
    glUseProgram(shaders::Catalog[0].gl_shader_id);
    glBindVertexArray(buffer::VAO); 
    glDrawElements(GL_TRIANGLES, quads.size()*6, GL_UNSIGNED_INT, nullptr);

  }



  void drop()
  {

  }

}
  #endif