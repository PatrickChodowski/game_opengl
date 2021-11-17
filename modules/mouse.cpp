#include "mouse.h"
#include "quads.h"

#include <iostream>
#include <map>
#include <string>

#include "../dictionary.h"

namespace mouse2
{

  std::map <int , sig_ptr> click = {};

  void _find_clicked_quads(float click_x, float click_y)
  {
    int quad_id;
    for(auto q : quads2::AllQuads)
    {
      if((q.window_x <= click_x) & (click_x < q.window_x+q.window_w) & (click_y >= q.window_y & click_y < q.window_y + q.window_h))
      {
        std::cout << "Clicked on quad: " << q.id 
                  << " object ID: " << q.object_id 
                  << " object type id " << q.object_type_id 
                  << std::endl;

        mouse2::click[q.object_type_id](q.object_id);
      }
    };
  }

  // how to handle different types of click -> on menu, on map, on entity, left click, right click etc.


  void _click_menu(int object_id)
  {
    std::cout << "Clicked on menu object id: " << object_id << std::endl;
  };

  void _click_entity(int object_id)
  {
    std::cout << "Clicked on entity object id: " << object_id << std::endl;
  };

  void _click_map(int object_id)
  {
    std::cout << "Clicked on map object id: " << object_id << std::endl;
  };

  void _click_text(int object_id)
  {
    std::cout << "Clicked on text object id: " << object_id << std::endl;
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
        mouse2::print_mouse(e, "Left");
        mouse2::_find_clicked_quads(e.x, e.y);
      break;

      case SDL_BUTTON_RIGHT:
        print_mouse(e, "Right");
      break;

      case SDL_BUTTON_MIDDLE:
        print_mouse(e, "Middle");
      break;
    }
  };







}
