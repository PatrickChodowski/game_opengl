
#include <map>

#ifndef SCRIPTS_PY_H
#define SCRIPTS_PY_H


namespace scripts
{
  typedef void (*sig_ptr)();
  extern std::map<int,sig_ptr> Handler;
  extern std::map<std::string, float> args;

  // Initialize python handler
  void init();

  // Execute Python script
  void execute(std::string& script_name);

  // Drop Python interpreter (not sure about it xd)
  void drop();

  // API functions:

  // Function 0: spawn mob
  void _spawn_mob();

  // Function 1: drop mob
  void _drop_mob();

  // Function 2: interact_npc
  void _interact_npc();

  // Function 3: _spawn_item
  void _spawn_item();

  // Function 4: _drop_item
  void _drop_item();

  // Function 5: _start_animation
  void _start_animation();

  // Function 6: _init_travel
  void _init_travel();

  // Function 7: _print_entity
  void _print_entity();

  // Function 8: _print_models
  void _print_models();

  // Function 9: _print_quads
  void _print_quads();


}

#endif
