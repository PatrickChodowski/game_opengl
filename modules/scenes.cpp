
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "logger.h"
#include "maps.h"
#include "menu2.h"
#include "scenes.h"
#include "utils.h"

#include "../dictionary.h"
#include "../dependencies/json_struct.h"

namespace scenes
{ 
  // Current scene event handler
  int SCENE_ID = SCENE_ID_MAIN_MENU;
  int EVENT_HANDLER_ID;
  int MAP_ID;
  float HERO_START_X;
  float HERO_START_y;
  std::map<int, scenes::SceneData> scenes;


  void read_data(std::string& name)
  {
    scenes::SceneData SD;
    std::string data_path = "./data/scenes/"+name+".json";
    std::string json_data = utils::read_text_file(data_path);
    JS::ParseContext context(json_data);
    context.parseTo(SD);
    scenes::scenes.insert({SD.id, SD});
  };

  void init()
  {
    std::vector<std::string> scene_list = utils::list_json_files("data/scenes");
    for(int s=0; s < scene_list.size(); s++)
    {
      scenes::read_data(scene_list[s]);
    };
  }


  void load(int scene_id)
  {
    if(scenes::scenes.count(scene_id) > 0)
    {    
      scenes::SCENE_ID = scene_id;
      scenes::EVENT_HANDLER_ID = scenes::scenes[scene_id].events_handler_id;
      scenes::MAP_ID = scenes::scenes[scene_id].map_id;
      scenes::HERO_START_X = scenes::scenes[scene_id].hero_start_x;
      scenes::HERO_START_y = scenes::scenes[scene_id].hero_start_y;

      // Load maps
      maps::load(scenes::MAP_ID);

      // Load menu slots
      for(int s=0; s<scenes::scenes[scene_id].menu_slots.size(); s++)
      {
        int slot_id = scenes::scenes[scene_id].menu_slots[s];
        menu::currentmenuslots[slot_id] = menu::menuslots[slot_id];
      }
      // Load menus
      for(int t=0; t<scenes::scenes[scene_id].menu_types.size(); t++)
      {
        int menu_type_id = scenes::scenes[scene_id].menu_types[t];
        menu::add(menu_type_id);
      }
      
    }
  }



}


