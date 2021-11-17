#include <iostream>
#include <map>
#include <SDL2/SDL.h>
#include <string>
#include <vector>


#include "game.h"

#include "buffer.h"
#include "camera.h"
#include "entity.h"
#include "events.h"
#include "fonts.h"
#include "hero.h"
#include "items.h"
#include "logger.h"
#include "maps.h"
#include "menu.h"
#include "mobs.h"
#include "mouse.h"
#include "quads.h"
#include "shaders.h"
#include "textures.h"

#include "../dictionary.h"

namespace game2
{
  bool RUNNING = true;
  bool PAUSE = false;
  int CURRENT_SHADER_ID = 0;
  int LEVEL_ID = 2;
  float TILE_DIM = 96;
  float WINDOW_VERTEX_WIDTH = 10;
  float WINDOW_VERTEX_HEIGHT = 8;
  float WINDOW_WIDTH = WINDOW_VERTEX_WIDTH*TILE_DIM;
  float WINDOW_HEIGHT = WINDOW_VERTEX_HEIGHT*TILE_DIM;
  const Uint8 *KEYBOARD = SDL_GetKeyboardState(NULL);


  void init_level(int level_id, bool is_new_game)
  {
    if (level_id < MAIN_MENU_LEVEL_ID)
    {
      if(is_new_game)
      {
        hero2::create_new("john","barbarian");
      } else 
      {
        // saves::load()
      }
      maps2::init_map(level_id);
      mobs2::spawn(level_id);
      items2::put_item_on_ground(0, 600, 500);
    }
    menu2::load(level_id);
    fonts2::render();
  }

  void switch_level(int level_id)
  {
    game2::clear_level();
    game2::init_level(level_id, false);
  }

  void clear_level()
  {
    maps2::clear();
    camera::reset();
    entity::clear();
    fonts2::clear();
    menu2::clear();

    // saves::save()
  }

  void init()
  {
    buffer2::init();
    fonts2::init("arial"); // its important to keep it before textures becuase of bindings
    items2::init();
    maps2::init();
    menu2::init();
    mobs2::init();
    mouse2::init();
    shaders2::init();
    textures2::init();
    // hero2::create_new("john","barbarian");
    // hero2::hero.map_id = 100;
    // menu2::render(hero2::hero.map_id);
    // maps2::init_map(hero2::hero.map_id);
    // mobs2::spawn(hero2::hero.map_id);
    // items2::put_item_on_ground(0, 600, 500);

    game2::init_level(2, true);

  };

  void update()
  {
    entity::render();
    menu2::render();
    fonts2::render();

    quads2::accumulate();
    camera::scale_quads(camera::cam.x, camera::cam.y, camera::cam.zoom);
    logger2::log();
    textures2::bind();

    buffer2::update(quads2::AllQuads);
    // buffer2::update(entity::EntityQuads);
    // buffer2::update(maps2::MapQuads);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // sampler array creation


    int sampler_size = (textures2::BoundTextures.size() + 1);
    int sampler[sampler_size]; 
    sampler[0] = 0;
    for (int i = 0; i < textures2::BoundTextures.size(); i++)
    {
      sampler[(i+1)] = textures2::BoundTextures[i];
    } 

    // std::cout << "sampler: ";
    // for(int s=0; s< sampler_size; s++)
    // {
    //   std::cout << sampler[s] << ", ";
    // }
    // std::cout << std::endl;

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
    mobs2::drop();
    shaders2::drop();
    textures2::drop();
  }


}
