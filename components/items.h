#ifndef ITEMS_H
#define ITEMS_H


namespace items
{


  struct ItemData
  {
    int id;
    std::string type;
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

    JS_OBJ(id, type, joint_id, width_og, 
           height_og, items_frame_id, items_texture_id, texture_id,
           min_dmg, max_dmg, min_speed, max_speed);
  };

  struct GeneratedItemData
  {
    int type_id;
    int dmg;  
    int speed;
  };

  std::map<int, ItemData> ItemCatalog = {};
  std::map<int, GeneratedItemData> GeneratedItems = {};

  void load_item(std::string name)
  {
    // loads item data from /items directory
    ItemData ITD;
    std::string data_path = "./items/"+name+".json";
    std::string json_data = utils::read_text_file(data_path);
    JS::ParseContext context(json_data);
    context.parseTo(ITD);
    ItemCatalog.insert(std::pair<int, ItemData>{ITD.id, ITD}); 
  }

  void init()
  {
    std::vector<std::string> item_list = utils::list_files("./items/");
    for(int i=0; i < item_list.size(); i++)
    {
      load_item(item_list[i]);
    }
  }

  quads::Quad render_item_on_ground(int item_id, int x, int y)
  {
    struct ItemData ITD = ItemCatalog[item_id];
    int item_entity_id = ent::gen_entity_id();
    quads::Quad item_quad = ent::render_entity(ENTITY_TYPE_ID_ITEM, 
                                               false,
                                               ITD.items_texture_id,
                                               ITD.items_frame_id, 
                                               x, 
                                               y, 
                                               ITD.height_og, 
                                               ITD.width_og, 
                                               ENTITY_NOT_STATIC, 
                                               textures::FontTD,
                                               false,
                                               true,
                                               item_entity_id);
    return item_quad;
  }
}

#endif