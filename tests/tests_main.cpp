

#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <vector>
#include <map>

#include "../modules/anims.h"
#include "../modules/ecs.h"
#include "../modules/models.h"
#include "../modules/game.h"

#include "tests_anims.h"
#include "tests_camera.h"
#include "tests_collisions.h"
#include "tests_ecs.h"
#include "tests_items.h"
#include "tests_travel.h"
#include "tests_utils.h"

typedef bool (*sig_ptr)();
std::map<std::string, sig_ptr> tests;
float total_tests;
float passed;
float failed;
std::vector<std::string> failed_tests;

int FPS;
Uint32 flags = SDL_WINDOW_OPENGL;
std::string GAME_CONFIG_PATH = "config.json";


void init()
{
  // models and animations are now needed for loading entities to the scene. Entity has model assigned and loaded automatically and animation set to idle
  anims::init();
  ecs::init();
  models::init("./data/models");
  maps::init();
  collisions::init();
  items::init();


  // anims
  tests["test_anims__hero_anim_start"] = test_anims__hero_anim_start;
  tests["test_anims__entity_anim_start"] = test_anims__entity_anim_start;
  tests["test_anims__entity_anim_not_start"] = test_anims__entity_anim_not_start;
  tests["test_anims__entity_anim_start_later"] = test_anims__entity_anim_start_later;
  tests["test_anims__entity_anim_play"] = test_anims__entity_anim_play;
  tests["test_anims__entity_anim_breakable"] = test_anims__entity_anim_breakable;
  tests["test_anims__entity_anim_same_type"] = test_anims__entity_anim_same_type;

  // camera
  tests["test_camera__scale_quad_static"] = test_camera__scale_quad_static;
  tests["test_camera__scale_quad_dynamic"] = test_camera__scale_quad_dynamic;

  // collisions
  tests["test_collisions__set_sensors_on_entity_count"] = test_collisions__set_sensors_on_entity_count;
  tests["test_collisions__set_sensors_on_entity_top"] = test_collisions__set_sensors_on_entity_top;
  tests["test_collisions__set_sensors_on_entity_bottomleft"] = test_collisions__set_sensors_on_entity_bottomleft;
  tests["test_collisions__set_sensors_on_entity_count_after_update"] = test_collisions__set_sensors_on_entity_count_after_update;
  tests["test_collisions__set_sensors_on_entity_top_after_update"] = test_collisions__set_sensors_on_entity_top_after_update;
  tests["test_collisions__check_far_distance"] = test_collisions__check_far_distance;
  tests["test_collisions__check_near_distance"] = test_collisions__check_near_distance;
  tests["test_collisions__check_collision_item"] = test_collisions__check_collision_item;

  // ecs
  tests["test_ecs__create_entity_basic"] = test_ecs__create_entity_basic;
  tests["test_ecs__create_entity_component"] = test_ecs__create_entity_component;
  tests["test_ecs__drop_entity"] = test_ecs__drop_entity;
  tests["test_ecs__set_position"] = test_ecs__set_position;
  tests["test_ecs__update_position"] = test_ecs__update_position;
  tests["test_ecs__update_position_diff"] = test_ecs__update_position_diff;
  tests["test_ecs__revert_position_x"] = test_ecs__revert_position_x;
  tests["test_ecs__revert_position_y"] = test_ecs__revert_position_y;
  tests["test_ecs__save_temp"] = test_ecs__save_temp;

  // items
  tests["test_items__pickup_item"] = test_items__pickup_item;
  tests["test_items__yeet_item"] = test_items__yeet_item;

  // travel
  tests["test_travel__get_angle"] = test_travel__get_angle;
  tests["test_travel__"] = test_travel__;

  // utils
  tests["test_utils_generate_id_first_id_is_1"] = test_utils_generate_id_first_id_is_1;
  tests["test_utils_drop_id_empty_index"] = test_utils_drop_id_empty_index;
  tests["test_utils_re_generate_ids"] = test_utils_re_generate_ids;
  tests["test_utils_re_generate_ids2"] = test_utils_re_generate_ids2;
  tests["test_utils_re_generate_ids3"] = test_utils_re_generate_ids3;

};


void eval(std::string test_name, bool test_result)
{
  if(test_result){
    std::cout << " - \033[1;32mTest \033[0m" << test_name << " \033[1;32m Passed! \033[0m" << std::endl;
    passed += 1;
  }
  else{
    std::cout << " - \033[1;31mTest \033[0m" << test_name << " \033[1;31m Failed! \033[0m" << std::endl;
    failed += 1;
    failed_tests.push_back(test_name);
  }
};

int main()
{
  game::read_config(GAME_CONFIG_PATH);
  SDL_Init(SDL_INIT_VIDEO);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_Window *WINDOW = SDL_CreateWindow("Vikings",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        game::WINDOW_WIDTH,
                                        game::WINDOW_HEIGHT,
                                        flags);
  SDL_GLContext GLCONTEXT = SDL_GL_CreateContext(WINDOW);
  Uint32 current_sdl_gl = SDL_GL_MakeCurrent(WINDOW, GLCONTEXT);
  SDL_GL_SetSwapInterval(1);
  glViewport(0, 0, game::WINDOW_WIDTH, game::WINDOW_HEIGHT);
  GLenum err = glewInit();
  utils::check_glew(err);

  // No need in tests
  game::IS_DEBUG_MODE = false;

  init();
  std::cout << std::endl;
  std::cout << "Starting tests..." << std::endl;
  std::cout << std::endl;

  for (auto const& [k, v]:tests)
  {
    //std::cout << "   [STARTING TEST]   " << k << std::endl;
    total_tests += 1;
    bool test_result = v();
    eval(k, test_result);
  }

  std::cout << std::endl;
  std::cout << "Tests finished" << std::endl;
  std::cout << std::endl;
  std::cout << "======== SUMMARY ========" << std::endl;
  std::cout << "  Total tests: " << total_tests << std::endl;
  std::cout << "    Passed: " << passed << " (" << passed/total_tests << ")" <<std::endl;
  std::cout << "    Failed: " << failed << " (" << failed/total_tests << ")" <<std::endl;
  std::cout << std::endl;

  if(failed>0){
    std::cout << "    Failed test IDS: ";
    for(int f=0; f<failed_tests.size(); f++)
    {
      std::cout << failed_tests[f] << ", ";
    }
    std::cout << std::endl;
  }
  return 0;
}

