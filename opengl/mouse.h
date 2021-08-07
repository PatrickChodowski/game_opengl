#ifndef MOUSE_H
#define MOUSE_H


namespace mouse
{
  // temporarily here?
  void click_quad(int clicked_quad_id)
  {
    logger::print("Quad ID: " + std::to_string(clicked_quad_id));

    logger::print("Clicking on " + std::to_string(clicked_quad_id) + 
    " is clicked value: " + 
    std::to_string(quads::AllQuads[clicked_quad_id].is_clicked) + 
    " double check quad id: " + 
    std::to_string(quads::AllQuads[clicked_quad_id].id));

    if(quads::AllQuads[clicked_quad_id].is_clicked == 0.0f)
    {
      quads::AllQuads[clicked_quad_id].is_clicked = 1.0f;
    } else {
      quads::AllQuads[clicked_quad_id].is_clicked = 0.0f;
    }
    ;
  }


  void query_quads(int x, int y, std::vector<quads::ScaledQuad> sv, std::vector<quads::Quad> lv)
  {
    int quad_id;
    for(auto q : sv)
    {
      if((q.x <= x) & (x < q.x+q.w) & (y >= q.y & y < q.y + q.h))
      {
          logger::print("Clicked tile_id: " + std::to_string(q.id)); 
          click_quad(q.id);

      }
    };
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
              << " timestamp:  "     << e.timestamp 
              << " Which: " << name
              << std::endl;
  }

  void handle_mouse(SDL_MouseMotionEvent e,  SDL_MouseButtonEvent b, std::vector<quads::ScaledQuad> v, std::vector<quads::Quad> lv)
  {
    switch (b.button)
    {
      case SDL_BUTTON_LEFT:
        print_mouse(e, "Left");
        query_quads(e.x, e.y, v, lv);
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