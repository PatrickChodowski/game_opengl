
#include "entity.h"
#include "game.h"
#include "logger.h"
#include "menu.h"
#include "mouse.h"
#include "quads.h"

#include <iostream>
#include <map>
#include <string>
#include <set>

#include "../dictionary.h"

namespace mouse
{
  // for travelling purposes
  float last_click_x;
  float last_click_y;
  
  std::map <int , sig_ptr> click = {};
  std::map <int, int> ClickPriorities;

  // need to aggregate unique_object_types, object_ids and mouse_buttons
  void _find_clicked_quads(float click_x, float click_y, int mouse_button_id)
  {
    std::vector<mouse::ClickData> clicks = {};
    std::set<int> object_types = {};
    int min_click_priority = 1;

    // Search for quads containing click point
    for(auto q : quads::AllQuads)
    {
      if((q.window_x <= click_x) & (click_x < q.window_x+q.window_w) & (click_y >= q.window_y & click_y < q.window_y + q.window_h))
      {
        mouse::ClickData cdd;
        cdd.quad_id = q.id;
        cdd.object_id = q.object_id;
        cdd.object_type_id = q.object_type_id;
        cdd.mouse_button_id = mouse_button_id;
        cdd.priority = mouse::ClickPriorities[q.object_type_id];
        clicks.push_back(cdd);
        object_types.insert(cdd.object_type_id);
      }
    };

    if(object_types.count(OBJECT_TYPE_ENTITY))
    {
      min_click_priority = mouse::ClickPriorities[OBJECT_TYPE_ENTITY];
    }
    if(object_types.count(OBJECT_TYPE_MENU))
    {
      min_click_priority = mouse::ClickPriorities[OBJECT_TYPE_MENU];
    }
    if(object_types.count(OBJECT_TYPE_BUTTON))
    {
      min_click_priority = mouse::ClickPriorities[OBJECT_TYPE_BUTTON];
    }


    for(int c=0; c<clicks.size(); c++)
    {
      if(clicks[c].priority >= min_click_priority)
      {
        // std::cout << " Clicked on quad: " << clicks[c].quad_id  << std::endl;
        mouse::click[clicks[c].object_type_id](clicks[c].object_id, clicks[c].mouse_button_id);
      }
    }
  }

  void _click_entity(int object_id, int mouse_button_id)
  {
    logger::print("Clicked on ENTITY object id: " + std::to_string(object_id) + " with mouse button id: " + std::to_string(mouse_button_id));
    entity::EntityData edd = entity::entities[object_id];
    // if right click
    if((mouse_button_id == MOUSE_BUTTON_RIGHT))
    {
      if(!entity::entities[object_id].is_clicked)
      {
        entity::entities[object_id].menu_id = menu::add_to_slot(MENU_ENTITY_ID, object_id);
      } else 
      {
        menu::drop(entity::entities[object_id].menu_id);
      }

      entity::entities[object_id].is_clicked  = !entity::entities[object_id].is_clicked;
    } 
  };

  void _click_map(int object_id, int mouse_button_id)
  {
    std::cout << "Clicked on MAP object id: " << object_id << " with mouse button id: " << mouse_button_id << std::endl;
  };

  void _click_button(int object_id, int mouse_button_id)
  {
    std::cout << "Clicked on BUTTON object id: " << object_id << " with mouse button id: " << mouse_button_id << std::endl;
    // int logic_object_id = menu::_check_if_load_game(object_id);
    // menu::CurrentMenuButtons[object_id].is_clicked = !menu::CurrentMenuButtons[object_id].is_clicked;
    // menu::ClickButton[object_id]();
    buttons::ButtonFunctions[buttons::buttons[object_id].button_function_id](object_id);
  };

  void _click_menu(int object_id, int mouse_button_id)
  {
    std::cout << "Clicked on MENU object id: " << object_id << " with mouse button id: " << mouse_button_id << std::endl;
  };


  void init()
  {
    mouse::click[OBJECT_TYPE_ENTITY] = _click_entity;
    mouse::click[OBJECT_TYPE_MAP] = _click_map;
    mouse::click[OBJECT_TYPE_BUTTON] = _click_button;
    mouse::click[OBJECT_TYPE_MENU] = _click_menu;

    mouse::ClickPriorities[OBJECT_TYPE_BUTTON] = 6;
    mouse::ClickPriorities[OBJECT_TYPE_MENU] = 5;
    mouse::ClickPriorities[OBJECT_TYPE_ENTITY] = 4;
    mouse::ClickPriorities[OBJECT_TYPE_MAP] = 1;
    mouse::ClickPriorities[OBJECT_TYPE_TEXT] = 0;
    mouse::ClickPriorities[OBJECT_TYPE_DEBUG] = 0;

  }



  void print_mouse(SDL_MouseMotionEvent e, const char* name)
  {
    std::cout << "Click pos: "  << e.x << "," << e.y 
              << " timestamp:  "     << e.timestamp 
              << " Which: " << name
              << std::endl;
  };

  void handle_mouse(SDL_MouseMotionEvent e, SDL_MouseButtonEvent b)
  {
    switch (b.button)
    {
      case SDL_BUTTON_LEFT:
        // mouse::print_mouse(e, "Left");
        mouse::_find_clicked_quads(e.x, e.y, MOUSE_BUTTON_LEFT);
      break;

      case SDL_BUTTON_RIGHT:
        // print_mouse(e, "Right");
        mouse::_find_clicked_quads(e.x, e.y, MOUSE_BUTTON_RIGHT);
      break;

      case SDL_BUTTON_MIDDLE:
        print_mouse(e, "Middle");
      break;
    }
  };







}
