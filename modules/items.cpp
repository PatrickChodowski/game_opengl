#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "collisions.h"
#include "entity.h"
#include "items.h"
#include "utils.h"
#include "quads.h"

#include "../dependencies/json_struct.h"
#include "../dictionary.h"


namespace items
{
  std::map<int, ItemData> items = {};
  std::map<int, GeneratedItemData> GeneratedItems = {};

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

  void put_item_on_ground(int item_id, float x, float y)
  {
    items::ItemData tdd = items::items[item_id];
    tdd.x = x;
    tdd.y = y;
    tdd.w = tdd.width_og;
    tdd.h = tdd.height_og;
    tdd.current_frame = tdd.items_frame_id;
    tdd.texture_id = items::items[item_id].items_texture_id;

    // logic for items to be stored in different table? Same as alive mobs
    int entity_id = entity::create(tdd, ENTITY_TYPE_ITEM, CAMERA_DYNAMIC);
  }

}
