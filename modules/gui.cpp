
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "gui.h"
#include "quads.h"
#include "utils.h"
#include "../dictionary.h"

namespace gui2
{
  std::vector<int> UsedGuiIds = {};
  std::map<int, gui2::GuiData> guis;
  std::map<int, gui2::GuiSlotData> guislots = {};
  std::vector<quads2::QuadData> GuiQuads;

  void init()
  {
    gui2::GuiSlotData gsd0;
    gsd0.id = 0;
    gsd0.free = true;
    gsd0.x = 550;
    gsd0.y = 10;
    gsd0.gui_id = -1;
    guislots[0] = gsd0;

    gui2::GuiSlotData gsd1;
    gsd1.id = 1;
    gsd1.free = true;
    gsd1.x = 755;
    gsd1.y = 10;
    gsd1.gui_id = -1;
    guislots[1] = gsd1;
  }

  int _get_free_slot()
  {
    int min_slot_id = -1;
    for (auto const& [k, v] : guislots)
    {
      if(v.free)
      {
        min_slot_id = k;
        break;
      }
    }
    return min_slot_id;
  }

  void _free_slot(int gui_id)
  { 
    for (auto & [k, v] : guislots)
    {
      if(v.gui_id == gui_id)
      {
        v.free = true;
        v.gui_id = -1;
      }
    }
  }

  int add(int object_id, int object_type_id)
  {
    int free_slot = gui2::_get_free_slot();
    if(free_slot > -1)
    {
      gui2::GuiData gdd;
      gdd.id = utils2::generate_id(gui2::UsedGuiIds);

      gdd.x = gui2::guislots[free_slot].x;
      gdd.y = gui2::guislots[free_slot].y;

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

      gui2::guis[gdd.id] = gdd;
      gui2::guislots[free_slot].gui_id = gdd.id;
      gui2::guislots[free_slot].free = false;

      return gdd.id;
    } else 
    {
      return -1;
    }

  };

  void render()
  {
    gui2::GuiQuads.clear();
    gui2::GuiQuads = quads2::make_quads(gui2::guis, OBJECT_TYPE_GUI);
  }


  void drop(int gui_id)
  {
    gui2::_free_slot(gui_id);
    guis.erase(gui_id);
    utils2::drop_id(gui2::UsedGuiIds, gui_id);
  }
  

  void clear()
  {
    gui2::UsedGuiIds.clear();
    gui2::guis.clear();
    gui2::guislots.clear();
    gui2::GuiQuads.clear();
  }

}
