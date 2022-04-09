
#include <iostream>
#include <string>
#include <vector>

#include "ecs.h"
#include "game.h"
#include "utils.h"

#include "../dictionary.h"
#include "../dependencies/parallel_hashmap/phmap.h"


#ifndef MODULES_SCENES_H
#define MODULES_SCENES_H

namespace scenes
{
  typedef void (*sig_ptr)();
  // Catalog of functions to be chosen based on the scene id. SceneID/Function
  extern phmap::flat_hash_map <int, sig_ptr> SceneLoader;

  // Initializes scene list with functions
  void init();

  // functions

  // Scene ID: 1 New game menu
  void _load_scene_new_game_menu();

  // Scene ID: 2 Load game menu
  void _load_scene_load_game_menu();




}


#endif