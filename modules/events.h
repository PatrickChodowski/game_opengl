
#include <SDL2/SDL.h>
#include "../dependencies/parallel_hashmap/phmap.h"

#ifndef MODULES_EVENTS_H
#define MODULES_EVENTS_H

namespace events
{
  typedef void (*sig_ptr)(SDL_Event event);
  // Catalog of functions to be chosen based on the scene id
  extern phmap::flat_hash_map <int , sig_ptr> EventsHandler;

  // Intializies objects in events 
  void init();

  // Function handling input keys in any menu
  void _handle_menu(SDL_Event event);

  // Function handling input keys during gameplay
  void _handle_in_game(SDL_Event event);

  // Function handling new game input keys
  void _handle_new_game(SDL_Event event);

  // Function handling load game input keys
  void _handle_load_game(SDL_Event event);

  // Keyboard scan for camera movements:
  void _scan_for_camera_move();

  // Handle window inputs runtime
  void handle_events(SDL_Event event);

}


#endif
