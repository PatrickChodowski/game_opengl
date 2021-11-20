
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

  extern int active_guis;
  extern std::vector<int> UsedGuiIds;
  extern std::map<int, gui2::GuiData> guis;
  extern std::vector<quads2::QuadData> GuiQuads;

  // Adds new gui to guis list, returns gui id
  int add(float x, float y, int object_id, int object_type_id);

  // Makes quads - GuiQuads from guis
  void render();

  // Remove gui from guis
  void drop(int gui_id);

  // Clears all gui data
  void clear();

}

#endif
