
#include <chrono>
#include <map>
#include <vector>
#include <SDL2/SDL.h>
#include <string>

#ifndef MODULES_GAME_H
#define MODULES_GAME_H


namespace game
{
  // Game running variables
  extern bool PAUSE;
  extern bool RUNNING;
  extern bool IS_DEBUG_MODE;
  extern int SCENE_ID;
  extern int IS_MENU;
  extern int CURRENT_SHADER_ID;
  extern float WINDOW_WIDTH;
  extern float WINDOW_HEIGHT;
  extern const Uint8 *KEYBOARD;
  extern std::chrono::time_point<std::chrono::high_resolution_clock> GAME_START_TIME;

  // Initialize all systems for new scene
  void init_scene(int scene_id, bool is_new_game);

  // checks if next scene id is a menu
  void _check_if_menu();

  // Delete all data necessary for clear the scene
  void clear_scene();

  // Switches scene. Clears current scene data and initializes new one
  void switch_scene(int scene_id, bool is_new_game);

  // Initialize all in-game systems inside single game::init()
  void init();

  // Update game systems and buffer
  void update();

  // Cleanup after game loop is closed
  void drop();

}



#endif
