#ifndef MOUSE_H
#define MOUSE_H

namespace mouse
{
  

  struct Mouse
  {
    int x;
    int y;

    int last_click_timestamp;
    bool was_released;
  };

  void print_mouse(SDL_MouseMotionEvent e, std::string  name)
  {
    std::cout << "Click pos: "  << e.x << "," << e.y 
              // << " state:  "    << e.state 
              // << " type:  "     << e.type  // SDL_MOuseMotion
              // << " which:  "    << e.which // mouse instance id
              // << " xrel:  "     << e.xrel 
              // << " yrel:  "     << e.yrel 
              << " timestamp:  "     << e.timestamp 
              << " Which: " << name
              << std::endl;
  }

  void handle_mouse(SDL_MouseMotionEvent e,  SDL_MouseButtonEvent b)
  {
    switch (b.button)
    {
      case SDL_BUTTON_LEFT:
        print_mouse(e, "Left");
      break;

      case SDL_BUTTON_RIGHT:
        print_mouse(e, "Right");
      break;

      case SDL_BUTTON_MIDDLE:
        print_mouse(e, "Middle");
      break;

      case SDL_BUTTON_X1:
        print_mouse(e, "X1");
      break;

      case SDL_BUTTON_X2:
        print_mouse(e, "X2");
      break;
    }
  }










}


#endif