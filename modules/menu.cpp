
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

namespace menu
{
  std::map <int , sig_ptr> ClickButton = {};
  int CLICKED_BUTTON_ID;
  std::vector<std::string> saves;
  std::string NewGameName;
  std::map<int, menu::MenuData> menus;
  std::map<int, menu::ButtonData> menubuttons;
  std::map<int, menu::ButtonData> CurrentMenuButtons;
  std::vector<quads::QuadData> MenuQuads;
  const std::string _allowed_input = "abcdefghijklmnoprstuvwxyzABCDEFGHIJKLMNOPRSTUVWXYZ";
  
  void _read_button_data(std::string& name)
  {
    menu::ButtonData BD;
    std::string data_path = "./data/menubuttons/"+name+".json";
    std::string json_data = utils::read_text_file(data_path);
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
    menu::menubuttons.insert({BD.id, BD});
  };

  void _read_menu_data(std::string& name)
  {
    menu::MenuData MD;
    std::string data_path = "./data/menu/"+name+".json";
    std::string json_data = utils::read_text_file(data_path);
    JS::ParseContext context(json_data);
    context.parseTo(MD);
    menu::menus.insert({MD.id, MD});
  };

  void list_saves()
  {
    menu::saves = utils::list_json_files("saves");
  };

  void load(int scene_id)
  {
    // check if level_id exists in the menus
    if (menu::menus.find(scene_id) != menu::menus.end()) 
    {
      menu::CurrentMenuButtons.clear();
      // Get level button list
      std::vector<int> button_list =  menu::menus[scene_id].buttons;

      // Recreate button data for selected level
      for(int b=0; b<button_list.size(); b++)
      {
        
        // saves handling
        if(button_list[b] == 6)
        {
          // create button for each save
          for(int s=0; s<menu::saves.size(); s++)
          {
            menu::ButtonData bdd = menu::menubuttons[button_list[b]];
            bdd.id = bdd.id + (s+100);
            bdd.label = menu::saves[s];
            bdd.y = bdd.y + (bdd.h*(s+1)) + 5;
            bdd.label_id = fonts::add(bdd.label, bdd.x + 5, bdd.y + (bdd.h/1.5), CAMERA_STATIC, 1.0f, 1.0f, 1.0f, 1.0f);
            menu::CurrentMenuButtons[bdd.id] = bdd;
          }

        } else 
        {
          menu::ButtonData bdd = menu::menubuttons[button_list[b]];
          bdd.label_id = fonts::add(bdd.label, bdd.x + 5, bdd.y + (bdd.h/1.5), CAMERA_STATIC, 1.0f, 1.0f, 1.0f, 1.0f);
          if(bdd.id == MENU_BUTTON_NEWGAME_NAME)
          {
            fonts::NEW_GAME_LABEL_ID = bdd.label_id;
          }
          menu::CurrentMenuButtons[button_list[b]] = bdd;
        }
      }
    }
  };

  void render()
  {
    // Clear quads data
    menu::MenuQuads.clear();

    // Create new quads
    menu::MenuQuads = quads::make_quads(menu::CurrentMenuButtons, OBJECT_TYPE_MENU);
  }

  void _click_new_game()
  {
    game::switch_scene(NEWGAME_MENU_SCENE_ID, false);
  };

  void _click_load_game()
  {
    game::switch_scene(LOADGAME_MENU_SCENE_ID, false);
  };

  void _click_settings()
  {
    game::switch_scene(SETTINGS_MENU_SCENE_ID, false);
  };

  void _click_exit()
  {
    game::RUNNING = false;
  };

  void _click_newgame_name()
  {
    game::switch_scene(2, true);
  }

  void _click_loadgame_name()
  {
    game::switch_scene(2, true);
  }

  void _click_back()
  {
    game::switch_scene(MAIN_MENU_SCENE_ID, false);
  };



  void init()
  {
    menu::ClickButton[MENU_BUTTON_NEWGAME] = _click_new_game;
    menu::ClickButton[MENU_BUTTON_LOADGAME] = _click_load_game;
    menu::ClickButton[MENU_BUTTON_SETTINGS] = _click_settings;
    menu::ClickButton[MENU_BUTTON_EXIT] = _click_exit;
    menu::ClickButton[MENU_BUTTON_NEWGAME_NAME] = _click_newgame_name;
    menu::ClickButton[MENU_BUTTON_BACK] = _click_back;
    menu::ClickButton[MENU_BUTTON_LOADGAME_NAME] = _click_loadgame_name;

    std::vector<std::string> button_list = utils::list_json_files("data/menubuttons");
    for(int b=0; b < button_list.size(); b++)
    {
      menu::_read_button_data(button_list[b]);
    };

    std::vector<std::string> menu_list = utils::list_json_files("data/menu");
    for(int m=0; m < menu_list.size(); m++)
    {
      menu::_read_menu_data(menu_list[m]);
    };


    menu::list_saves();
  }

  void clear()
  {
    menu::NewGameName = "";
    menu::CurrentMenuButtons.clear();
    menu::MenuQuads.clear();
  }

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
      button_id = MENU_BUTTON_LOADGAME_NAME;
    }
    return button_id;
  }


} 
