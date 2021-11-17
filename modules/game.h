#include <map>
#include <vector>
#include <SDL2/SDL.h>
#include <string>

#ifndef MODULES_GAME_H
#define MODULES_GAME_H


namespace game2
{
  // Game running variables
  extern bool PAUSE;
  extern bool RUNNING;
  extern int CURRENT_SHADER_ID;
  extern int LEVEL_ID;
  extern float TILE_DIM;
  extern float WINDOW_VERTEX_WIDTH;
  extern float WINDOW_VERTEX_HEIGHT;
  extern float WINDOW_WIDTH;
  extern float WINDOW_HEIGHT;
  extern const Uint8 *KEYBOARD;

  // Initialize all systems for new level (new scene)
  void init_level(int level_id, bool is_new_game);

  // Delete all data necessary for clear the level (scene)
  void clear_level();

  // Switches level. Clears current level data and initializes new one
  void switch_level(int level_id);

  // Initialize all in-game systems inside single game::init()
  void init();

  // Update game systems and buffer
  void update();

  // Cleanup after game loop is closed
  void drop();

}



#endif
