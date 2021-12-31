#include <iostream>
#include <string>
#include <vector>

#include "collisions.h"
#include "entity.h"
#include "hero.h"
#include "items.h"
#include "utils.h"
#include "quads.h"

#include "../dependencies/json_struct.h"
#include "../dictionary.h"
#include "../dependencies/parallel_hashmap/phmap.h"


namespace items
{
  phmap::flat_hash_map<int, ItemData> items = {};
  phmap::flat_hash_map<int, GeneratedItemData> GeneratedItems = {};
  phmap::flat_hash_map<int, GeneratedItemData> ItemsOnGround = {};
  phmap::flat_hash_map<int, GeneratedItemData> EquippedItems = {};

  void read_data(std::string name)
  {
    ItemData ITD;
    std::string data_path = "./data/items/"+name+".json";
    std::string json_data = utils::read_text_file(data_path);
    JS::ParseContext context(json_data);
    context.parseTo(ITD);
    items::items.insert(std::pair<int, ItemData>{ITD.id, ITD}); 
  };

  void init()
  {
    std::vector<std::string> item_list = utils::list_json_files("./data/items/");
    for(int i=0; i < item_list.size(); i++)
    {
      items::read_data(item_list[i]);
    }
  };

  void refresh()
  {
    items::items.clear();
  }

  void clear()
  {
    items::GeneratedItems.clear();
    items::ItemsOnGround.clear();
    items::EquippedItems.clear();
  }

  void drop(int entity_id)
  {
    if(items::GeneratedItems.count(entity_id) > 0)
    {
      items::GeneratedItems.erase(entity_id);
      entity::drop(entity_id);
    }

    if(items::ItemsOnGround.count(entity_id) > 0)
    {
      items::ItemsOnGround.erase(entity_id);
    };

    if(items::EquippedItems.count(entity_id) > 0)
    {
      items::EquippedItems.erase(entity_id);
    };
  }

  void pickup(int entity_id)
  {
    if(items::GeneratedItems.count(entity_id) > 0 & items::ItemsOnGround.count(entity_id) > 0)
    {
      items::ItemsOnGround.erase(entity_id);
      items::EquippedItems[entity_id] = items::GeneratedItems[entity_id]; 
      hero::hero.equipped_items.push_back(entity_id);
      entity::hide(entity_id);
      std::cout << "Hero picked up item " << entity_id << std::endl;
    }
  }

  void yeet(int entity_id, float x, float y)
  {
    if(items::GeneratedItems.count(entity_id) > 0 & items::EquippedItems.count(entity_id) > 0)
    {
      items::EquippedItems.erase(entity_id);
      items::ItemsOnGround[entity_id] = items::GeneratedItems[entity_id]; 
      items::ItemsOnGround[entity_id].x = x;
      items::ItemsOnGround[entity_id].y = y;
      int old_entity_id = entity::create(items::ItemsOnGround[entity_id], ENTITY_TYPE_ITEM, CAMERA_DYNAMIC, entity_id);
      std::cout << "Hero yeeted item " << entity_id << std::endl;
    }
  }

  void spawn(int item_id, float x, float y)
  {
    items::GeneratedItemData tdd;

    tdd.x = x;
    tdd.y = y;
    tdd.w = items::items[item_id].width_og;
    tdd.h = items::items[item_id].height_og;
    tdd.current_frame = items::items[item_id].items_frame_id;
    tdd.texture_id = items::items[item_id].items_texture_id;
    tdd.type = items::items[item_id].type;

    // logic for items to be stored in different table? Same as alive mobs
    int entity_id = entity::create(tdd, ENTITY_TYPE_ITEM, CAMERA_DYNAMIC);
    items::GeneratedItems[entity_id] = tdd;
    items::ItemsOnGround[entity_id] = tdd;
  }

  std::vector<std::string> info(int entity_id)
  {
    std::vector<std::string> infos = {};
    infos.push_back(items::GeneratedItems[entity_id].type);
    return infos;
  }

}
