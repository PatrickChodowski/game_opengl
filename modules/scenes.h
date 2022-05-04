
#include <iostream>
#include <string>
#include <vector>

#include "ecs.h"
#include "game.h"
#include "utils.h"

#include "../dictionary.h"
#include "../dependencies/json_struct.h"
#include "../dependencies/parallel_hashmap/phmap.h"


#ifndef MODULES_SCENES_H
#define MODULES_SCENES_H

namespace scenes
{
  // Data to load from scene file
  struct SceneData 
  {
    int id;
    int events_handler_id;
    int map_id;
    float hero_start_x, hero_start_y;
    std::string label;
    std::vector<std::string> entities;
    bool is_gp; // is it gameplay
    
    JS_OBJ(id, events_handler_id, map_id, hero_start_x, hero_start_y, label, is_gp, entities);
  };

  typedef void (*sig_ptr)();
  // Catalog of functions to be chosen based on the scene id. SceneID/Function
  extern phmap::flat_hash_map<int, sig_ptr> SceneLoader;

  // Collection of all scenes
  extern phmap::flat_hash_map<int, scenes::SceneData> scenes;

  // Initializes scene list with functions
  void init();

  // Reads scene data to struct
  void read_data(std::string &name);

  // Loads new scene to the screen
  void load(int scene_id, int load_scene_from);

  // Clear previous scene and load new one
  void switch_scene(int scene_id, int load_scene_from);


  // Scene functions
  // Scene ID: 1 New game menu
  void _load_scene_new_game_menu();

  // Scene ID: 2 Load game menu
  void _load_scene_load_game_menu();

  // Scene ID: 4 Dungeon level 1
  void _load_scene_dungeon_level_1();

  // Scene ID: 5 Dungeon level 2
  void _load_scene_dungeon_level_2();

}


#endif