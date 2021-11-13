#include <SDL2/SDL.h>

#ifndef MODULES_EVENTS_H
#define MODULES_EVENTS_H

namespace events2
{
  // Check if current level is a menu
  int _check_if_menu(int level_id);

  // Function handling input keys during gameplay
  void _handle_game_on_controls(SDL_Keycode key);

  // Function handling new game input keys
  void _handle_new_game_name_input(SDL_Keycode key);

  // Function handling load game input keys
  void _handle_load_game_menu_input(SDL_Keycode key);

  // Keyboard scan for camera movements:
  void scan_for_camera_move();

  // Handle window inputs runtime
  void handle_events(SDL_Event event);

}


#endif
