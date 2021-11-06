
#include <vector>
#include <map>
#include <string>
#include "quads.h"
#include "collisions.h"
#include "../dependencies/json_struct.h"

#ifndef MODULES_ITEMS_H
#define MODULES_ITEMS_H


// Persistent data. Table of tiles and all operations on this table. 

namespace items2
{
  struct ItemData
  {
    int id;
    int joint_id;
    int width_og;
    int height_og;
    int items_frame_id;
    int items_texture_id;
    int texture_id;
    int min_dmg;
    int max_dmg;
    int min_speed;
    int max_speed;
    int current_frame;

    float x, y;
    float w, h;
    std::string type;

    JS_OBJ(id, type, joint_id, width_og, 
           height_og, items_frame_id, items_texture_id, texture_id,
           min_dmg, max_dmg, min_speed, max_speed, type);
  };
  
  struct GeneratedItemData
  {
    int type_id;
    int dmg;  
    int speed;
  };

  extern std::map<int, ItemData> items;
  extern std::map<int, GeneratedItemData> GeneratedItems;

  // Reads item data from json file
  void read_data(std::string name);

  // Finds all item data files and reads them in
  void init();

  // Render item object (entity) on the ground
  void put_item_on_ground(int item_id, float x, float y);

}

#endif