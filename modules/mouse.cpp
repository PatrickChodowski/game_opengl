#include "mouse.h"
#include "quads.h"

#include <iostream>
#include <string>

namespace mouse2
{

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
      }
    };
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
