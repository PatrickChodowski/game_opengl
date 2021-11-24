
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
#include "debug.h"
#include "entity.h"
#include "events.h"
#include "fonts.h"
#include "gui.h"
#include "hero.h"
#include "items.h"
#include "logger.h"
#include "maps.h"
#include "menu.h"
#include "mobs.h"
#include "mouse.h"
#include "navmesh.h"
#include "quads.h"
#include "shaders.h"
#include "textures.h"

#include "../dictionary.h"

namespace game2
{
  bool RUNNING = true;
  bool PAUSE = false;
  int IS_MENU = IN_GAME_SCENE_ID; // 200 if in game, some menu ID if a menu
  bool IS_DEBUG_MODE = true;
  int CURRENT_SHADER_ID = 0;
  int SCENE_ID = 2;
  float TILE_DIM = 96;
  float WINDOW_VERTEX_WIDTH = 10;
  float WINDOW_VERTEX_HEIGHT = 8;
  float WINDOW_WIDTH = WINDOW_VERTEX_WIDTH*TILE_DIM;
  float WINDOW_HEIGHT = WINDOW_VERTEX_HEIGHT*TILE_DIM;
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
        hero2::create_new("john","barbarian");
      } else 
      {
        // saves::load()
      }
      maps2::init_map(scene_id);
      mobs2::spawn(scene_id);
      gui2::init();
      items2::put_item_on_ground(0, 600, 500);
    }
    menu2::load(scene_id);
    game2::SCENE_ID = scene_id;
    game2::_check_if_menu();

    logger::log(LOG_LVL_INFO, 
                "finish game::init_scene","game::init_scene",
               __FILE__,__LINE__, LOG_END_TIMER);
  }

  void _check_if_menu()
  { 
    if(game2::SCENE_ID < MAIN_MENU_SCENE_ID)
    {
      game2::IS_MENU = IN_GAME_SCENE_ID;
    } else 
    {
      game2::IS_MENU = game2::SCENE_ID;
    }
  }

  void switch_scene(int scene_id, bool is_new_game = false)
  {
    game2::clear_scene();
    game2::init_scene(scene_id, is_new_game);
  }

  void clear_scene()
  {
    maps2::clear();
    nav::clear();
    camera::reset();
    entity::clear();
    fonts2::clear();
    menu2::clear();
    quads2::clear();
    debug2::clear();
    gui2::clear();
    buttons::clear();

    // saves::save()
  }

  void init()
  {
    buffer2::init();
    buttons::init();
    events2::init();
    fonts2::init("Ignotum"); // its important to keep it before textures becuase of bindings
    gui2::init();
    items2::init();
    logger::init();
    maps2::init();
    menu2::init();
    mobs2::init();
    mouse2::init();
    shaders2::init();
    textures2::init();
    game2::init_scene(2, true);
  };

  void update()
  {
    maps2::render();
    entity::render();
    debug2::render();
    menu2::render();
    gui2::render();
    buttons::render();
    fonts2::render();
  

    quads2::accumulate();
    camera::scale_quads(camera::cam.x, camera::cam.y, camera::cam.zoom);
    logger::log_data();
    textures2::bind();

    buffer2::update_quads(quads2::AllQuads);

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
    // draw scene
    glDrawElements(GL_TRIANGLES, quads2::AllQuads.size()*6, GL_UNSIGNED_INT, nullptr);

    // draw debug lines here
    buffer2::update_lines(debug2::lines);
    glDrawArrays(GL_LINES, 0, debug2::lines.size()*2);

  }


  void drop()
  {
    buffer2::drop();
    mobs2::drop();
    shaders2::drop();
    textures2::drop();
  }


}
