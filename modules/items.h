
#include <vector>
#include <string>
#include "quads.h"

#include "../dependencies/json_struct.h"
#include "../dependencies/parallel_hashmap/phmap.h"

#ifndef MODULES_ITEMS_H
#define MODULES_ITEMS_H


// Persistent data. Table of tiles and all operations on this table. 

namespace items
{

  // Non entity item attributes
  struct ItemData
  {
    int item_id;
    int model_id;
    int joint_id;
    int min_dmg;
    int max_dmg;
    int min_speed;
    int max_speed;
    float w;
    float h;
    std::string name;

    JS_OBJ(item_id, model_id, joint_id, min_dmg, max_dmg, min_speed, max_speed, w, h, name);

  };

  // Dictionary of all items
  extern phmap::flat_hash_map<int, ItemData> items;

  // Reads item data from json file
  void read_data(std::string name);

  // Finds all item data files and reads them in
  void init();

  // Create item entity
  int generate_item(int item_id, float x, float y, int item_location_id);

  // Renders in hand item
  void equip(int entity_id);

  // Pickup an item
  void pickup(int entity_id);

  // Drop item on the ground
  void yeet(int entity_id, float x, float y);

  // Clear all data
  void refresh();

}

#endif