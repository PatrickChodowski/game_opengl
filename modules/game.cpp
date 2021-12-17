
#include <chrono>
#include <iostream>
#include <map>
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
#include "navmesh.h"
#include "npcs.h"
#include "quads.h"
#include "shaders.h"
#include "textures.h"
#include "travel.h"

#include "../scripts/py.h"

#include "../dictionary.h"

namespace game
{
  bool RUNNING = true;
  bool PAUSE = false;
  bool IS_DEBUG_MODE = false;
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
  bool LOG_TO_FILES = false;
  std::map<int, game::SceneData> scenes;

  void read_data(std::string& name)
  {
    game::SceneData SD;
    std::string data_path = "./data/scenes/"+name+".json";
    std::string json_data = utils::read_text_file(data_path);
    JS::ParseContext context(json_data);
    context.parseTo(SD);
    game::scenes.insert({SD.id, SD});
  };

  void init_scenes()
  {
    std::vector<std::string> scene_list = utils::list_json_files("data/scenes");
    for(int s=0; s < scene_list.size(); s++)
    {
      game::read_data(scene_list[s]);
    };
  }

  void load_scene(int scene_id, bool from_save)
  {
    if(game::scenes.count(scene_id) > 0)
    {    
      game::SCENE_ID = scene_id;
      game::EVENT_HANDLER_ID = game::scenes[scene_id].events_handler_id;
      game::MAP_ID = game::scenes[scene_id].map_id;
      game::HERO_START_X = game::scenes[scene_id].hero_start_x;
      game::HERO_START_Y = game::scenes[scene_id].hero_start_y;

      if(!game::scenes[scene_id].is_gp)
      {
        hero::refresh();
      }

      // Load maps
      maps::init_map(game::MAP_ID);

      // load mobs based on the map
      mobs::spawn_from_nest(game::MAP_ID);

      // Spawns npcs for the map
      npcs::spawn_from_map(game::MAP_ID);

      // Load menu slots
      for(int s=0; s<game::scenes[scene_id].menu_slots.size(); s++)
      {
        int slot_id = game::scenes[scene_id].menu_slots[s];
        menu::currentmenuslots[slot_id] = menu::menuslots[slot_id];
      }
      // Load menus
      for(int t=0; t<game::scenes[scene_id].menu_types.size(); t++)
      {
        int menu_type_id = game::scenes[scene_id].menu_types[t];
        menu::add(menu_type_id);
      }

      // Set hero position and centralize camera if new game or switch level but not from save
      if((game::HERO_START_X != -1000 & game::HERO_START_Y != -1000) & !from_save)
      {
        hero::set_position(game::HERO_START_X, game::HERO_START_Y);
        camera::cam.x = (game::HERO_START_X - (game::WINDOW_WIDTH/2) + (hero::hero.w/2));
        camera::cam.y = - (game::HERO_START_Y - (game::WINDOW_HEIGHT/2) + (hero::hero.h/2));
      }
    }
  }
  
  void switch_scene(int scene_id, bool from_save)
  {
    game::clear_scene();
    game::load_scene(scene_id, from_save);
    game::SCENE_ID = scene_id;
  }

  void clear_scene()
  {
    anims::clear();
    collisions::clear();
    maps::clear();
    nav::clear();
    camera::reset();
    entity::clear();
    fonts::clear();
    menu::clear();
    mobs::clear();
    npcs::clear();
    quads::clear();
    debug::clear();
    buttons::clear();
    travel::clear();

    // saves::save()
  }

  void init()
  {
    //std::cout << " size of quad data: " << sizeof(quads::QuadData) << std::endl;

    quads::clear();
    anims::init();
    buffer::init();
    buttons::init();
    collisions::init();
    entity::init();
    events::init();
    fonts::init("Ignotum"); // its important to keep it before textures becuase of bindings
    items::init();
    logger::init();
    maps::init();
    menu::init();
    mobs::init();
    mouse::init();
    npcs::init();
    game::init_scenes();
    scripts::init();
    shaders::init();
    textures::init();

    // Loads scene based on SCENE_ID
    game::load_scene(game::SCENE_ID, false);
    //game::load_scene(SCENE_ID_DUNGEON_LEVEL_1, false);
  };

  void update()
  {
    // std::cout << "hero x y:" << hero::hero.x <<  ", " << hero::hero.y << std::endl;
    // std::cout << "entities size:" << entity::entities.size() << std::endl;
    // std::cout << "hero entity id: " << hero::hero.entity_id << std::endl;
    // std::cout << "entity hero x y " << entity::entities[hero::hero.entity_id].pos.x << ", " << entity::entities[hero::hero.entity_id].pos.y << std::endl;

    quads::clear();
    maps::render();
    entity::render();
    debug::render();
    menu::render();
    buttons::render();
    fonts::render();
    nav::render();
  
    quads::update();
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
    shaders::drop();
    textures::drop();
    scripts::drop();
  }

  void refresh()
  {
    game::clear_scene();
    game::scenes.clear();

    anims::refresh();
    hero::refresh();
    items::refresh();
    maps::refresh();
    menu::refresh();
    mobs::refresh();
    npcs::refresh();

    anims::init();
    items::init();
    maps::init();
    menu::init();
    mobs::init();
    npcs::init();
    game::init_scenes();

    hero::create_new("test", "barbarian");
    game::load_scene(game::SCENE_ID, false);

  };


  void read_config(std::string& config_path)
  {
    std::string json_data = utils::read_text_file(config_path);
    JS::ParseContext context(json_data);
    context.parseTo(game::Config);
    game::WINDOW_WIDTH = game::Config.window_width;
    game::WINDOW_HEIGHT = game::Config.window_height;
    game::SCENE_ID = game::Config.starting_scene;
    game::LOG_TO_FILES  = game::Config.log_to_files;
  }

}
