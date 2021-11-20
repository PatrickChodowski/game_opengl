
#include <map>
#include <string>
#include <vector>

#include "quads.h"
#include "utils.h"
#include "../dictionary.h"

#ifndef MODULES_GUI_H
#define MODULES_GUI_H

namespace gui2
{
  struct GuiSlotData
  {
    int id;
    int gui_id;
    float x, y;
    bool free;
  };

  struct GuiData
  {
    int id;
    int texture_id;
    int frame_id;

    float x, y;
    float w, h;
    float r, g, b, a;

    float camera_type;
    float norm_x_start, norm_x_end;

    bool is_clicked;

    std::vector<int> labels;
    std::vector<int> buttons;

  };

  extern std::vector<int> UsedGuiIds;
  extern std::map<int, gui2::GuiData> guis;
  extern std::map<int, gui2::GuiSlotData> guislots;
  extern std::vector<quads2::QuadData> GuiQuads;

  typedef int (*sig_ptr)(int, int);
  // Catalog of functions to be chosen based on the object_type_id. Takes object_id and gui_slot_id as arguments
  extern std::map <int , sig_ptr> display;

  // Initialize objects and logic for gui
  void init();

  // Returns minimum free slot id
  int _get_free_slot();

  // Returns slot id for given gui
  int _free_slot();

  // Add context menu for specific object in the game
  int add_context_menu(int object_id, int object_type_id);

  // Makes quads - GuiQuads from guis
  void render();

  // Remove gui from guis
  void drop(int gui_id);

  // Clears all gui data
  void clear();

}

#endif
