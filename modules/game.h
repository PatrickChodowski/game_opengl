
#include <chrono>
#include <map>
#include <vector>
#include <SDL2/SDL.h>
#include <string>

#include "../dependencies/json_struct.h"

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
    std::vector<int> menu_slots;
    std::vector<int> menu_types;
    float hero_start_x, hero_start_y;
    std::string label;

    JS_OBJ(id, events_handler_id, map_id, hero_start_x, hero_start_y, menu_slots, menu_types, label);
  };

  extern int EVENT_HANDLER_ID;
  extern int MAP_ID;
  extern float HERO_START_X;
  extern float HERO_START_Y;
  extern std::map<int, game::SceneData> scenes;

  // Reads scene data to struct
  void read_data(std::string &name);

  // Reads all scenes data to scenes catalog
  void init_scenes();

  // Initialize all systems for new scene
  void load_scene(int scene_id, bool from_save);

  // Delete all data necessary for clear the scene
  void clear_scene();

  // Switches scene. Clears current scene data and initializes new one
  void switch_scene(int scene_id, bool from_save);

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

}



#endif
