#include <SDL2/SDL.h>

#include <map>
#include <string>

#ifndef MODULES_MOUSE_H
#define MODULES_MOUSE_H


namespace mouse2
{
  struct MouseData
  {
    int x, y;
    int last_click_timestamp;
    bool was_released;
  };

  // https://gist.github.com/mkawserm/9259513
  typedef void (*sig_ptr)(int);
  // Catalog of functions to be chosen based on the object_type_id. Takes object_id as argument
  extern std::map <int , sig_ptr> click;

  // Print out click information
  void print_mouse(SDL_MouseMotionEvent e, const char* name);

  // Handle mouse events
  void handle_mouse(SDL_MouseMotionEvent e, SDL_MouseButtonEvent b);

  // Find which quads are clicked on:
  void _find_clicked_quads(float click_x, float click_y);

  // Click logic for given menu button
  void _click_menu(int object_id);

  // Click logic for given entity
  void _click_entity(int object_id);

  // Click logic for given map
  void _click_map(int object_id);

  // Click logic for given text
  void _click_text(int object_id);

  // Initialize Mouse logic
  void init();

}



#endif