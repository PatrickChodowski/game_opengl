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
    /*
      Loads map quads from map file


    */


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

            quad.id = qm::gen_quad_id();
            quad.texture_id = texture_id;
            quad.is_clicked = 0.0;
            quad.type_id = QUAD_TYPE_LEVEL;
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

    levels::LevelQuads = load_map_from_file(maps::Catalog[map_id].name, 
                                    maps::Catalog[map_id].vertex_width, 
                                    maps::Catalog[map_id].vertex_height, 
                                    maps::Catalog[map_id].texture_id);

    for(int i = 0; i < levels::LevelQuads.size(); i++)
    { 
      levels::LevelQuads[i].a = qm::gen_vertex_id();
      levels::LevelQuads[i].b = qm::gen_vertex_id();
      levels::LevelQuads[i].c = qm::gen_vertex_id();
      levels::LevelQuads[i].d = qm::gen_vertex_id();

      float norm_x_start = (float)textures::Catalog[levels::LevelQuads[i].texture_id].frames[levels::LevelQuads[i].frame_id].x/
      (float)textures::Catalog[levels::LevelQuads[i].texture_id].width;

      float norm_x_end =  (float)(textures::Catalog[levels::LevelQuads[i].texture_id].frames[levels::LevelQuads[i].frame_id].x + 
      textures::Catalog[levels::LevelQuads[i].texture_id].frames[levels::LevelQuads[i].frame_id].w)/
      (float)textures::Catalog[levels::LevelQuads[i].texture_id].width;

      // create vertex struct - A
      levels::LevelQuads[i].v_a.vertex_id = levels::LevelQuads[i].a;
      levels::LevelQuads[i].v_a.tile_id = levels::LevelQuads[i].id;
      levels::LevelQuads[i].v_a.frame_id = levels::LevelQuads[i].frame_id;
      levels::LevelQuads[i].v_a.x_pos = (float)levels::LevelQuads[i].x;
      levels::LevelQuads[i].v_a.y_pos = (float)levels::LevelQuads[i].y;
      levels::LevelQuads[i].v_a.z_pos = 0.0f;
      levels::LevelQuads[i].v_a.r_col = 0.0f;
      levels::LevelQuads[i].v_a.g_col = 0.0f;
      levels::LevelQuads[i].v_a.b_col = 0.0f;
      levels::LevelQuads[i].v_a.a_col = 1.0f;
      levels::LevelQuads[i].v_a.tex_coord_x = norm_x_start;
      levels::LevelQuads[i].v_a.tex_coord_y = 0.0f;
      levels::LevelQuads[i].v_a.texture_id = textures::Catalog[levels::LevelQuads[i].texture_id].opengl_texture_id;
      levels::LevelQuads[i].v_a.is_clicked = (float)levels::LevelQuads[i].is_clicked;
      levels::LevelQuads[i].v_a.type_id = (float)levels::LevelQuads[i].type_id;

      // create vertex struct - B
      levels::LevelQuads[i].v_b.vertex_id = levels::LevelQuads[i].b;
      levels::LevelQuads[i].v_b.tile_id = levels::LevelQuads[i].id;
      levels::LevelQuads[i].v_b.frame_id = levels::LevelQuads[i].frame_id;
      levels::LevelQuads[i].v_b.x_pos = (float)levels::LevelQuads[i].x + (float)TILE_DIM  - (float)quads::VERTEX_OFFSET;
      levels::LevelQuads[i].v_b.y_pos = (float)levels::LevelQuads[i].y;
      levels::LevelQuads[i].v_b.z_pos = 0.0f;
      levels::LevelQuads[i].v_b.r_col = 0.0f;
      levels::LevelQuads[i].v_b.g_col = 0.0f;
      levels::LevelQuads[i].v_b.b_col = 0.0f;
      levels::LevelQuads[i].v_b.a_col = 1.0f;
      levels::LevelQuads[i].v_b.tex_coord_x = norm_x_end;
      levels::LevelQuads[i].v_b.tex_coord_y = 0.0f;
      levels::LevelQuads[i].v_b.texture_id = textures::Catalog[levels::LevelQuads[i].texture_id].opengl_texture_id;
      levels::LevelQuads[i].v_b.is_clicked = (float)levels::LevelQuads[i].is_clicked;
      levels::LevelQuads[i].v_b.type_id = (float)levels::LevelQuads[i].type_id;

      // create vertex struct - C
      levels::LevelQuads[i].v_c.vertex_id = levels::LevelQuads[i].c;
      levels::LevelQuads[i].v_c.tile_id = levels::LevelQuads[i].id;
      levels::LevelQuads[i].v_c.frame_id = levels::LevelQuads[i].frame_id;
      levels::LevelQuads[i].v_c.x_pos = (float)levels::LevelQuads[i].x;
      levels::LevelQuads[i].v_c.y_pos = (float)levels::LevelQuads[i].y + (float)TILE_DIM - (float)quads::VERTEX_OFFSET;
      levels::LevelQuads[i].v_c.z_pos = 0.0f;
      levels::LevelQuads[i].v_c.r_col = 0.0f;
      levels::LevelQuads[i].v_c.g_col = 0.0f;
      levels::LevelQuads[i].v_c.b_col = 0.0f;
      levels::LevelQuads[i].v_c.a_col = 1.0f;
      levels::LevelQuads[i].v_c.tex_coord_x = norm_x_start;
      levels::LevelQuads[i].v_c.tex_coord_y = 1.0f;
      levels::LevelQuads[i].v_c.texture_id = textures::Catalog[levels::LevelQuads[i].texture_id].opengl_texture_id;
      levels::LevelQuads[i].v_c.is_clicked = (float)levels::LevelQuads[i].is_clicked;
      levels::LevelQuads[i].v_c.type_id = (float)levels::LevelQuads[i].type_id;


      // create vertex struct - D
      levels::LevelQuads[i].v_d.vertex_id = levels::LevelQuads[i].d;
      levels::LevelQuads[i].v_d.tile_id = levels::LevelQuads[i].id;
      levels::LevelQuads[i].v_d.frame_id = levels::LevelQuads[i].frame_id;
      levels::LevelQuads[i].v_d.x_pos = (float)levels::LevelQuads[i].x + (float)TILE_DIM  - (float)quads::VERTEX_OFFSET;
      levels::LevelQuads[i].v_d.y_pos = (float)levels::LevelQuads[i].y + (float)TILE_DIM - (float)quads::VERTEX_OFFSET;
      levels::LevelQuads[i].v_d.z_pos = 0.0f;
      levels::LevelQuads[i].v_d.r_col = 1.0f;
      levels::LevelQuads[i].v_d.g_col = 1.0f;
      levels::LevelQuads[i].v_d.b_col = 1.0f;
      levels::LevelQuads[i].v_d.a_col = 1.0f;
      levels::LevelQuads[i].v_d.tex_coord_x = norm_x_end;
      levels::LevelQuads[i].v_d.tex_coord_y = 1.0f;
      levels::LevelQuads[i].v_d.texture_id = textures::Catalog[levels::LevelQuads[i].texture_id].opengl_texture_id;
      levels::LevelQuads[i].v_d.is_clicked = (float)levels::LevelQuads[i].is_clicked;
      levels::LevelQuads[i].v_d.type_id = (float)levels::LevelQuads[i].type_id;

      // create vindices 
      levels::LevelQuads[i].i_left.a = levels::LevelQuads[i].a;
      levels::LevelQuads[i].i_left.b = levels::LevelQuads[i].b;
      levels::LevelQuads[i].i_left.c = levels::LevelQuads[i].c;

      levels::LevelQuads[i].i_right.a = levels::LevelQuads[i].b;
      levels::LevelQuads[i].i_right.b = levels::LevelQuads[i].c;
      levels::LevelQuads[i].i_right.c = levels::LevelQuads[i].d;
  }

    if(LOGGING==-1)
    {
      print_out_quads(levels::LevelQuads);
    }

  }


  void update(std::vector<quads::Quad> quads)
  {
    textures::bind_all();
    buffer::update(quads);

    // glClear(GL_COLOR_BUFFER_BIT); // black by default
    //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // sampler array creation
    int sampler_size = (textures::BoundTextures.size() + 1);
    int sampler[sampler_size]; 
    sampler[0] = 0;
    for (int i = 0; i < textures::BoundTextures.size(); i++) {
      sampler[(i+1)] = textures::BoundTextures[i];
    }

    // react to camera changes
    glm::mat4 MVP = camera::generate_mvp(camera::zoom, -camera::x, camera::y);

    // this should react to level quads only
    ScaledLevelQuads = camera::scale_move_quads(quads, -camera::x, camera::y);

    // set uniforms
    glUniform1iv(glGetUniformLocation(shaders::Catalog[CURRENT_SHADER_ID].gl_shader_id, "textures"), sampler_size, sampler);
    glUniformMatrix4fv(glGetUniformLocation(shaders::Catalog[CURRENT_SHADER_ID].gl_shader_id, "mvp"), 1, GL_FALSE, glm::value_ptr(MVP));
    
    // set shader
    glUseProgram(shaders::Catalog[CURRENT_SHADER_ID].gl_shader_id);
    // draw 
    glDrawElements(GL_TRIANGLES, quads.size()*6, GL_UNSIGNED_INT, nullptr);

  }



  void drop()
  {
    /*
      Drops current level

      - needs to remove quads
      - what if next level will be bigger?

      - dont know
    */

    for(int q = 0; q < levels::LevelQuads.size(); q++)
    {
      qm::delete_quad_id(levels::LevelQuads[q].id);
      qm::delete_vertex_id(levels::LevelQuads[q].a);
      qm::delete_vertex_id(levels::LevelQuads[q].b);
      qm::delete_vertex_id(levels::LevelQuads[q].c);
      qm::delete_vertex_id(levels::LevelQuads[q].d);
    }
    levels::LevelQuads.clear();
  }


}
  #endif