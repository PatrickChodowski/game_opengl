
#ifndef SCRIPTS_PY_H
#define SCRIPTS_PY_H


namespace scripts
{
  typedef void (*sig_ptr)();
  extern std::map <int,sig_ptr> Handler;
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


}

#endif
