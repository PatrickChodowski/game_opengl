
#ifndef MODULES_GAME_H
#define MODULES_GAME_H


namespace game2
{
  // Main Loop Running condition
  extern bool RUNNING;
  extern int CURRENT_SHADER_ID;

  // Initialize all in-game systems inside single game::init()
  void init();

  // Update game systems and buffer
  void update();

}



#endif
