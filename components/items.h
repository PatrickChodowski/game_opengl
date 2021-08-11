#ifndef ITEMS_H
#define ITEMS_H

// joints of the item to hero's body
#define JOINT_RIGHT_HAND 0
#define JOINT_LEFT_HAND 1
#define JOINT_UPPER_BODY 2
#define JOINT_LOWER_BODY 3
#define JOINT_LEGS 4
#define JOIN_HEAD 5 


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

    JS_OBJ(id, type, joint_id, width_og, 
           height_og, items_frame_id, items_texture_id, texture_id);
  };

  std::map<int, ItemData> ItemCatalog = {};
  std::vector<int> items_on_ground = {};

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
}

#endif