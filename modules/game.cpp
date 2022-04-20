
#include <chrono>
#include <iostream>
#include <SDL2/SDL.h>
#include <string>
#include <vector>


#include "game.h"

#include "anims.h"
#include "buffer.h"
#include "buttons.h"
#include "camera.h"
#include "collisions.h"
#include "debug.h"
#include "ecs.h"
#include "events.h"
#include "fonts.h"
#include "hero.h"
#include "items.h"
#include "logger.h"
#include "maps.h"
#include "mobs.h"
#include "models.h"
#include "mouse.h"
#include "navmesh.h"
#include "npcs.h"
#include "quads.h"
#include "saves.h"
#include "scenes.h"
#include "shaders.h"
#include "sounds.h"
#include "travel.h"

#include "../scripts/py.h"

#include "../dictionary.h"
#include "../dependencies/parallel_hashmap/phmap.h"

namespace game
{
  bool RUNNING = true;
  bool PAUSE = false;
  bool IS_DEBUG_MODE;
  int CURRENT_SHADER_ID = 0;
  float WINDOW_WIDTH;
  float WINDOW_HEIGHT;
  const Uint8 *KEYBOARD = SDL_GetKeyboardState(NULL);
  std::chrono::time_point<std::chrono::high_resolution_clock> GAME_START_TIME = std::chrono::high_resolution_clock::now();
  game::ExternalConfigData Config;

    // Current scene event handler
  int SCENE_ID;
  int EVENT_HANDLER_ID;
  int MAP_ID;
  float HERO_START_X;
  float HERO_START_Y;

  void clear()
  {
    anims::clear();
    collisions::clear();
    maps::clear();
    nav::clear();
    camera::reset();
    ecs::clear();
    models::clear();
    quads::clear();
    debug::clear();
    travel::clear();
  }

  void init()
  {
    //sounds::init();
    quads::clear();
    ecs::init();
    models::init();

    anims::init();
    quads::init();
    buffer::init();
    buttons::init();
    collisions::init();
    events::init();
    hero::init();
    items::init();
    logger::init();
    maps::init();
    mobs::init();
    mouse::init();
    npcs::init();
    scenes::init();
    scripts::init();
    shaders::init();

    // Loads scene based on SCENE_ID
    scenes::load(game::SCENE_ID, false);
    //scenes::load(SCENE_ID_DUNGEON_LEVEL_1, false);
  };

  void update()
  {
    quads::clear();
    travel::update();
    collisions::update();
    anims::update();
    maps::render();
    quads::render();
    fonts::render();
    nav::render();

    int sampler_size = (models::SceneModels.size() + 1);
    int sampler[sampler_size];
    models::make_sampler(sampler, sampler_size);
 
    quads::update();
    camera::scale_quads(camera::cam.x, camera::cam.y, camera::cam.zoom);
    buffer::update_quads(quads::AllQuads);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT); // -> only if depth test is enabled

    // react to camera changes
    camera::DYNAMIC_MVP = camera::gen_dynamic_mvp(-camera::cam.x, camera::cam.y, camera::cam.zoom);

    // independent of camera moving
    camera::STATIC_MVP = camera::gen_static_mvp();
  
    // Set Light properties
    float light_coords[3] = {100, 200, 300};
    glUniform3fv(glGetUniformLocation(shaders::shaders[CURRENT_SHADER_ID].gl_shader_id, "light_coords"), 1, light_coords);


    // glm::vec4 light_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    // glm::vec3 light_pos = glm::vec3(0.5f, 0.5f, 0.5f);
    // glm::mat4 light_model = glm::mat4(1.0f);
    // light_model = glm::translate(light_model, light_pos);

    // glUniform4f(glGetUniformLocation(shaders::shaders[CURRENT_SHADER_ID].gl_shader_id, "light_color"), light_color.x, light_color.y, light_color.z, light_color.w);
	  // glUniform3f(glGetUniformLocation(shaders::shaders[CURRENT_SHADER_ID].gl_shader_id, "light_pos"), light_pos.x, light_pos.y, light_pos.z);

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
    shaders::drop();
    models::drop();
    scripts::drop();
  }

  void refresh()
  {
    game::clear();
    scenes::scenes.clear();

    items::refresh();
    maps::refresh();
    mobs::refresh();
    npcs::refresh();
    items::init();
    maps::init();
    mobs::init();
    models::init();
    npcs::init();
    scenes::load(game::SCENE_ID, false);
  };

  void read_config(std::string& config_path)
  {
    std::string json_data = utils::read_text_file(config_path);
    JS::ParseContext context(json_data);
    context.parseTo(game::Config);
    game::WINDOW_WIDTH = game::Config.window_width;
    game::WINDOW_HEIGHT = game::Config.window_height;
    game::SCENE_ID = game::Config.starting_scene;
    game::IS_DEBUG_MODE = game::Config.is_debug_mode;
  }

}
