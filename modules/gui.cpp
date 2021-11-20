
#include <map>
#include <string>
#include <vector>

#include "gui.h"
#include "quads.h"
#include "utils.h"
#include "../dictionary.h"

namespace gui2
{
  int active_guis = 0;
  std::vector<int> UsedGuiIds = {};
  std::map<int, gui2::GuiData> guis;
  std::vector<quads2::QuadData> GuiQuads;

  int add(float x, float y, int object_id, int object_type_id)
  {
    gui2::GuiData gdd;
    gdd.id = utils2::generate_id(gui2::UsedGuiIds);
    gdd.x = x;
    gdd.y = y;

    gdd.w = 200;
    gdd.h = 400;
    gdd.r = 1.0;
    gdd.g = 1.0;
    gdd.b = 1.0;
    gdd.a = 0.8;

    gdd.texture_id = 0;
    gdd.frame_id = 0;
    gdd.is_clicked = false;
    gdd.norm_x_start = 0.0;
    gdd.norm_x_end = 0.0;
    gdd.camera_type = CAMERA_STATIC;

    gui2::active_guis += 1;
    gui2::guis[gdd.id] = gdd;
    return gdd.id;
  };

  void render()
  {
    gui2::GuiQuads.clear();
    gui2::GuiQuads = quads2::make_quads(gui2::guis, OBJECT_TYPE_GUI);
  }


  void drop(int gui_id)
  {
    guis.erase(gui_id);
    gui2::active_guis -= 1;
  }
  

  void clear()
  {
    gui2::UsedGuiIds.clear();
    gui2::guis.clear();
    gui2::GuiQuads.clear();
  }

}
