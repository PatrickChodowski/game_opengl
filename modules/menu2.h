
#include <map>
#include <string>
#include <vector>

#include "buttons.h"
#include "fonts.h"
#include "mouse.h"
#include "quads.h"

#include "../dependencies/json_struct.h"

#ifndef MODULES_MENU2_H
#define MODULES_MENU2_H


namespace menu
{

  struct MenuSlotData
  {
    int id;
    int menu_id;
    float x, y;
    bool free;

    JS_OBJ(id, x, y);
  };

  // Button data read from the file
  struct MenuButtonData
  {
    std::string label;
    int button_function_id; // reference to button type from buttons
    float x, y;

    JS_OBJ(label, button_function_id, x, y);
  };

  // Label data read from the file
  struct MenuLabelData
  {
    std::string label;
    float x, y;
    float r, g, b;

    JS_OBJ(label, x, y, r, g, b);
  };

  // Menu data read in from the data/menu2
  struct MenuData
  {
    int id;
    int menu_type_id;
    int menu_slot_id;
    int texture_id;
    int frame_id;

    float x, y, z;
    float w, h;
    float r, g, b, a;
    float camera_type;
    float norm_x_start, norm_x_end;

    std::string menu_name;
    std::vector<menu::MenuButtonData> button_data;
    std::vector<menu::MenuLabelData> label_data;

    std::vector<int> button_ids;
    std::vector<int> label_ids;
    
    bool is_clicked;

    JS_OBJ(menu_type_id, x, y, w, h, texture_id, frame_id, 
    r, g, b, a, button_data, label_data);
  };

  extern std::vector<int> Index;
  extern std::map<int, MenuData> menus;
  extern std::map<int, menu::MenuSlotData> menuslots;
  extern std::map<int, MenuData> currentmenus;
  extern std::vector<quads::QuadData> MenuQuads;
  extern std::vector<std::string> saves;
  extern std::string NewGameName;

  // Read menu data from the file
  void read_menu_data(std::string& name);

  // Read menu slot data from the file
  void read_menu_slot_data(std::string& name);

  // Read all menu datas
  void init();

  // Adds menu into currentmenus by menu_type_id, creates button and labels objects
  // Returns unique menu_id
  int add(int menu_type_id);

  // Creates quads out of the currentmenus
  void render();

  // Clears all menu data
  void clear();

  // Remove menu by its id, remove corresponding buttons and labels
  void drop(int menu_id);

  // Lists saves file to menu::saves;
  void _list_saves();

  // Method to validate input in new character menu
  bool _validate_input(std::string input);

  // Validates new character name - if exists or is it have more than 0 characters
  bool _validate_name();

  // Checks if clicked button was for a load game
  int _check_if_load_game(int button_id);

  // Returns minimum free menu slot id
  int _get_free_slot();

  // Frees slot id for given menu
  void _free_slot(int menu_id);


}

#endif
