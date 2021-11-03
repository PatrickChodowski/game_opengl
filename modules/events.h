#include <SDL2/SDL.h>
#include "../dictionary.h"

#ifndef MODULES_EVENTS_H
#define MODULES_EVENTS_H

namespace events2
{
  // Function handling input keys during gameplay
  void _handle_game_on_controls(SDL_Keycode key);

  // Function handling new game input keys
  void _handle_new_game_name_input(SDL_Keycode key);

  // Function handling load game input keys
  void _handle_load_game_menu_input(SDL_Keycode key);

  // Handle window inputs runtime
  void handle_events(SDL_Event event);

}


#endif
