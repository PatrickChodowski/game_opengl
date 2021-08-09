#ifndef EVENTS_H
#define EVENTS_H


namespace events
{

  void handle_game_on_controls(SDL_Keycode key)
  {
    switch (key)
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
          game::set_state("MAIN_MENU");
        break;

        // case SDLK_l:
        //   CURRENT_SHADER = "light_radius_shading_program";
        //   break;
        // case SDLK_g:
        //   CURRENT_SHADER = "canvas";
        //   break;
    }
  }

  void handle_new_game_name_input(SDL_Keycode key)
  {
    switch (key)
    {
      case SDLK_BACKSPACE:
        if(menu::NewGameName.size() > 0)
        {
          menu::NewGameName.pop_back();
          game::CHANGE_STATE_TRIGGER = true;
        }
        break;
      case SDLK_RETURN:
        if(menu::validate_name()){
          CAMPAIGN_NAME = menu::NewGameName;
          game::set_state("GAME_ON");
        }
        break;
    }
  }


  
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
          if (game::GAME_STATE["GAME_ON"]){
            if(event.wheel.y > 0) {camera::zoom += camera::zoom_speed;}
            else if(event.wheel.y < 0){camera::zoom -= camera::zoom_speed;}
          }
          break;
        
        case SDL_MOUSEBUTTONDOWN:
          mouse::handle_mouse(event.motion, event.button, v, lv);
        break;

        case SDL_QUIT:
          RUNNING = false;
          break;

      case SDL_KEYDOWN:
        if(game::GAME_STATE["GAME_ON"])
        {
          handle_game_on_controls(event.key.keysym.sym);
        } else if (game::GAME_STATE["NEW_GAME_MENU"])
        {
          handle_new_game_name_input(event.key.keysym.sym);
        }
        break;
      case SDL_TEXTINPUT:
        if(game::GAME_STATE["NEW_GAME_MENU"])
        {
          if(menu::validate_input(event.text.text) && menu::NewGameName.size() < 8)
          {
            menu::NewGameName += event.text.text;
            game::CHANGE_STATE_TRIGGER = true;
          }
          break;
        }
        break;
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