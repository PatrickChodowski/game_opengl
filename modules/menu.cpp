
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "fonts.h"
#include "game.h"
#include "menu.h"
#include "mouse.h"
#include "quads.h"
#include "utils.h"

#include "../dictionary.h"
#include "../dependencies/json_struct.h"

namespace menu2
{
  std::map <int , sig_ptr> ClickButton = {};
  std::vector<std::string> saves;
  std::string NewGameName;
  std::map<int, menu2::MenuData> menus;
  std::map<int, menu2::ButtonData> menubuttons;
  std::map<int, menu2::ButtonData> CurrentMenuButtons;
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
    BD.r = 0.5;
    BD.g = 0.5;
    BD.b = 0.5;
    BD.a = 0.5;
    BD.norm_x_start = 0;
    BD.norm_x_end = 0;
    BD.button_type_id = BD.id;
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

  void load(int scene_id)
  {
    // check if level_id exists in the menus
    if (menu2::menus.find(scene_id) != menu2::menus.end()) 
    {
      menu2::CurrentMenuButtons.clear();
      // Get level button list
      std::vector<int> button_list =  menu2::menus[scene_id].buttons;

      // Recreate button data for selected level
      for(int b=0; b<button_list.size(); b++)
      {
        menu2::ButtonData bdd = menu2::menubuttons[button_list[b]];
        bdd.label_id = fonts2::add(bdd.label, bdd.x + 5, bdd.y + (bdd.h/1.5), CAMERA_STATIC, 1.0);
        if(bdd.id == MENU_BUTTON_NEWGAME_NAME)
        {
          fonts2::NEW_GAME_LABEL_ID = bdd.label_id;
        }
        menu2::CurrentMenuButtons[button_list[b]] = bdd;
      }
    }
  };

  void render()
  {
    // Clear quads data
    quads2::clear_quads_data(menu2::MenuQuads);
    menu2::MenuQuads.clear();

    // Create new quads
    menu2::MenuQuads = quads2::make_quads(menu2::CurrentMenuButtons, OBJECT_TYPE_MENU);
  }

  void _click_new_game()
  {
    game2::switch_scene(NEWGAME_MENU_SCENE_ID, false);
  };

  void _click_load_game()
  {
    game2::switch_scene(LOADGAME_MENU_SCENE_ID, false);
  };

  void _click_settings()
  {
    game2::switch_scene(SETTINGS_MENU_SCENE_ID, false);
  };

  void _click_exit()
  {
    game2::RUNNING = false;
  };

  void _click_newgame_name()
  {
    game2::switch_scene(2, true);
  }

  void _click_back()
  {
    game2::switch_scene(MAIN_MENU_SCENE_ID, false);
  };



  void init()
  {
    menu2::ClickButton[MENU_BUTTON_NEWGAME] = _click_new_game;
    menu2::ClickButton[MENU_BUTTON_LOADGAME] = _click_load_game;
    menu2::ClickButton[MENU_BUTTON_SETTINGS] = _click_settings;
    menu2::ClickButton[MENU_BUTTON_EXIT] = _click_exit;
    menu2::ClickButton[MENU_BUTTON_NEWGAME_NAME] = _click_newgame_name;
    menu2::ClickButton[MENU_BUTTON_BACK] = _click_back;

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

  void clear()
  {
    menu2::NewGameName = "";
    menu2::CurrentMenuButtons.clear();
    quads2::clear_quads_data(menu2::MenuQuads);
    menu2::MenuQuads.clear();
  }

  bool _validate_input(std::string input)
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
