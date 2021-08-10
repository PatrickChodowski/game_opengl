#ifndef EVENTS_H
#define EVENTS_H

// event ids:
#define MOVE_LEFT 0
#define MOVE_RIGHT 1
#define MOVE_UP 2
#define MOVE_DOWN 3
#define STAND_STILL 4
#define ATTACK 5


namespace events
{
  void handle_game_on_controls(SDL_Keycode key)
  {
    switch (key)
    { 
      case SDLK_ESCAPE:
          game::set_state("MAIN_MENU");
        break;
      
      case SDLK_x:
          hero::attack_state = true;
          hero::update_frame(ATTACK);
        break;

      case SDLK_s:
          saves::save_game();
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
      case SDLK_ESCAPE:
          game::set_state("MAIN_MENU");
        break;
    }
  }

  void handle_load_game_menu_input(SDL_Keycode key)
  {
    switch (key)
    {
      case SDLK_ESCAPE:
        game::set_state("MAIN_MENU");
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

      case SDL_KEYDOWN: // has to remove camera/hero move from here in order to run smoothly
        if(game::GAME_STATE["GAME_ON"])
        {
          handle_game_on_controls(event.key.keysym.sym);
        } else if(game::GAME_STATE["NEW_GAME_MENU"])
        {
          handle_new_game_name_input(event.key.keysym.sym);
        } else if(game::GAME_STATE["LOAD_GAME_MENU"])
        {
          handle_load_game_menu_input(event.key.keysym.sym);
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

    // keyboard scanning for camera movement
    // will be moving in only 4 directions
    if(hero::attack_state == false){
      if(KEYBOARD[SDL_SCANCODE_LEFT]){
        camera::move_x -= camera::speed;
        hero::update_frame(MOVE_LEFT);
      } 
      else if(KEYBOARD[SDL_SCANCODE_RIGHT]){
        camera::move_x += camera::speed;
        hero::update_frame(MOVE_RIGHT);
      }
      else if(KEYBOARD[SDL_SCANCODE_UP]){
        camera::move_y += camera::speed;
        hero::update_frame(MOVE_UP);
      }
      else if(KEYBOARD[SDL_SCANCODE_DOWN]){
        camera::move_y -= camera::speed;
        hero::update_frame(MOVE_DOWN);
      } else {
        hero::update_frame(STAND_STILL);
      }
    } else {
      hero::update_frame(ATTACK);
    }

    // if its true, than camera will stay in the centre and rest of the environment will be moving
    if(camera::centric)
    {
      camera::x += camera::move_x;
      camera::y += camera::move_y;
    } 

  }

}



#endif