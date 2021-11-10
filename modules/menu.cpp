
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
  const std::string _allowed_input = "abcdefghijklmnoprstuvwxyzABCDEFGHIJKLMNOPRSTUVWXYZ";
  
  void _read_button_data(std::string& name)
  {
    menu2::ButtonData BD;
    std::string data_path = "./data/menubuttons/"+name+".json";
    std::string json_data = utils2::read_text_file(data_path);
    JS::ParseContext context(json_data);
    context.parseTo(BD);
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
