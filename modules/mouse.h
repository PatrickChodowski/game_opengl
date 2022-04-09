#include <SDL2/SDL.h>

#include <string>

#ifndef MODULES_MOUSE_H
#define MODULES_MOUSE_H


namespace mouse
{
  struct MouseData
  {
    int x, y;
    int last_click_timestamp;
    bool was_released;
  };

  struct LastClickData
  {
    float world_x;
    float world_y;
    float window_x;
    float window_y;
  };

  struct ClickData
  {
    int object_id;
    int object_type_id;
    int mouse_button_id;
    int priority;
  };


  // https://gist.github.com/mkawserm/9259513
  typedef void (*sig_ptr)(int, int);
  // Catalog of functions to be chosen based on the object_type_id. Takes object_id as argument
  extern phmap::flat_hash_map<int, sig_ptr> click;
  extern phmap::flat_hash_map<int, int> ClickPriorities;
  extern LastClickData last_click;

  // Print out click information
  void print_mouse(SDL_MouseMotionEvent e, const char* name);

  // Handle mouse events
  void handle_mouse(SDL_MouseMotionEvent e, SDL_MouseButtonEvent b);

  // Find which quads are clicked on:
  void _find_clicked_quads(float click_x, float click_y, int mouse_button_id);

  // Click logic for given entity
  void _click_entity(int object_id, int mouse_button_id);

  // Initialize Mouse logic
  void init();

}



#endif