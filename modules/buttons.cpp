
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
  std::map <int, buttons::GuiButtonData> guibuttons;
  std::vector<quads::QuadData> GuiButtonQuads;

  void init()
  {
    buttons::ButtonFunctions[GUI_BUTTON_NEWGAME] = buttons::_click_new_game;
    buttons::ButtonFunctions[GUI_BUTTON_LOADGAME] = buttons::_click_load_game;
    buttons::ButtonFunctions[GUI_BUTTON_SETTINGS] = buttons::_click_settings;
    buttons::ButtonFunctions[GUI_BUTTON_EXIT] = buttons::_click_exit;
    buttons::ButtonFunctions[GUI_BUTTON_NEWGAME_NAME] = buttons::_click_newgame_name;
    buttons::ButtonFunctions[GUI_BUTTON_BACK] = buttons::_click_back;
    buttons::ButtonFunctions[GUI_BUTTON_LOADGAME_NAME] = buttons::_click_loadgame_name;
    buttons::ButtonFunctions[GUI_BUTTON_TRAVEL] = buttons::_button_travel;
  };

  int add(std::string text, float x, float y, int button_function_id)
  {
    buttons::GuiButtonData bdd;
    bdd.id = utils::generate_id(buttons::Index);
    bdd.x = x;
    bdd.y = y;
    bdd.w = 100;
    bdd.h = 60;
    bdd.is_clicked = false;
    bdd.r = 0.8;
    bdd.g = 0.5;
    bdd.b = 0.5;
    bdd.a = 1.0;
    bdd.button_function_id = button_function_id;
    bdd.camera_type = CAMERA_STATIC;

    bdd.label_id = fonts::add(text, 
                               x + 15, 
                               y + 50, 
                               CAMERA_STATIC, 
                               0.7,
                               0.0f, 
                               0.0f, 
                               0.0f);

    buttons::guibuttons[bdd.id] = bdd;
    return bdd.id;
  };

  void render()
  {
    buttons::GuiButtonQuads.clear();
    buttons::GuiButtonQuads = quads::make_quads(buttons::guibuttons, OBJECT_TYPE_BUTTON);
  }


  void drop(int button_id)
  {
    fonts::drop(buttons::guibuttons[button_id].label_id);
    buttons::guibuttons.erase(button_id);
    utils::drop_id(buttons::Index, button_id);
  };

  void clear()
  {
    buttons::Index.clear();
    buttons::guibuttons.clear();
    buttons::GuiButtonQuads.clear();
  }

  void _click_new_game(int placeholder)
  {
    game::switch_scene(NEWGAME_MENU_SCENE_ID, false);
  };

  void _click_load_game(int placeholder)
  {
    game::switch_scene(LOADGAME_MENU_SCENE_ID, false);
  };

  void _click_settings(int placeholder)
  {
    game::switch_scene(SETTINGS_MENU_SCENE_ID, false);
  };

  void _click_exit(int placeholder)
  {
    game::RUNNING = false;
  };

  void _click_newgame_name(int placeholder)
  {
    game::switch_scene(2, true);
  }

  void _click_loadgame_name(int placeholder)
  {
    game::switch_scene(2, true);
  }

  void _click_back(int placeholder)
  {
    game::switch_scene(MAIN_MENU_SCENE_ID, false);
  };

  // Rick click on entity, open entity gui
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
