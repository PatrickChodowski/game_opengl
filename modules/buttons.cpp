
#include "buttons.h"

#include <map>
#include <string>
#include <vector>

#include "fonts.h"
#include "game.h"
#include "quads.h"
#include "utils.h"

#include "../dictionary.h"

namespace buttons
{
  std::map <int,sig_ptr> ButtonFunctions;
  std::vector<int> Index = {};
  std::map <int, buttons::ButtonData> buttons;
  std::vector<quads::QuadData> ButtonQuads;

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
  };

  int add(std::string text, float x, float y, float w, float h, int button_function_id)
  {
    buttons::ButtonData bdd;
    bdd.id = utils::generate_id(buttons::Index);
    bdd.x = x;
    bdd.y = y;
    bdd.w = w;
    bdd.h = h;
    bdd.is_clicked = false;
    bdd.r = 0.8;
    bdd.g = 0.5;
    bdd.b = 0.5;
    bdd.a = 1.0;
    bdd.button_function_id = button_function_id;
    bdd.camera_type = CAMERA_STATIC;

    bdd.label_id = fonts::add(text, 
                               x + 15, 
                               y + 55, 
                               CAMERA_STATIC, 
                               1.0f,
                               0.0f, 
                               0.0f, 
                               0.0f);

    buttons::buttons[bdd.id] = bdd;
    return bdd.id;
  };

  void render()
  {
    buttons::ButtonQuads.clear();
    buttons::ButtonQuads = quads::make_quads(buttons::buttons, OBJECT_TYPE_BUTTON);
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
    buttons::ButtonQuads.clear();
  }

  void _click_new_game(int placeholder)
  {
    game::switch_scene(SCENE_ID_NEW_GAME_MENU);
  };

  void _click_load_game(int placeholder)
  {
    game::switch_scene(SCENE_ID_LOAD_GAME_MENU);
  };

  void _click_settings(int placeholder)
  {
    game::switch_scene(SCENE_ID_SETTINGS_MENU);
  };

  void _click_exit(int placeholder)
  {
    game::RUNNING = false;
  };

  void _click_newgame_name(int placeholder)
  {
    game::switch_scene(SCENE_ID_NEW_GAME_MENU);
  }

  void _click_loadgame_name(int placeholder)
  {
    game::switch_scene(SCENE_ID_DUNGEON_LEVEL_1);
  }

  void _click_back(int placeholder)
  {
    game::switch_scene(SCENE_ID_MAIN_MENU);
  };

  // Rick click on entity, open entity menu
  void _button_travel(int object_id)
  {   
    // Button travel
    // how to implement that? lolz
    // x,y = mouse::_request_position();
    // travel::goto(x,y)

    // OR 
    // Create travel option but in idle state
    // Next click on the map will add x y for travel and change the idle state to active
  };



}
