
#include <map>
#include <string>
#include <vector>

#include "quads.h"
#include "utils.h"

#ifndef MODULES_BUTTONS_H
#define MODULES_BUTTONS_H

// In-game buttons implementations.
namespace buttons
{
  // It should trigger a function when clicked
  struct GuiButtonData
  {
    int id;
    int label_id;
    int button_function_id;
    int texture_id = 0;
    int frame_id = 0;

    float x, y;
    float w, h;

    float r, g, b, a;
    float norm_x_start;
    float norm_x_end;
    float camera_type;

    bool is_clicked;
  };

  typedef void (*sig_ptr)(int);
  // Catalog of functions to be chosen based on the button_function_id
  extern std::map <int , sig_ptr> ButtonFunctions;
  extern std::vector<int> Index;
  extern std::map <int, buttons::GuiButtonData> guibuttons;
  extern std::vector<quads::QuadData> GuiButtonQuads;

  // Initialize GuiButtons logic and data
  void init();

  // Add GUI button. Returns button_id
  int add(std::string text, float x, float y, int button_function_id);

  // Render quads
  void render();

  // Drop selected Gui button by button_id
  void drop(int button_id);

  // Clear all buttons data
  void clear();

  // functions

  // Logic of what happens after clicking on new game
  void _click_new_game(int placeholder);

  // Logic of what happens after clicking on load game 
  void _click_load_game(int placeholder);

  // Logic of what happens after clicking on settings
  void _click_settings(int placeholder);

  // Logic of what happens after clicking on exit
  void _click_exit(int placeholder);

  // Logic of what happens after clicking on newgame_name
  void _click_newgame_name(int placeholder);

  // Logic of what happens after clicking on newgame_name
  void _click_loadgame_name(int placeholder);

  // Logic of what happens after clicking on back to main menu button
  void _click_back(int placeholder);

  // Start new travel object
  void _button_travel(int object_id);


}

#endif
