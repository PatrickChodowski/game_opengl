#ifndef GAME_H
#define GAME_H


// all the loop/scene/game view related stuff I hope
namespace game
{

  float light_coords[3] = {hero::HERO_X + (hero::HERO_WIDTH/2), hero::HERO_Y + (hero::HERO_HEIGHT/2), 300};
  glm::mat4 STATIC_MVP;
  glm::mat4 DYNAMIC_MVP;
  glm::mat4 ZOOM_MVP;


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
      camera::reset();
      menu::drop();
      maps::drop_map();
      ent::drop_entities();
      fonts::drop_texts();
      CURRENT_SHADER_ID = 0;
      if(GAME_STATE["MAIN_MENU"])
      {
        camera::speed = 0;
        menu::load_menu({0,1,2,3});
      } else if(GAME_STATE["NEW_GAME_MENU"]){
        camera::speed = 0;
        menu::load_menu({4,5});
      } else if(GAME_STATE["GAME_ON"]){
        menu::NewGameName = "";
        camera::speed = camera::base_speed;

        if(NEW_GAME){
          maps::init_map(MAP_ID, maps::Catalog[MAP_ID].default_player_x, maps::Catalog[MAP_ID].default_player_y);
          fonts::render_text(CAMPAIGN_NAME.c_str(), 600, 50, textures::FontTD, 0.5, 0.5, 0.5, 0.5, 1.0);
          quads::Quad hero = ent::render_entity(0,3,0, hero::HERO_X, hero::HERO_Y, hero::HERO_HEIGHT, hero::HERO_WIDTH, 2.0f,textures::FontTD);
          ent::EntityQuads.push_back(hero);
        } else {
          saves::load_game(CAMPAIGN_NAME);
          maps::init_map(MAP_ID, maps::Catalog[MAP_ID].default_player_x, maps::Catalog[MAP_ID].default_player_y);
          fonts::render_text(CAMPAIGN_NAME.c_str(), 600, 50, textures::FontTD, 0.5, 0.5, 0.5, 0.5, 1.0);
          quads::Quad hero = ent::render_entity(0,3,0, hero::HERO_X, hero::HERO_Y, hero::HERO_HEIGHT, hero::HERO_WIDTH, 2.0f,textures::FontTD);
          ent::EntityQuads.push_back(hero);
        }
 
      } else if(GAME_STATE["LOAD_GAME_MENU"]){
        camera::speed = 0;
        menu::load_menu({6, 1});
      }
    CHANGE_STATE_TRIGGER = false;
   }
   else {
     if(GAME_STATE["GAME_ON"]){
      // if the game state didnt change and we are in GAME_ON mode
      // reset entities only
      ent::drop_entities();

      // render entity again
      quads::Quad hero = ent::render_entity(0, 3, hero::current_frame, hero::HERO_X, hero::HERO_Y, hero::HERO_HEIGHT, hero::HERO_WIDTH, 2.0f,textures::FontTD);
      ent::EntityQuads.push_back(hero);
     } 




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

    // react to camera changes
    DYNAMIC_MVP = camera::generate_dynamic_mvp(camera::zoom, -camera::x, camera::y);

    // independent of camera moving
    STATIC_MVP = camera::generate_static_mvp();

    // zoom only for hero?
    ZOOM_MVP = camera::generate_zoom_only_mvp(camera::zoom);
    

    // this should react to map quads only ?
    quads::ScaledAllQuads = camera::scale_move_quads(quads::AllQuads, -camera::x, camera::y);


    // set light source coordinates
    glUniform3fv(glGetUniformLocation(shaders::Catalog[CURRENT_SHADER_ID].gl_shader_id, "light_coords"), 1, light_coords);

    // set uniforms
    glUniform1iv(glGetUniformLocation(shaders::Catalog[CURRENT_SHADER_ID].gl_shader_id, "textures"), sampler_size, sampler);
    glUniformMatrix4fv(glGetUniformLocation(shaders::Catalog[CURRENT_SHADER_ID].gl_shader_id, "static_mvp"), 1, GL_FALSE, glm::value_ptr(STATIC_MVP));
    glUniformMatrix4fv(glGetUniformLocation(shaders::Catalog[CURRENT_SHADER_ID].gl_shader_id, "dynamic_mvp"), 1, GL_FALSE, glm::value_ptr(DYNAMIC_MVP));
    glUniformMatrix4fv(glGetUniformLocation(shaders::Catalog[CURRENT_SHADER_ID].gl_shader_id, "zoom_mvp"), 1, GL_FALSE, glm::value_ptr(ZOOM_MVP));
    
    // set shader
    glUseProgram(shaders::Catalog[CURRENT_SHADER_ID].gl_shader_id);
    // draw 
    glDrawElements(GL_TRIANGLES, quads.size()*6, GL_UNSIGNED_INT, nullptr);
  }

}




#endif