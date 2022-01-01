
#include <vector>
#include <string>
#include "quads.h"
#include "collisions.h"
#include "../dependencies/json_struct.h"
#include "../dependencies/parallel_hashmap/phmap.h"

#ifndef MODULES_ITEMS_H
#define MODULES_ITEMS_H


// Persistent data. Table of tiles and all operations on this table. 

namespace items
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
  
    float speed;
    float x, y;
    float w, h;
    std::string type;

    JS_OBJ(id, type, joint_id, width_og, 
           height_og, items_frame_id, items_texture_id, texture_id,
           min_dmg, max_dmg, min_speed, max_speed, type);
  };
  
  struct GeneratedItemData
  {
    int item_id;
    int entity_id;
    int type_id;
    int dmg;  
    int speed;

    int texture_id;
    int current_frame;

    float x,y;
    float w,h;

    std::string type;

  };

  extern phmap::flat_hash_map<int, ItemData> items;
  // Generated items = Items On Ground + Equipped Items
  extern phmap::flat_hash_map<int, GeneratedItemData> GeneratedItems;
  extern phmap::flat_hash_map<int, GeneratedItemData> ItemsOnGround;
  extern phmap::flat_hash_map<int, GeneratedItemData> EquippedItems;

  // Reads item data from json file
  void read_data(std::string name);

  // Finds all item data files and reads them in
  void init();

  // Render item object (entity) on the ground
  void spawn(int item_id, float x, float y);

  // Renders in hand item
  void put_in_hand(int entity_id);

  // Drop item entity 
  void drop(int entity_id);

  // Pickup an item
  void pickup(int entity_id);

  // Drop item on the ground
  void yeet(int entity_id, float x, float y);

  // Clear temporary data
  void clear();

  // Clear all data
  void refresh();

  // Returns vector of strings with item information
  std::vector<std::string> info(int entity_id);


}

#endif