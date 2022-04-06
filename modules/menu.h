

#include <string>
#include <vector>

#include "buttons.h"
#include "fonts.h"
#include "mouse.h"
#include "quads.h"

#include "../dependencies/json_struct.h"
#include "../dependencies/parallel_hashmap/phmap.h"

#ifndef MODULES_MENU_H
#define MODULES_MENU_H


namespace menu
{

  struct MenuSlotData
  {
    int id;
    int menu_id;
    float x, y;
    bool free = true;

    JS_OBJ(id, x, y);
  };

  // Button data read from the file
  struct MenuButtonData
  {
    std::string label;
    int button_function_id; // reference to button type from buttons
    float x, y;
    float w, h;
    float r, g, b, a; 

    JS_OBJ(label, button_function_id, x, y, w, h, r, g, b, a);
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
    int texture_id = -1;
    int model_id = -1;
    int frame_id = -1;
    int object_type_id;
    int assigned_entity_id;

    quads::Position pos;
    quads::Color color;
    quads::Dims dims;
    quads::Norm norm;

    float r,g,b,a;
    float x,y,z;
    float w,h;

    float camera_type;

    std::string menu_name;
    std::vector<menu::MenuButtonData> button_data;
    std::vector<menu::MenuLabelData> label_data;

    std::vector<int> button_ids;
    std::vector<int> label_ids;
    
    bool is_clicked;
    bool is_reversed = false;

    JS_OBJ(menu_type_id, r, g, b, a, x , y, w, h, texture_id, frame_id, object_type_id, 
    button_data, label_data);
  };

  extern std::vector<int> Index;
  extern phmap::flat_hash_map<int, MenuData> menus;
  extern phmap::flat_hash_map<int, menu::MenuSlotData> menuslots;
  extern phmap::flat_hash_map<int, menu::MenuSlotData> currentmenuslots; 

  extern phmap::flat_hash_map<int, MenuData> currentmenus;
  extern std::vector<std::string> saves;
  extern phmap::flat_hash_map<int, std::string> saves_buttons_map;
  extern std::string NewGameName;
  extern std::string LoadGameName;

  // Read menu data from the file
  void read_menu_data(std::string& name);

  // Read menu slot data from the file
  void read_menu_slot_data(std::string& name);

  // Read all menu datas
  void init();

  // Clears all data
  void refresh();

  // Adds menu into currentmenus by menu_type_id, creates button and labels objects
  // Returns unique menu_id
  int add(int menu_type_id);

  // Add menu to free slot
  int add_to_slot(int menu_type_id, int object_id);

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

  // Returns minimum free menu slot id
  int _get_free_slot();

  // Frees slot id for given menu
  void _free_slot(int menu_id);


}

#endif
