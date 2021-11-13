#include <map>
#include <vector>
#include <SDL2/SDL.h>
#include <string>

#ifndef MODULES_GAME_H
#define MODULES_GAME_H


namespace game2
{
  // Game running variables
  extern bool RUNNING;
  extern int CURRENT_SHADER_ID;
  extern float TILE_DIM;
  extern float WINDOW_VERTEX_WIDTH;
  extern float WINDOW_VERTEX_HEIGHT;
  extern float WINDOW_WIDTH;
  extern float WINDOW_HEIGHT;
  extern std::map<std::string, bool> GAME_STATE;
  extern std::vector<std::string> GAME_STATE_LIST;
  extern bool CHANGE_STATE_TRIGGER;
  extern const Uint8 *KEYBOARD;

  // Initialize game states set
  void _init_game_states();

  // Sets game state
  void set_state();

  // Gets game state
  std::string get_state();

  // Initialize all in-game systems inside single game::init()
  void init();

  // Update game systems and buffer
  void update();

  // Cleanup after game loop is closed
  void drop();

}



#endif
