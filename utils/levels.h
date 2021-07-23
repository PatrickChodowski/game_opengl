#ifndef LEVELS_H
#define LEVELS_H

// Level is the same thing as "scene"

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
    // int TILE_COUNTER = 1; // old version, need to use get_next_quad_id

      // read in the tile info
    if (in_file.is_open())
    {
      for (int r = 0; r < vertex_height; r++)
        {
          for (int c = 0; c < vertex_width; c++)
          {
            struct quads::Quad quad;
            quad.x = c * camera::tile_dim;
            quad.y = r * camera::tile_dim;
            quad.w = camera::tile_dim;
            quad.h = camera::tile_dim;
            in_file >> quad.frame_id;
            //quad.id = TILE_COUNTER;
            quad.id = qm::gen_quad_id();
            quad.texture_id = texture_id;
            quad.is_clicked = 0.0;
            tile_map.push_back(quad);
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


  void update(std::vector<quads::Quad> quads_vector)
  {
    textures::bind_all();
    // just test::
    //std::vector<quads::Quad> quads = quads::assign_vertices(quads_vector);
    std::vector<quads::Quad> quads = quads_vector;
    buffer::update(quads_vector);
    // test finish


    glClear(GL_COLOR_BUFFER_BIT);

    // sampler array creation
    int sampler_size = (textures::BoundTextures.size() + 1); //3
    int sampler[sampler_size]; 
    sampler[0] = 0;
    for (int i = 0; i < textures::BoundTextures.size(); i++) {
      sampler[(i+1)] = textures::BoundTextures[i];
    }

    // react to camera changes
    glm::mat4 MVP = camera::generate_mvp(camera::zoom, -camera::x, camera::y);
    ScaledLevelQuads = camera::scale_move_quads(quads_vector, -camera::x, camera::y);

    // set uniforms
    glUniform1iv(glGetUniformLocation(shaders::Catalog[CURRENT_SHADER_ID].gl_shader_id, "textures"), sampler_size, sampler);
    glUniformMatrix4fv(glGetUniformLocation(shaders::Catalog[CURRENT_SHADER_ID].gl_shader_id, "mvp"), 1, GL_FALSE, glm::value_ptr(MVP));
    
    // set shader
    glUseProgram(shaders::Catalog[CURRENT_SHADER_ID].gl_shader_id);
    // bind buffer
    glBindVertexArray(buffer::VAO); 
    // draw 
    glDrawElements(GL_TRIANGLES, quads.size()*6, GL_UNSIGNED_INT, nullptr);

  }



  void drop()
  {

  }


}
  #endif