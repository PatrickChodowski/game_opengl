#ifndef EVENTS_H
#define EVENTS_H


namespace events
{
  
  void handle_events(SDL_Event event, std::vector<quads::ScaledQuad> v, std::vector<quads::Quad> lv)
  // subsystem for handling players input
  {

    camera::move_x = 0;
    camera::move_y = 0;
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
        case SDL_MOUSEWHEEL:
          if(event.wheel.y > 0) {camera::zoom += camera::zoom_speed;}
          else if(event.wheel.y < 0){camera::zoom -= camera::zoom_speed;}
          break;
        
        case SDL_MOUSEBUTTONDOWN:
          mouse::handle_mouse(event.motion, event.button, v, lv);
        break;

        case SDL_QUIT:
          RUNNING = false;
          break;

      case SDL_KEYDOWN:
        switch (event.key.keysym.sym)
        {
        case SDLK_LEFT:
          camera::move_x -= camera::speed;
          break;
        case SDLK_RIGHT:
          camera::move_x += camera::speed;
          break;
        case SDLK_UP:
          camera::move_y += camera::speed;
          break;
        case SDLK_DOWN:
          camera::move_y -= camera::speed;
          break;
        
        case SDLK_m:
          if (MAIN_MENU_ON)
            {MAIN_MENU_ON = false;}
          else {MAIN_MENU_ON = true;}
          break;

        // case SDLK_l:
        //   CURRENT_SHADER = "light_radius_shading_program";
        //   break;
        // case SDLK_g:
        //   CURRENT_SHADER = "canvas";
        //   break;
        }
      };
    };

    // if its true, than camera will stay in the centre and rest of the environment will be moving
    if(camera::centric)
    {
      camera::x += camera::move_x;
      camera::y += camera::move_y;
    } 

  }

}



#endif