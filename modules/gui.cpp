
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "buttons.h"
#include "entity.h"
#include "fonts.h"
#include "gui.h"
#include "quads.h"
#include "utils.h"
#include "../dictionary.h"

namespace gui
{
  std::vector<int> Index = {};
  std::map<int, gui::GuiData> guis;
  std::map<int, gui::GuiSlotData> guislots = {};
  std::vector<quads::QuadData> GuiQuads;
  std::map <int , sig_ptr> display = {};

  void init()
  {
    gui::GuiSlotData gsd0;
    gsd0.id = 0;
    gsd0.free = true;
    gsd0.x = 550;
    gsd0.y = 10;
    gsd0.gui_id = -1;
    guislots[0] = gsd0;

    gui::GuiSlotData gsd1;
    gsd1.id = 1;
    gsd1.free = true;
    gsd1.x = 755;
    gsd1.y = 10;
    gsd1.gui_id = -1;
    guislots[1] = gsd1;

    gui::display[OBJECT_TYPE_ENTITY] = entity::display;
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

  int add_context_menu(int object_id, int object_type_id)
  {
    int free_slot = gui::_get_free_slot();
    if(free_slot > -1)
    {
      gui::GuiData gdd;
      gdd.id = utils::generate_id(gui::Index);

      gdd.x = gui::guislots[free_slot].x;
      gdd.y = gui::guislots[free_slot].y;

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

      gui::guis[gdd.id] = gdd;


      gui::guislots[free_slot].gui_id = gdd.id;
      gui::guislots[free_slot].free = false;

      // Add labels
      int label_id = gui::display[object_type_id](object_id, free_slot);
      gdd.labels.push_back(label_id);
      
      // Add buttons (menu? entity menu? - redefine menu?)
      int button_id = buttons::add("click", 
                                   gdd.x+5, 
                                   gdd.y+70, 1);
      gdd.buttons.push_back(button_id);

      gui::guis[gdd.id] = gdd;

      return gdd.id;
    } else 
    {
      return -1;
    }

  };

  void render()
  {
    gui::GuiQuads.clear();
    gui::GuiQuads = quads::make_quads(gui::guis, OBJECT_TYPE_GUI);
  }


  void drop(int gui_id)
  {
    // Delete labels assigned to this gui
    for(int b=0; b<gui::guis[gui_id].buttons.size(); b++)
    {
      buttons::drop(gui::guis[gui_id].buttons[b]);
    }

    for(int l=0; l<gui::guis[gui_id].labels.size(); l++)
    {
      fonts::drop(gui::guis[gui_id].labels[l]);
    }

    gui::_free_slot(gui_id);
    guis.erase(gui_id);
    utils::drop_id(gui::Index, gui_id);

  }
  

  void clear()
  {
    gui::Index.clear();
    gui::guis.clear();
    gui::guislots.clear();
    gui::GuiQuads.clear();
  }

}
