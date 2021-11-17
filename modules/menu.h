#include <vector>
#include <map>
#include <string>
#include "quads.h"
#include "mouse.h"
#include "../dependencies/json_struct.h"

#ifndef MODULES_MENU_H
#define MODULES_MENU_H

// Persistent data. Table of current menu button and all operations on this table. Level -> 99
namespace menu2
{

  struct ButtonData
  {
    int id;
    int texture_id;
    int frame_id;

    float x,y;
    float w,h;
    float r, g, b, a;

    float camera_type;
    float norm_x_start, norm_x_end;

    std::string label;
    std::string name;

    bool is_clicked;
    bool temp_label;

    JS_OBJ(id, x, y, w, h, r, g, b, a, label, name, temp_label);

  };

  struct MenuData
  {
    int id;
    std::vector<int> buttons;
    JS_OBJ(id, buttons);
  };

  typedef void (*sig_ptr)();
  // Catalog of functions to be chosen based on the button id
  extern std::map <int , sig_ptr> ClickButton;

  extern std::vector<std::string> saves;

  // Map of LevelID (100,101,102 and buttons: (std::vector<int> buttons))
  extern std::map<int, menu2::MenuData> menus;
  extern std::map<int, menu2::ButtonData> menubuttons;
  extern std::map<int, menu2::ButtonData> CurrentMenuButtons;
  extern std::vector<quads2::QuadData> MenuQuads;
  extern std::string NewGameName;

  // Initialize different menu data and load into menu catalog (menus)
  void init();

  // Reads in menu buttons data
  void _read_button_data(std::string& name);

  // Reads in menu data
  void _read_menu_data(std::string& name);

  // Lists saves file to menu2::saves;
  void list_saves();

  // Load correct menu buttons to CurrentMenuButtons for selected level. Levels 100, 101, 102 etc. mean menus
  void load(int level_id);

  // Make quads out of CurrentMenuButtons
  void render();

  // Logic of what happens after clicking on new game
  void _click_new_game();

  // Logic of what happens after clicking on load game 
  void _click_load_game();

  // Logic of what happens after clicking on settings
  void _click_settings();

  // Logic of what happens after clicking on exit
  void _click_exit();

  // Logic of what happens after clicking on newgame_name
  void _click_newgame_name();

  // Logic of what happens after clicking on back to main menu button
  void _click_back();

  // Removes all menu data
  void clear();

  // Method to validate input in new character menu
  bool _validate_input(std::string input);

  // Validates new character name - if exists or is it have more than 0 characters
  bool _validate_name();

}

#endif
