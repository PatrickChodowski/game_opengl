
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

  // Initialize all in-game systems inside single game::init()
  void init();

  // Update game systems and buffer
  void update();

  // Cleanup after game loop is closed
  void drop();

}



#endif
