
#include "entity.h"
#include "game.h"
#include "gui.h"
#include "logger.h"
#include "menu.h"
#include "mouse.h"
#include "quads.h"

#include <iostream>
#include <map>
#include <string>

#include "../dictionary.h"

namespace mouse2
{

  std::map <int , sig_ptr> click = {};

  void _find_clicked_quads(float click_x, float click_y, int mouse_button_id)
  {
    for(auto q : quads2::AllQuads)
    {
      if((q.window_x <= click_x) & (click_x < q.window_x+q.window_w) & (click_y >= q.window_y & click_y < q.window_y + q.window_h))
      {
        std::cout << " Clicked on quad: " << q.id  << std::endl;
        mouse2::click[q.object_type_id](q.object_id, mouse_button_id);
      }
    };
  }

  void _click_menu(int object_id, int mouse_button_id)
  {
    std::cout << "Clicked on menu object id: " << object_id << " with button id: " << mouse_button_id << std::endl;
    std::cout << "is clicked current value: " <<  menu2::CurrentMenuButtons[object_id].is_clicked << std::endl;

    // Making sure its clicking on button_id type 6 to trigger the logic, not the save ID (over > 100)
    int logic_object_id = menu2::_check_if_load_game(object_id);

    menu2::CurrentMenuButtons[object_id].is_clicked = !menu2::CurrentMenuButtons[object_id].is_clicked;
    menu2::ClickButton[logic_object_id]();

  };

  void _click_entity(int object_id, int mouse_button_id)
  {
    logger::print("Clicked on entity object id: " + std::to_string(object_id) + " with mouse  button id: " + std::to_string(mouse_button_id));
    // entity::EntityData edd = entity::entities[object_id];
    // if right click and debug mode is on
    if((mouse_button_id == MOUSE_BUTTON_RIGHT) & game2::IS_DEBUG_MODE)
    {
      if(!entity::entities[object_id].is_clicked)
      {
        entity::entities[object_id].gui_popup_id = gui2::add(object_id, OBJECT_TYPE_ENTITY);
      } else 
      {
        gui2::drop(entity::entities[object_id].gui_popup_id);
      }
      entity::entities[object_id].is_clicked  = !entity::entities[object_id].is_clicked;
    } 
  };

  void _click_map(int object_id, int mouse_button_id)
  {
    std::cout << "Clicked on map object id: " << object_id << " with button id: " << mouse_button_id << std::endl;
  };

  void _click_text(int object_id, int mouse_button_id)
  {
    std::cout << "Clicked on text object id: " << object_id<< " with button id: " << mouse_button_id << std::endl;
  };


  void init()
  {
    mouse2::click[OBJECT_TYPE_ENTITY] = _click_entity;
    mouse2::click[OBJECT_TYPE_MAP] = _click_map;
    mouse2::click[OBJECT_TYPE_MENU] = _click_menu;
    mouse2::click[OBJECT_TYPE_TEXT] = _click_text;
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
        // mouse2::print_mouse(e, "Left");
        mouse2::_find_clicked_quads(e.x, e.y, MOUSE_BUTTON_LEFT);
      break;

      case SDL_BUTTON_RIGHT:
        // print_mouse(e, "Right");
        mouse2::_find_clicked_quads(e.x, e.y, MOUSE_BUTTON_RIGHT);
      break;

      case SDL_BUTTON_MIDDLE:
        print_mouse(e, "Middle");
      break;
    }
  };







}
