#include <iostream>
#include <map>
#include <string>
#include <vector>


#include "game.h"

#include "buffer.h"
#include "camera.h"
#include "entity.h"
#include "hero.h"
#include "maps.h"
#include "mobs.h"
#include "quads.h"
#include "shaders.h"
#include "textures.h"

namespace game2
{
  bool RUNNING = true;
  int CURRENT_SHADER_ID = 0;
  float TILE_DIM = 96;
  float WINDOW_VERTEX_WIDTH = 10;
  float WINDOW_VERTEX_HEIGHT = 8;
  float WINDOW_WIDTH = WINDOW_VERTEX_WIDTH*TILE_DIM;
  float WINDOW_HEIGHT = WINDOW_VERTEX_HEIGHT*TILE_DIM;
  bool CHANGE_STATE_TRIGGER = false;
  
  std::map<std::string, bool> GAME_STATE;
  std::vector<std::string> GAME_STATE_LIST = {"GAME_ON",
                                              "MAIN_MENU",
                                              "NEW_GAME_MENU",
                                              "LOAD_GAME_MENU",
                                              "SETTINGS_MENU"};

  void _init_game_states()
  {
    GAME_STATE.insert(std::pair<std::string, bool>("GAME_ON", true));
    GAME_STATE.insert(std::pair<std::string, bool>("MAIN_MENU", false));
    GAME_STATE.insert(std::pair<std::string, bool>("NEW_GAME_MENU", false));
    GAME_STATE.insert(std::pair<std::string, bool>("LOAD_GAME_MENU", false));
    GAME_STATE.insert(std::pair<std::string, bool>("SETTINGS_MENU", false));
  };

  void set_state(std::string state_name)
  {
    std::string old_state = get_state();
    if(old_state != state_name){
      GAME_STATE[state_name] = true;
      for(int s = 0; s < GAME_STATE_LIST.size(); s++)
      {
        if(GAME_STATE_LIST[s] != state_name){
          GAME_STATE[GAME_STATE_LIST[s]] = false;
        }
      }
      std::cout << "Set the game state to " << state_name << std::endl;
      CHANGE_STATE_TRIGGER = true;
    }
  }

  std::string get_state()
  {
    std::string current_state;
    for(int s = 0; s < GAME_STATE_LIST.size(); s++)
    {
      if(GAME_STATE[GAME_STATE_LIST[s]])
      {
        current_state = GAME_STATE_LIST[s];
        break;
      }
    }
    std::cout << "Current state: " << current_state << std::endl;
    return current_state;
  }


  void init()
  {
    game2::_init_game_states();
    game2::set_state("GAME_ON");

    buffer2::init();
    maps2::init();
    mobs2::init();
    shaders2::init();
    textures2::init();

    hero2::create_new("john","barbarian");
    maps2::init_map(hero2::hero.map_id);
    mobs2::spawn(hero2::hero.map_id);

  };

  void update()
  {
    entity::render();
    quads2::accumulate();
    textures2::bind();
    //buffer2::update(quads2::AllQuads);

    // buffer2::update(entity::EntityQuads);
    buffer2::update(maps2::MapQuads);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // sampler array creation
    int sampler_size = (textures2::BoundTextures.size() + 1);
    int sampler[sampler_size]; 
    sampler[0] = 0;
    for (int i = 0; i < textures2::BoundTextures.size(); i++)
    {
      sampler[(i+1)] = textures2::BoundTextures[i];
    }


    // react to camera changes
    camera::DYNAMIC_MVP = camera::gen_dynamic_mvp(-camera::cam.x, camera::cam.y, camera::cam.zoom);

    // independent of camera moving
    camera::STATIC_MVP = camera::gen_static_mvp();

    // zoom only
    camera::ZOOM_MVP = camera::gen_zoom_only_mvp(camera::cam.zoom);
  
    // set light source coordinates
    float light_coords[3] = {100, 200, 300};
    glUniform3fv(glGetUniformLocation(shaders2::shaders[CURRENT_SHADER_ID].gl_shader_id, "light_coords"), 1, light_coords);

    // set uniforms
    glUniform1iv(glGetUniformLocation(shaders2::shaders[CURRENT_SHADER_ID].gl_shader_id, "textures"), sampler_size, sampler);
    glUniformMatrix4fv(glGetUniformLocation(shaders2::shaders[CURRENT_SHADER_ID].gl_shader_id, "static_mvp"), 1, GL_FALSE, glm::value_ptr(camera::STATIC_MVP));
    glUniformMatrix4fv(glGetUniformLocation(shaders2::shaders[CURRENT_SHADER_ID].gl_shader_id, "dynamic_mvp"), 1, GL_FALSE, glm::value_ptr(camera::DYNAMIC_MVP));
    glUniformMatrix4fv(glGetUniformLocation(shaders2::shaders[CURRENT_SHADER_ID].gl_shader_id, "zoom_mvp"), 1, GL_FALSE, glm::value_ptr(camera::ZOOM_MVP));
    
    // set shader
    glUseProgram(shaders2::shaders[CURRENT_SHADER_ID].gl_shader_id);
    // draw 
    glDrawElements(GL_TRIANGLES, quads2::AllQuads.size()*6, GL_UNSIGNED_INT, nullptr);

  }


  void drop()
  {
    buffer2::drop();
    maps2::drop();
    mobs2::drop();
    shaders2::drop();
    textures2::drop();
  }


}
