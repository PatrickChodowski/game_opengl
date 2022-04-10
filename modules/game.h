
#include <chrono>
#include <vector>
#include <SDL2/SDL.h>
#include <string>

#include "../dependencies/json_struct.h"
#include "../dependencies/parallel_hashmap/phmap.h"

#ifndef MODULES_GAME_H
#define MODULES_GAME_H


namespace game
{
  // Game running variables
  extern bool PAUSE;
  extern bool RUNNING;
  extern bool IS_DEBUG_MODE;
  extern int SCENE_ID;
  extern int CURRENT_SHADER_ID;
  extern float WINDOW_WIDTH;
  extern float WINDOW_HEIGHT;
  extern const Uint8 *KEYBOARD;
  extern std::chrono::time_point<std::chrono::high_resolution_clock> GAME_START_TIME;

  struct ExternalConfigData
  {
    float window_width;
    float window_height;
    int starting_scene;

    JS_OBJ(window_width, window_height, starting_scene);
  };

  extern game::ExternalConfigData Config;
  extern int EVENT_HANDLER_ID;
  extern int MAP_ID;
  extern float HERO_START_X;
  extern float HERO_START_Y;

  typedef void (*sig_ptr)(int scene_id);
  // Functions map to loading a hero
  extern phmap::flat_hash_map<int, sig_ptr> HeroLoader;

  // Delete all data necessary for clear the scene
  void clear();

  // Initialize all in-game systems inside single game::init()
  void init();

  // Update game systems and buffer
  void update();

  // Cleanup after game loop is closed
  void drop();

  // Clear all data and re-initialize
  void refresh();

  // Reads in game config
  void read_config(std::string& config_path);

  // Loads hero from new game logic
  void _load_hero_from_new_game(int scene_id);

  // Loads hero from load game logic
  void _load_hero_from_load_game(int scene_id);

  // Loads hero from change level logic
  void _load_hero_from_change_level(int scene_id);


}



#endif
