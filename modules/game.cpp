
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
#include "entity.h"
#include "events.h"
#include "fonts.h"
#include "hero.h"
#include "items.h"
#include "logger.h"
#include "maps.h"
#include "menu.h"
#include "mobs.h"
#include "models.h"
#include "mouse.h"
#include "navmesh.h"
#include "npcs.h"
#include "quads.h"
#include "saves.h"
#include "shaders.h"
#include "travel.h"

#include "../scripts/py.h"

#include "../dictionary.h"
#include "../dependencies/parallel_hashmap/phmap.h"

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
  phmap::flat_hash_map<int, game::SceneData> scenes;
  phmap::flat_hash_map<int, sig_ptr> HeroLoader;

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

    game::HeroLoader[SCENE_LOAD_FROM_NEW] = game::_load_hero_from_new_game;
    game::HeroLoader[SCENE_LOAD_FROM_LOAD] = game::_load_hero_from_load_game;
    game::HeroLoader[SCENE_LOAD_CHANGE_LEVEL] = game::_load_hero_from_change_level;
    std::cout << "Scenes Initialized" << std::endl;
  }

  void _load_hero_from_new_game(int scene_id)
  {
    game::HERO_START_X = game::scenes[scene_id].hero_start_x;
    game::HERO_START_Y = game::scenes[scene_id].hero_start_y;

    if(!game::scenes[scene_id].is_gp)
    {
      hero::refresh();
    }
    
    if(game::HERO_START_X != -1000 & game::HERO_START_Y != -1000)
    {
      hero::create_new(menu::NewGameName, "barbarian");
      hero::set_position(game::HERO_START_X, game::HERO_START_Y);
      camera::cam.x = (game::HERO_START_X - (game::WINDOW_WIDTH/2) + (hero::hero.w/2));
      camera::cam.y = - (game::HERO_START_Y - (game::WINDOW_HEIGHT/2) + (hero::hero.h/2));
    }
  };

  void _load_hero_from_load_game(int scene_id)
  {
    if(!game::scenes[scene_id].is_gp)
    {
      hero::refresh();
    }
    std::cout << "Menu Load Game Name: " << menu::LoadGameName << std::endl;
    saves::load_game(menu::LoadGameName);
  };

  void _load_hero_from_change_level(int scene_id)
  {
    if(game::HERO_START_X != -1000 & game::HERO_START_Y != -1000)
    {
      hero::hero.entity_id = entity::create(hero::hero, ENTITY_TYPE_HERO, CAMERA_DYNAMIC);
      hero::set_position(game::HERO_START_X, game::HERO_START_Y);
      camera::cam.x = (game::HERO_START_X - (game::WINDOW_WIDTH/2) + (hero::hero.w/2));
      camera::cam.y = - (game::HERO_START_Y - (game::WINDOW_HEIGHT/2) + (hero::hero.h/2));
    }
  };

  void load_scene(int scene_id, int load_scene_from)
  {
    if(game::scenes.count(scene_id) > 0)
    {    
      models::load(fonts::FONT_MODEL_ID);
      game::SCENE_ID = scene_id;
      game::EVENT_HANDLER_ID = game::scenes[scene_id].events_handler_id;
      game::MAP_ID = game::scenes[scene_id].map_id;
      game::HeroLoader[load_scene_from](scene_id);

      // // Load maps
      maps::init_map(game::MAP_ID);

      // // load mobs based on the map
      // mobs::spawn_from_nest(game::MAP_ID);

      // // Spawns npcs for the map
      // npcs::spawn_from_map(game::MAP_ID);

      //items::spawn(1, 200, 300);

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

      std::cout << "Loaded Scene: " << scene_id << std::endl;
    }
  }
  
  void switch_scene(int scene_id, int load_scene_from)
  {
    game::clear_scene();
    game::load_scene(scene_id, load_scene_from);
    game::SCENE_ID = scene_id;

    // have to cleared after loading the scene
    menu::NewGameName = "";
    menu::LoadGameName = "";

    std::cout << "Finished switching scenes " << std::endl;
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
    models::clear();
    npcs::clear();
    quads::clear();
    debug::clear();
    buttons::clear();
    travel::clear();
  }

  void init()
  {
    //std::cout << " size of quad data: " << sizeof(quads::QuadData) << std::endl;

    quads::clear();
    ecs::init();
    models::init();

    int logo_entity_id = ecs::create_entity_from_file("logo");
    ecs::drop(logo_entity_id);

    anims::init();
    quads::init();
    buffer::init();
    buttons::init();
    collisions::init();
    entity::init();
    events::init();
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

    // Loads scene based on SCENE_ID
    game::load_scene(game::SCENE_ID, false);
    //game::load_scene(SCENE_ID_DUNGEON_LEVEL_1, false);
  };

  void update()
  {
    quads::clear();
    maps::render();
    entity::render();
    debug::render();
    menu::render();
    models::bind();
    buttons::render();
    fonts::render();
    nav::render();
    int sampler_size = (models::SceneModels.size() + 1);
    int sampler[sampler_size]; 
    models::populate_sampler(sampler);
 
    quads::update();
    camera::scale_quads(camera::cam.x, camera::cam.y, camera::cam.zoom);
    logger::log_data();
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

    // draw models here
    // buffer::update_models(models::MeshVertices, models::meshes);
    // glDrawElements(GL_TRIANGLES, models::MeshVertices.size(), GL_UNSIGNED_INT, nullptr);

    //entity::print_entity_data();
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
    game::clear_scene();
    game::scenes.clear();

    hero::refresh();
    items::refresh();
    maps::refresh();
    menu::refresh();
    mobs::refresh();
    //models::refresh();
    npcs::refresh();
    items::init();
    maps::init();
    menu::init();
    mobs::init();
    models::init();
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
