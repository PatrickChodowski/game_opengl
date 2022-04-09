
#include "buttons.h"

#include <iostream>
#include <string>
#include <vector>

#include "fonts.h"
#include "game.h"
#include "hero.h"
#include "quads.h"
#include "saves.h"
#include "travel.h"
#include "utils.h"

#include "../dependencies/parallel_hashmap/phmap.h"
#include "../dictionary.h"

namespace buttons
{
  phmap::flat_hash_map <int,sig_ptr> ButtonFunctions;

  void init()
  {
    buttons::ButtonFunctions[BUTTON_NEWGAME] = buttons::_click_new_game;
    buttons::ButtonFunctions[BUTTON_LOADGAME] = buttons::_click_load_game;
    buttons::ButtonFunctions[BUTTON_SETTINGS] = buttons::_click_settings;
    buttons::ButtonFunctions[BUTTON_EXIT] = buttons::_click_exit;
    buttons::ButtonFunctions[BUTTON_NEWGAME_NAME] = buttons::_click_newgame_name;
    buttons::ButtonFunctions[BUTTON_BACK] = buttons::_click_back;
    buttons::ButtonFunctions[BUTTON_LOADGAME_NAME] = buttons::_click_loadgame_name;

    std::cout << "Buttons Initialized" << std::endl;
  };

  // not so nice exception for new game name button
  // if(button_function_id == BUTTON_NEWGAME_NAME)
  // {
  //   fonts::NEW_GAME_LABEL_ID = bdd.label_id;
  // }

  void _click_new_game(int placeholder)
  {
    game::switch_scene(SCENE_ID_NEW_GAME_MENU, false);
  };

  void _click_load_game(int placeholder)
  {
    //menu::_list_saves();
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
    if(saves::_validate_name())
    {
      game::switch_scene(SCENE_ID_DUNGEON_LEVEL_1, SCENE_LOAD_FROM_NEW);
    }
  }

  void _click_loadgame_name(int button_id)
  {
    //menu::LoadGameName = menu::saves_buttons_map[button_id];
    //saves::SaveData SD = saves::read_save_data(menu::LoadGameName);
    //game::switch_scene(SD.scene_id, SCENE_LOAD_FROM_LOAD);  
  }

  void _click_back(int placeholder)
  {
    game::switch_scene(SCENE_ID_MAIN_MENU, false);
  };

}
