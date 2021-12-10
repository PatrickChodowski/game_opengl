#include "../dependencies/json_struct.h"

#include <map>
#include <string>
#include <vector>



#ifndef MODULES_MOBS_H
#define MODULES_MOBS_H

// Persistent data. Table of mobs and all operations on this table. Loaded on new level
namespace mobs
{

  struct MobData
  {
    int id;
    int texture_id;
    int current_frame;
    int min_dmg;
    int max_dmg;
    int min_speed;
    int max_speed;
    int min_hp;
    int max_hp;
    int min_def;
    int max_def;

    float speed;
    float x, y;
    float w, h;

    std::string type;

    JS_OBJ(id, type, w, h, texture_id, min_dmg, max_dmg, min_speed,
    max_speed, min_hp, max_hp, min_def, max_def, speed);
  };

  extern std::map<int, MobData> mobs;
  extern std::vector<int> Index;
  extern std::map<int, MobData> spawnedmobs;

  // Reads mobs data
  void read_data(std::string name);

  // Initialize Mobs. Reads data for all mobs in data/mobs
  void init();

  // Clears all data
  void refresh();

  // clears Mobs information
  void clear();

  // Spawns mob entities according to the map - in the nests. Creates entities and adds to entity list
  void spawn_from_nest(int map_id);

}


#endif 