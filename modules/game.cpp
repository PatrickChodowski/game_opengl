
#include <chrono>
#include <iostream>
#include <map>
#include <SDL2/SDL.h>
#include <string>
#include <vector>


#include "game.h"

#include "buffer.h"
#include "buttons.h"
#include "camera.h"
#include "collisions.h"
#include "debug.h"
#include "entity.h"
#include "events.h"
#include "fonts.h"
#include "hero.h"
#include "items.h"
#include "logger.h"
#include "maps.h"
#include "menu2.h"
#include "mobs.h"
#include "mouse.h"
#include "navmesh.h"
#include "quads.h"
#include "scenes.h"
#include "shaders.h"
#include "textures.h"
#include "travel.h"

#include "../dictionary.h"

namespace game
{
  bool RUNNING = true;
  bool PAUSE = false;
  bool IS_DEBUG_MODE = true;
  int CURRENT_SHADER_ID = 0;
  float WINDOW_WIDTH = 960;
  float WINDOW_HEIGHT = 768;
  const Uint8 *KEYBOARD = SDL_GetKeyboardState(NULL);
  std::chrono::time_point<std::chrono::high_resolution_clock> GAME_START_TIME = std::chrono::high_resolution_clock::now();


  void init_scene(int scene_id, bool is_new_game)
  {
    logger::log(LOG_LVL_INFO, 
                "start game::init_scene","game::init_scene",
               __FILE__,__LINE__, LOG_START_TIMER);

    if (scene_id < MAIN_MENU_SCENE_ID)
    {
      if(is_new_game)
      {
        hero::create_new("john","barbarian");
      } else 
      {
        // saves::load()
      }
      maps::init_map(scenes::MAP_ID);
      mobs::spawn(scenes::MAP_ID);
      //items::put_item_on_ground(0, 600, 500);
    }
    //menu::load(scene_id);

    logger::log(LOG_LVL_INFO, 
                "finish game::init_scene","game::init_scene",
               __FILE__,__LINE__, LOG_END_TIMER);
  }


  void switch_scene(int scene_id, bool is_new_game = false)
  {
    game::clear_scene();
    game::init_scene(scene_id, is_new_game);
  }

  void clear_scene()
  {
    maps::clear();
    nav::clear();
    camera::reset();
    entity::clear();
    fonts::clear();
    menu::clear();
    quads::clear();
    debug::clear();
    buttons::clear();
    travel::clear();

    // saves::save()
  }

  void init()
  {
    buffer::init();
    buttons::init();
    collisions::init();
    events::init();
    fonts::init("Ignotum"); // its important to keep it before textures becuase of bindings
    items::init();
    logger::init();
    maps::init();
    menu::init();
    mobs::init();
    mouse::init();
    scenes::init();
    shaders::init();
    textures::init();

    // Loads scene based on SCENE_ID
    scenes::load(SCENE_ID_MAIN_MENU);
  };

  void update()
  {
    maps::render();
    entity::render();
    debug::render();
    menu::render();
    buttons::render();
    fonts::render();
  

    quads::accumulate();
    camera::scale_quads(camera::cam.x, camera::cam.y, camera::cam.zoom);
    logger::log_data();
    textures::bind();

    buffer::update_quads(quads::AllQuads);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // sampler array creation
    int sampler_size = (textures::BoundTextures.size() + 1);
    int sampler[sampler_size]; 
    sampler[0] = 0;
    for (int i = 0; i < textures::BoundTextures.size(); i++)
    {
      sampler[(i+1)] = textures::BoundTextures[i];
    } 

    // react to camera changes
    camera::DYNAMIC_MVP = camera::gen_dynamic_mvp(-camera::cam.x, camera::cam.y, camera::cam.zoom);

    // independent of camera moving
    camera::STATIC_MVP = camera::gen_static_mvp();

  
    // set light source coordinates
    float light_coords[3] = {100, 200, 300};
    glUniform3fv(glGetUniformLocation(shaders::shaders[CURRENT_SHADER_ID].gl_shader_id, "light_coords"), 1, light_coords);

    // set uniforms
    glUniform1iv(glGetUniformLocation(shaders::shaders[CURRENT_SHADER_ID].gl_shader_id, "textures"), sampler_size, sampler);
    glUniformMatrix4fv(glGetUniformLocation(shaders::shaders[CURRENT_SHADER_ID].gl_shader_id, "static_mvp"), 1, GL_FALSE, glm::value_ptr(camera::STATIC_MVP));
    glUniformMatrix4fv(glGetUniformLocation(shaders::shaders[CURRENT_SHADER_ID].gl_shader_id, "dynamic_mvp"), 1, GL_FALSE, glm::value_ptr(camera::DYNAMIC_MVP));

    // set shader
    glUseProgram(shaders::shaders[CURRENT_SHADER_ID].gl_shader_id);
    // draw scene
    glDrawElements(GL_TRIANGLES, quads::AllQuads.size()*6, GL_UNSIGNED_INT, nullptr);

    // draw debug lines here
    buffer::update_lines(debug::lines);
    glDrawArrays(GL_LINES, 0, debug::lines.size()*2);

    debug::clear();
  }

  void drop()
  {
    buffer::drop();
    mobs::clear();
    shaders::drop();
    textures::drop();
  }


}
