
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


namespace menu2
{
  struct MenuData
  {
    int id;
    int menu_slot_id;
    int texture_id;
    int frame_id;

    float r, g, b, a;
    float camera_type;

    std::vector<int> button_ids;
    std::vector<int> label_ids;
    std::vector<buttons::GuiButtonData> buttons;
    std::vector<fonts::LabelData> labels;
    std::string menu_name;

    bool is_clicked;
  };

  std::vector<int> Index;
  std::map<int, MenuData> menus;

  void read_data(std:string name);

  void init();

  void load();

  void clear();

  void drop();

}

#endif
