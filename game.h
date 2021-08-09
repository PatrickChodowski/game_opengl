#ifndef GAME_H
#define GAME_H


// all the loop/scene/game view related stuff I hope
namespace game
{

  void init()
  {
    // Init data for Maps, Textures etc.
    maps::init();
    shaders::init();
    textures::init();
    menu::init();
    
    // adding font texture to texture catalog
    textures::FontTD = fonts::init(FONT_NAME);
    textures::Catalog.insert({textures::FontTD.opengl_texture_id, textures::FontTD});
    textures::BoundTextures.push_back(textures::FontTD.opengl_texture_id);
  }


  void handle_game_state()
  {
   if(CHANGE_STATE_TRIGGER){ 
      if(GAME_STATE["MAIN_MENU"])
      {
        camera::speed = 0;
        menu::drop();
        maps::drop_map();
        fonts::drop_texts();
        menu::load_menu({0,1,2,3});
      } else if(GAME_STATE["NEW_GAME_MENU"]){
        camera::speed = 0;
        menu::drop();
        maps::drop_map();
        fonts::drop_texts();
        menu::load_menu({4,5});
      } else if(GAME_STATE["GAME_ON"]){
        camera::speed = camera::base_speed;
        menu::drop();
        maps::drop_map();
        fonts::drop_texts();
        maps::init_map(MAP_ID, maps::Catalog[MAP_ID].default_player_x, maps::Catalog[MAP_ID].default_player_y);
      }

    CHANGE_STATE_TRIGGER = false;
   }
  }



  void update(std::vector<quads::Quad> quads)
  {
    textures::bind_all();
    buffer::update(quads);

    // glClear(GL_COLOR_BUFFER_BIT); // black by default
    // glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // sampler array creation
    int sampler_size = (textures::BoundTextures.size() + 1);
    int sampler[sampler_size]; 
    sampler[0] = 0;
    for (int i = 0; i < textures::BoundTextures.size(); i++) {
      sampler[(i+1)] = textures::BoundTextures[i];
    }

    glm::mat4 MVP;
    // react to camera changes
    if(game::GAME_STATE["GAME_ON"]){
      MVP = camera::generate_mvp(camera::zoom, -camera::x, camera::y);
    } else {
      MVP = camera::generate_menu_mvp();
    }

    // this should react to map quads only ?
    quads::ScaledAllQuads = camera::scale_move_quads(quads::AllQuads, -camera::x, camera::y);

    // set uniforms
    glUniform1iv(glGetUniformLocation(shaders::Catalog[CURRENT_SHADER_ID].gl_shader_id, "textures"), sampler_size, sampler);
    glUniformMatrix4fv(glGetUniformLocation(shaders::Catalog[CURRENT_SHADER_ID].gl_shader_id, "mvp"), 1, GL_FALSE, glm::value_ptr(MVP));
    
    // set shader
    glUseProgram(shaders::Catalog[CURRENT_SHADER_ID].gl_shader_id);
    // draw 
    glDrawElements(GL_TRIANGLES, quads.size()*6, GL_UNSIGNED_INT, nullptr);
  }

}




#endif