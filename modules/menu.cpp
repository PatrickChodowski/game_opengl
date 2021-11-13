
#include <map>
#include <string>
#include <vector>
#include "quads.h"
#include "menu.h"
#include "mouse.h"
#include "utils.h"
#include "../dictionary.h"
#include "../dependencies/json_struct.h"

namespace menu2
{
  std::vector<std::string> saves;
  std::string NewGameName;
  std::map<int, menu2::MenuData> menus;
  std::map<int, menu2::ButtonData> menubuttons;
  std::vector<quads2::QuadData> MenuQuads;
  const std::string _allowed_input = "abcdefghijklmnoprstuvwxyzABCDEFGHIJKLMNOPRSTUVWXYZ";
  
  void _read_button_data(std::string& name)
  {
    menu2::ButtonData BD;
    std::string data_path = "./data/menubuttons/"+name+".json";
    std::string json_data = utils2::read_text_file(data_path);
    JS::ParseContext context(json_data);
    context.parseTo(BD);
    BD.camera_type = CAMERA_STATIC;
    BD.frame_id = 0;
    BD.is_clicked = 0;
    menu2::menubuttons.insert({BD.id, BD});
  };

  void _read_menu_data(std::string& name)
  {
    menu2::MenuData MD;
    std::string data_path = "./data/menu/"+name+".json";
    std::string json_data = utils2::read_text_file(data_path);
    JS::ParseContext context(json_data);
    context.parseTo(MD);
    menu2::menus.insert({MD.id, MD});
  };

  void list_saves()
  {
    menu2::saves = utils2::list_json_files("saves");
  };

  void render(int level_id)
  {
    // check if level_id exists in the menus
    if (menu2::menus.find(level_id) != menu2::menus.end()) 
    {
      // Clear quds data
      quads2::clear_quads_data(menu2::MenuQuads);
      menu2::MenuQuads.clear();

      // Get level button list
      std::map<int, ButtonData> level_buttons;
      std::vector<int> button_list =  menu2::menus[level_id].buttons;

      // Recreate button data for selected level
      for(int b=0; b<button_list.size(); b++)
      {
        level_buttons[button_list[b]] = menu2::menubuttons[button_list[b]];
      }

      // Should only render levels button:
      menu2::MenuQuads = quads2::make_quads(level_buttons, OBJECT_TYPE_MENU);
    }
  };

  void init()
  {
    std::vector<std::string> button_list = utils2::list_json_files("data/menubuttons");
    for(int b=0; b < button_list.size(); b++)
    {
      menu2::_read_button_data(button_list[b]);
    };

    std::vector<std::string> menu_list = utils2::list_json_files("data/menu");
    for(int m=0; m < menu_list.size(); m++)
    {
      menu2::_read_menu_data(menu_list[m]);
    };


    menu2::list_saves();
  }

  bool _validate_input(std::string& input)
  {
    bool good = false;
    if (menu2::_allowed_input.find(input) != std::string::npos) 
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

    if(menu2::NewGameName.size() > 0)
    {
      for(int s = 0; s<menu2::saves.size(); s++)
      {
        if(menu2::NewGameName == saves[s])
        {
          return good;
        }
      }
      good = true;
    }
    return good;
  };


} 
