#include "../dependencies/json_struct.h"
#include "../dependencies/parallel_hashmap/phmap.h"

#include <string>
#include <vector>

#ifndef MODULES_MOBS_H
#define MODULES_MOBS_H

// Persistent data. Table of mobs and all operations on this table. Loaded on new level
namespace mobs
{

  struct MobData
  {
    int mob_id;
    int model_id;

    float min_dmg;
    float max_dmg;
    float min_speed;
    float max_speed;
    float min_hp;
    float max_hp;
    float min_def;
    float max_def;
    std::string name;

    JS_OBJ(mob_id, model_id, min_dmg, max_dmg, min_speed,
    max_speed, min_hp, max_hp, min_def, max_def, name);
  };

  extern phmap::flat_hash_map<int, MobData> mobs;

  // Reads mobs data
  void read_data(std::string name);

  // Initialize Mobs. Reads data for all mobs in data/mobs
  void init();

  // Clears all data
  void refresh();

  // Spawns mob entities according to the map - in the nests. Creates entities and adds to entity list
  void spawn_from_nest(int map_id);

  // Spawn single mob
  void spawn(int mob_id, float x, float y);

}


#endif 