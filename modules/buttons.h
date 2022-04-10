
#include <string>
#include <vector>

#include "quads.h"
#include "utils.h"
#include "../dependencies/parallel_hashmap/phmap.h"

#ifndef MODULES_BUTTONS_H
#define MODULES_BUTTONS_H

// In-game buttons implementations.
namespace buttons
{

  typedef void (*sig_ptr)(int);
  // Catalog of functions to be chosen based on the button_function_id
  extern phmap::flat_hash_map <int , sig_ptr> ButtonFunctions;

  // Initialize Buttons logic and data
  void init();

  // functions
  // Logic of what happens after clicking on new game
  void _click_new_game(int placeholder);

  // Logic of what happens after clicking on load game 
  void _click_load_game(int placeholder);

  // Logic of what happens after clicking on settings
  void _click_settings(int placeholder);

  // Logic of what happens after clicking on exit
  void _click_exit(int placeholder);

  // Logic of what happens after clicking on newgame_name
  void _click_newgame_name(int placeholder);

  // Logic of what happens after clicking on newgame_name
  void _click_loadgame_name(int button_entity_id);

  // Logic of what happens after clicking on back to main menu button
  void _click_back(int placeholder);

}

#endif
