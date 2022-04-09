
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

  struct SceneData 
  {
    int id;
    int events_handler_id;
    int map_id;
    float hero_start_x, hero_start_y;
    std::string label;
    std::vector<std::string> entities;
    bool is_gp; // is it gameplay
    
    JS_OBJ(id, events_handler_id, map_id, hero_start_x, hero_start_y, label, is_gp, entities);
  };

  extern int EVENT_HANDLER_ID;
  extern int MAP_ID;
  extern float HERO_START_X;
  extern float HERO_START_Y;
  extern phmap::flat_hash_map<int, game::SceneData> scenes;


  typedef void (*sig_ptr)(int scene_id);
  // Functions map to loading a hero
  extern phmap::flat_hash_map<int, sig_ptr> HeroLoader;

  // Reads scene data to struct
  void read_data(std::string &name);

  // Reads all scenes data to scenes catalog
  void init_scenes();

  // Initialize all systems for new scene
  void load_scene(int scene_id, int load_scene_from);

  // Delete all data necessary for clear the scene
  void clear_scene();

  // Switches scene. Clears current scene data and initializes new one
  void switch_scene(int scene_id, int load_scene_from);

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
