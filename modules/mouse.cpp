#include "buttons.h"
#include "camera.h"
#include "ecs.h"
#include "game.h"
#include "logger.h"
#include "mouse.h"
#include "quads.h"

#include <iostream>
#include <string>
#include <set>

#include "../dictionary.h"
#include "../dependencies/parallel_hashmap/phmap.h"

namespace mouse
{
  mouse::LastClickData last_click;  
  phmap::flat_hash_map<int , sig_ptr> click = {};
  phmap::flat_hash_map<int, int> ClickPriorities;


  void init()
  {
    mouse::click[ENTITY_TYPE_LIVE] = _click_entity_type_live;
    mouse::click[ENTITY_TYPE_MAP] = _click_entity_type_map;
    mouse::click[ENTITY_TYPE_BUTTON] = _click_entity_type_button;
    mouse::click[ENTITY_TYPE_GUI] = _click_entity_type_gui;

    mouse::ClickPriorities[ENTITY_TYPE_BUTTON] = 6;
    mouse::ClickPriorities[ENTITY_TYPE_GUI] = 5;
    mouse::ClickPriorities[ENTITY_TYPE_LIVE] = 4;
    mouse::ClickPriorities[ENTITY_TYPE_MAP] = 1;
    std::cout << "Mouse Initialized" << std::endl;
  }


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
        cdd.entity_id = q.entity_id;
        cdd.entity_type_id = q.entity_type_id;
        cdd.mouse_button_id = mouse_button_id;
        cdd.priority = mouse::ClickPriorities[q.entity_type_id];
        clicks.push_back(cdd);
        object_types.insert(cdd.entity_type_id);
      }
    };

    // Should be based on Z later?
    if(object_types.count(ENTITY_TYPE_LIVE))
    {
      min_click_priority = mouse::ClickPriorities[ENTITY_TYPE_LIVE];
    }
    if(object_types.count(ENTITY_TYPE_GUI))
    {
      min_click_priority = mouse::ClickPriorities[ENTITY_TYPE_GUI];
    }
    if(object_types.count(ENTITY_TYPE_BUTTON))
    {
      min_click_priority = mouse::ClickPriorities[ENTITY_TYPE_BUTTON];
    }

    for(int c=0; c<clicks.size(); c++)
    {
      if(clicks[c].priority >= min_click_priority)
      {
        mouse::last_click.window_x = click_x;
        mouse::last_click.window_y = click_y;

        mouse::last_click.world_x = camera::reverse_coord_x(click_x, camera::cam.x, camera::cam.zoom);
        mouse::last_click.world_y = camera::reverse_coord_y(click_y, camera::cam.y, camera::cam.zoom);

        mouse::click[clicks[c].entity_type_id](clicks[c].entity_id, clicks[c].mouse_button_id);

        // Additionally check if there is click function assigned
        if(ecs::buttons.count(clicks[c].entity_id))
        {
          int button_func_id = ecs::buttons.at(clicks[c].entity_id).button_function_id;
          std::cout << " [MOUSE] Executing button function ID: " << button_func_id << std::endl; 
          buttons::ButtonFunctions.at(button_func_id)(-1);
        }

      }
    }
  }

  void _click_entity_type_live(int object_id, int mouse_button_id)
  {
    logger::print("Clicked on ENTITY object id: " + std::to_string(object_id) + " with mouse button id: " + std::to_string(mouse_button_id));
    // if right click
    // if(mouse_button_id == MOUSE_BUTTON_RIGHT){
    //   if(!entity::entities.at(object_id).is_clicked){
    //     entity::entities.at(object_id).menu_id = menu::add_to_slot(MENU_ENTITY_ID, object_id);
    //   } else {
    //     menu::drop(entity::entities.at(object_id).menu_id);
    //   }
    //   entity::entities.at(object_id).is_clicked  = !entity::entities.at(object_id).is_clicked;
    // } 
  };

  void _click_entity_type_map(int object_id, int mouse_button_id)
  {
    std::cout << "Clicked on MAP object id: " << object_id << " with mouse button id: " << mouse_button_id << std::endl;
  };

  void _click_entity_type_gui(int object_id, int mouse_button_id)
  {
    std::cout << "Clicked on GUI object id: " << object_id << " with mouse button id: " << mouse_button_id << std::endl;
  };

  void _click_entity_type_button(int object_id, int mouse_button_id)
  {
    std::cout << "Clicked on BUTTON object id: " << object_id << " with mouse button id: " << mouse_button_id << std::endl;
    // int logic_object_id = menu::_check_if_load_game(object_id);
    // menu::CurrentMenuButtons[object_id].is_clicked = !menu::CurrentMenuButtons[object_id].is_clicked;
    // menu::ClickButton[object_id]();
    // buttons::buttons[object_id].is_clicked = !buttons::buttons[object_id].is_clicked;
    // buttons::ButtonFunctions[buttons::buttons[object_id].button_function_id](object_id);
  };

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
