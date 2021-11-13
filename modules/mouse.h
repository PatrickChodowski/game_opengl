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
  void print_mouse(SDL_MouseMotionEvent e, const char* name);

  // Handle mouse events
  void handle_mouse(SDL_MouseMotionEvent e, SDL_MouseButtonEvent b);

  // Find which quads are clicked on:
  void _find_clicked_quads(float click_x, float click_y);

}



#endif