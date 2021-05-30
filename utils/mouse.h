#ifndef MOUSE_H
#define MOUSE_H

namespace mouse
{
  void query_quads(int x, int y, std::vector<quads::Quad> v)
  {
    int quad_id;
    logger::print("projected tile dim: " + std::to_string(camera::projected_tile_dim));
    for(auto q : v)
    {

      if((q.x <= x) & (x < q.x+camera::projected_tile_dim) & (y >= q.y & y < q.y + camera::projected_tile_dim))
      {
          logger::print("Clicked tile_id: " + std::to_string(q.id)); 
      }
    };
    //return quad_id;
  }

  

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

  void handle_mouse(SDL_MouseMotionEvent e,  SDL_MouseButtonEvent b, std::vector<quads::Quad> v)
  {
    switch (b.button)
    {
      case SDL_BUTTON_LEFT:
        print_mouse(e, "Left");
        query_quads(e.x, e.y, v);
      break;

      case SDL_BUTTON_RIGHT:
        print_mouse(e, "Right");
      break;

      case SDL_BUTTON_MIDDLE:
        print_mouse(e, "Middle");
      break;
    }




  }










}


#endif