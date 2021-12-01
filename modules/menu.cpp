
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "buttons.h"
#include "fonts.h"
#include "menu.h"
#include "mouse.h"
#include "quads.h"
#include "utils.h"

#include "../dictionary.h"
#include "../dependencies/json_struct.h"

namespace menu
{

  std::vector<int> Index;
  std::map<int, menu::MenuSlotData> menuslots = {};
  std::map<int, menu::MenuSlotData> currentmenuslots = {}; 

  std::map<int, menu::MenuData> menus;
  std::map<int, menu::MenuData> currentmenus;
  std::vector<quads::QuadData> MenuQuads;
  
  std::vector<std::string> saves;
  std::string NewGameName;
  const std::string _allowed_input = "abcdefghijklmnoprstuvwxyzABCDEFGHIJKLMNOPRSTUVWXYZ";
  

  void read_menu_data(std::string& name)
  {
    menu::MenuData MD;
    std::string data_path = "./data/menu/"+name+".json";
    std::string json_data = utils::read_text_file(data_path);
    JS::ParseContext context(json_data);
    context.parseTo(MD);

    MD.camera_type = CAMERA_STATIC;
    MD.norm_x_start = 0.0f;
    MD.norm_x_end = 1.0f;
    MD.menu_slot_id = -1;
    MD.z = 0;

    menu::menus.insert({MD.menu_type_id, MD});
  };

  void read_menu_slot_data(std::string& name)
  {
    menu::MenuSlotData MD;
    std::string data_path = "./data/menuslots/"+name+".json";
    std::string json_data = utils::read_text_file(data_path);
    JS::ParseContext context(json_data);
    context.parseTo(MD);
    menu::menuslots.insert({MD.id, MD});
  };

  void init()
  {
    std::vector<std::string> menu_list = utils::list_json_files("data/menu");
    for(int m=0; m < menu_list.size(); m++)
    {
      menu::read_menu_data(menu_list[m]);
    };

    std::vector<std::string> menu_slot_list = utils::list_json_files("data/menuslots");
    for(int m=0; m < menu_slot_list.size(); m++)
    {
      menu::read_menu_slot_data(menu_slot_list[m]);
    };

    menu::_list_saves();
  }

  int add(int menu_type_id)
  {
    int menu_id = utils::generate_id(menu::Index);
    currentmenus[menu_id] = menus[menu_type_id];
    currentmenus[menu_id].id = menu_id;

    // Add button objects to the menu
    for(int b=0; b<currentmenus[menu_id].button_data.size(); b++)
    { 
      int button_id;
      // Exception for saves
      if(currentmenus[menu_id].button_data[b].button_function_id != BUTTON_LOADGAME_NAME)
      {
        button_id = buttons::add(currentmenus[menu_id].button_data[b].label,
                                    currentmenus[menu_id].button_data[b].x,
                                    currentmenus[menu_id].button_data[b].y,
                                    currentmenus[menu_id].button_data[b].w,
                                    currentmenus[menu_id].button_data[b].h,
                                    currentmenus[menu_id].button_data[b].button_function_id);
        currentmenus[menu_id].button_ids.push_back(button_id);  
      } else 
      {
        // Saves buttons
        for(int m=0; m<menu::saves.size(); m++)
        {
          button_id = buttons::add(menu::saves[m],
                                   currentmenus[menu_id].button_data[b].x,
                                   (currentmenus[menu_id].button_data[b].y + (m*70)),
                                   currentmenus[menu_id].button_data[b].w,
                                   currentmenus[menu_id].button_data[b].h,
                                   currentmenus[menu_id].button_data[b].button_function_id);
          currentmenus[menu_id].button_ids.push_back(button_id); 
        }
      }
    }

    // Add Labels to the menu
    for(int l=0; l<currentmenus[menu_id].label_data.size(); l++)
    {
      int label_id = fonts::add(currentmenus[menu_id].label_data[l].label,
                                currentmenus[menu_id].label_data[l].x,
                                currentmenus[menu_id].label_data[l].y,
                                CAMERA_STATIC,
                                1.0f,
                                currentmenus[menu_id].label_data[l].r,
                                currentmenus[menu_id].label_data[l].g,
                                currentmenus[menu_id].label_data[l].b);

      currentmenus[menu_id].label_ids.push_back(label_id);
    }
    return menu_id;
  }

  void render()
  {
    //std::cout << "current menus size:" << menu::currentmenus.size() << std::endl;
    menu::MenuQuads.clear();
    menu::MenuQuads = quads::make_quads(menu::currentmenus, OBJECT_TYPE_MENU);
  }

  void drop(int menu_id)
  {
    // Delete labels assigned to this menu
    for(int b=0; b<menu::currentmenus[menu_id].button_ids.size(); b++)
    {
      buttons::drop(menu::currentmenus[menu_id].button_ids[b]);
    }

    for(int l=0; l<menu::currentmenus[menu_id].label_ids.size(); l++)
    {
      fonts::drop(menu::currentmenus[menu_id].label_ids[l]);
    }

    menu::_free_slot(menu_id);
    menu::currentmenus.erase(menu_id);
    utils::drop_id(menu::Index, menu_id);
  }

  void clear()
  {
    for(int i = 0; i < menu::Index.size(); i++)
    {
      menu::drop(menu::Index[i]);
    }
    menu::Index.clear();
    menu::MenuQuads.clear();
    menu::currentmenus.clear();
    menu::currentmenuslots.clear();
    menu::NewGameName = "";
  };

  void _list_saves()
  {
    menu::saves = utils::list_json_files("saves");
  };

  bool _validate_input(std::string input)
  {
    bool good = false;
    if (menu::_allowed_input.find(input) != std::string::npos) 
    {
      good = true;
    }
    return good;
  };

  bool _validate_name()
  {
    bool good = false;
    // check if doesnt exist in saves
    // check if has more than 0 characters

    if(menu::NewGameName.size() > 0)
    {
      for(int s = 0; s<menu::saves.size(); s++)
      {
        if(menu::NewGameName == saves[s])
        {
          return good;
        }
      }
      good = true;
    }
    return good;
  };

  int _check_if_load_game(int button_id)
  { 
    // Over 100 will be a loaded save
    if(button_id > 100)
    {
      button_id = BUTTON_LOADGAME_NAME;
    }
    return button_id;
  }


  int _get_free_slot()
  {
    int min_slot_id = -1;
    for (auto const& [k, v] : menu::menuslots)
    {
      if(v.free)
      {
        min_slot_id = k;
        break;
      }
    }
    return min_slot_id;
  }

  void _free_slot(int menu_id)
  { 
    for (auto & [k, v] : menu::menuslots)
    {
      if(v.menu_id == menu_id)
      {
        v.free = true;
        v.menu_id = -1;
      }
    }
  }
}

