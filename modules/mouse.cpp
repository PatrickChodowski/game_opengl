#include "mouse.h"
#include <iostream>
#include <string>

namespace mouse2
{

  void print_mouse(SDL_MouseMotionEvent e, std::string  name)
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
        print_mouse(e, "Left");
        //query_quads(e.x, e.y, quads);
        //mobs::switch_aggro();
        // e.x and e.y are for window. Its not scaled
        //mobs::move_aggro_mobs_to_point(e.x, e.y);
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
