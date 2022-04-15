#include <iostream>
#include <string>
#include <vector>

#include "collisions.h"
#include "ecs.h"
#include "hero.h"
#include "items.h"
#include "models.h"
#include "utils.h"
#include "quads.h"

#include "../dependencies/json_struct.h"
#include "../dictionary.h"
#include "../dependencies/parallel_hashmap/phmap.h"


namespace items
{
  phmap::flat_hash_map<int, items::ItemData> items = {};
  void read_data(std::string name)
  {
    ItemData ITD;
    std::string data_path = "./data/items/"+name+".json";
    std::string json_data = utils::read_text_file(data_path);
    JS::ParseContext context(json_data);
    context.parseTo(ITD);
    items::items.insert(std::pair<int, ItemData>{ITD.item_id, ITD}); 
    std::cout << " [ITEMS] Reading data for " << name << std::endl;
  };

  void init()
  {
    std::vector<std::string> item_list = utils::list_json_files("./data/items/");
    for(int i=0; i < item_list.size(); i++)
    {
      items::read_data(item_list[i]);
    }
    std::cout << "Items Initialized" << std::endl;
  };

  void refresh()
  {
    items::items.clear();
  }

  int generate_item(int item_id, float x, float y, int item_location_id)
  {
    std::cout << " [ITEMS] Generating entity for item ID " << item_id << std::endl;
    items::ItemData ITEM = items::items.at(item_id);
    ecs::TempEntityData e;
    e.name = ITEM.name;
    e.components = {0,1,2,3,8,10};
    e.entity_type_id = ENTITY_TYPE_ITEM;
    e.x = x;
    e.y = y;
    e.z = 0.29;
    e.w = ITEM.w;
    e.h = ITEM.h;
    e.r = 0.4;
    e.g = 0.4;
    e.b = 0.4;
    e.a = 1.0;
    e.model_id = ITEM.model_id;
    e.frame_id = 10100;
    e.side_id = ANIM_SIDE_FRONT;
    e.camera_type = CAMERA_DYNAMIC;
    e.item_id = ITEM.item_id;
    e.item_joint_id = ITEM.joint_id;

    // Create utils::random_from_range later
    e.item_dmg = ITEM.max_dmg;
    e.item_speed = ITEM.max_speed;
    e.item_location = item_location_id;

    e.radius = ITEM.radius;

    models::load(e.model_id);
    int item_entity_id = ecs::create_entity(&e);
    return item_entity_id;
  }

  void pickup(int entity_id, int item_entity_id)
  {
    if(ecs::items.count(item_entity_id)){
      if(ecs::items.at(item_entity_id).item_location == ITEM_LOCATION_GROUND){
        ecs::equipments.at(entity_id).equipment.push_back(item_entity_id);
        ecs::items.at(item_entity_id).item_location = ITEM_LOCATION_EQ;
        ecs::hide(item_entity_id);
        std::cout << " [ITEMS] Entity ID: "<< entity_id << " picked up item " << item_entity_id << std::endl;
      }
    }
  };

  void yeet(int entity_id, int item_entity_id, float x, float y)
  {
    int index = -1;
    std::vector<int> eq = ecs::equipments.at(entity_id).equipment;
    auto it = std::find(eq.begin(), eq.end(), item_entity_id);

    if (it != eq.end()){
      index = it - eq.begin();
    }

    if(index > -1){
      eq.erase(eq.begin() + index);
      ecs::show(item_entity_id);
      ecs::items.at(item_entity_id).item_location = ITEM_LOCATION_GROUND;
      ecs::set_position(item_entity_id, x, y);
      std::cout << " [ITEMS] Entity ID: "<< entity_id << " yeeted item " << item_entity_id << std::endl;
    }
  }

  void equip(int entity_id)
  {
    // if(items::GeneratedItems.count(entity_id) > 0 & items::EquippedItems.count(entity_id) > 0)
    // {
    //   items::EquippedItems[entity_id].x = hero::hero.hand_x;
    //   items::EquippedItems[entity_id].y = hero::hero.hand_y;
    //   //int old_entity_id = entity::create(items::EquippedItems[entity_id], ENTITY_TYPE_ITEM, CAMERA_DYNAMIC, entity_id);
    //   hero::hero.in_hand_entity_id = entity_id;
    // }
  };


}
