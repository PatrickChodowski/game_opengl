#include <SDL2/SDL.h>
#include <string>

#ifndef MODULES_MOUSE_H
#define MODULES_MOUSE_H


namespace mouse2
{
  struct MouseData
  {
    int x, y;
    int last_clicp_timestamp;
    bool was_released;
  };

  // Print out click information
  void print_mouse(SDL_MouseMotionEvent e, std::string  name);

  // Handle mouse events
  void handle_mouse(SDL_MouseMotionEvent e, SDL_MouseButtonEvent b);

}



#endif