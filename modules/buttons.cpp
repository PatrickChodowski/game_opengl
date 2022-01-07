
#include "buttons.h"

#include <iostream>
#include <string>
#include <vector>

#include "fonts.h"
#include "game.h"
#include "hero.h"
#include "menu.h"
#include "quads.h"
#include "saves.h"
#include "travel.h"
#include "utils.h"

#include "../dependencies/parallel_hashmap/phmap.h"
#include "../dictionary.h"

namespace buttons
{
  phmap::flat_hash_map <int,sig_ptr> ButtonFunctions;
  std::vector<int> Index = {};
  phmap::flat_hash_map <int, buttons::ButtonData> buttons;

  void init()
  {
    buttons::ButtonFunctions[BUTTON_NEWGAME] = buttons::_click_new_game;
    buttons::ButtonFunctions[BUTTON_LOADGAME] = buttons::_click_load_game;
    buttons::ButtonFunctions[BUTTON_SETTINGS] = buttons::_click_settings;
    buttons::ButtonFunctions[BUTTON_EXIT] = buttons::_click_exit;
    buttons::ButtonFunctions[BUTTON_NEWGAME_NAME] = buttons::_click_newgame_name;
    buttons::ButtonFunctions[BUTTON_BACK] = buttons::_click_back;
    buttons::ButtonFunctions[BUTTON_LOADGAME_NAME] = buttons::_click_loadgame_name;
    buttons::ButtonFunctions[BUTTON_TRAVEL] = buttons::_button_travel;

    std::cout << "Buttons Initialized" << std::endl;
  };

  int add(std::string text, float x, float y, float w, float h, int button_function_id, int menu_id)
  {
    buttons::ButtonData bdd;
    bdd.id = utils::generate_id(buttons::Index);
    bdd.pos.x = x;
    bdd.pos.y = y;
    bdd.dims.w = w;
    bdd.dims.h = h;
    bdd.is_clicked = false;
    bdd.color.r = 0.8;
    bdd.color.g = 0.5;
    bdd.color.b = 0.5;
    bdd.color.a = 1.0;
    bdd.button_function_id = button_function_id;
    bdd.camera_type = CAMERA_STATIC;
    bdd.menu_id = menu_id;

    bdd.label_id = fonts::add(text, 
                               x + 15, 
                               y + 55, 
                               CAMERA_STATIC, 
                               1.0f,
                               0.0f, 
                               0.0f, 
                               0.0f);

    // not so nice exception for new game name button
    if(button_function_id == BUTTON_NEWGAME_NAME)
    {
      fonts::NEW_GAME_LABEL_ID = bdd.label_id;
    }

    buttons::buttons[bdd.id] = bdd;
    return bdd.id;
  };

  void render()
  {
    quads::add_quads(buttons::buttons, OBJECT_TYPE_BUTTON);
  }


  void drop(int button_id)
  {
    fonts::drop(buttons::buttons[button_id].label_id);
    buttons::buttons.erase(button_id);
    utils::drop_id(buttons::Index, button_id);
  };

  void clear()
  {
    buttons::Index.clear();
    buttons::buttons.clear();
  }

  void _click_new_game(int placeholder)
  {
    game::switch_scene(SCENE_ID_NEW_GAME_MENU, false);
  };

  void _click_load_game(int placeholder)
  {
    game::switch_scene(SCENE_ID_LOAD_GAME_MENU, false);
  };

  void _click_settings(int placeholder)
  {
    game::switch_scene(SCENE_ID_SETTINGS_MENU, false);
  };

  void _click_exit(int placeholder)
  {
    game::RUNNING = false;
  };

  void _click_newgame_name(int placeholder)
  {
    if(menu::_validate_name())
    {
      game::switch_scene(SCENE_ID_DUNGEON_LEVEL_2, SCENE_LOAD_FROM_NEW);  
    }
  }

  void _click_loadgame_name(int button_id)
  {
    menu::LoadGameName = menu::saves_buttons_map[button_id];
    game::switch_scene(SCENE_ID_DUNGEON_LEVEL_2, SCENE_LOAD_FROM_LOAD);  
  }

  void _click_back(int placeholder)
  {
    game::switch_scene(SCENE_ID_MAIN_MENU, false);
  };

  void _button_travel(int button_id)
  {   
    int entity_id = menu::currentmenus[buttons::buttons[button_id].menu_id].assigned_entity_id;
    std::cout << "clicked on travel button, entity id:  " << entity_id << std::endl;
    travel::reset_last_click();
    travel::init_travel(entity_id);
  };

}
