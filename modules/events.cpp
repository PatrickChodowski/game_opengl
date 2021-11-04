#include <SDL2/SDL.h>
#include "../dictionary.h"
#include "events.h"
#include "game.h"

namespace events2
{

  void _handle_game_on_controls(SDL_Keycode key)
  {
    switch (key)
    { 
      case SDLK_ESCAPE:
          //game::set_state("MAIN_MENU");
        break;
      
      case SDLK_x:
          //hero::attack_state = true;
          //hero::update_frame(ATTACK);
        break;

      case SDLK_s:
          //saves::save_game();
        break;

      case SDLK_d:
          // if(DEBUG_MODE)
          // {
          //   DEBUG_MODE = false;
          // } else {
          //   DEBUG_MODE = true;
          // }
        break;

      case SDLK_l:
      // toggle between base and light shader
          // if(CURRENT_SHADER_ID == 0)
          // {
          //   CURRENT_SHADER_ID = 1;
          // } else {
          //   CURRENT_SHADER_ID = 0;
          // }
      break;
    }

  };

  void _handle_new_game_name_input(SDL_Keycode key)
  {
    switch (key)
    {
      case SDLK_BACKSPACE:
        // if(menu::NewGameName.size() > 0)
        // {
        //   menu::NewGameName.pop_back();
        //   game::CHANGE_STATE_TRIGGER = true;
        // }
        break;
      case SDLK_RETURN:
        // if(menu::validate_name()){
        //   CAMPAIGN_NAME = menu::NewGameName;
        //   NEW_GAME = true;
        //   game::set_state("GAME_ON");
        // }
        break;
      case SDLK_ESCAPE:
          // game::set_state("MAIN_MENU");
        break;
    }

  };

  void _handle_load_game_menu_input(SDL_Keycode key)
  {
    switch (key)
    {
      case SDLK_ESCAPE:
        // game::set_state("MAIN_MENU");
      break;
    }
  };

  void handle_events(SDL_Event event)
  {
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
        case SDL_MOUSEWHEEL:
          // if (game::GAME_STATE["GAME_ON"]){
          //   if(event.wheel.y > 0) {camera::zoom += camera::zoom_speed;}
          //   else if(event.wheel.y < 0){camera::zoom -= camera::zoom_speed;}
          // }
          break;
        
        case SDL_MOUSEBUTTONDOWN:
          //mouse::handle_mouse(event.motion, event.button, quads);
        break;

        case SDL_QUIT:
          game2::RUNNING = false;
          break;

      case SDL_KEYDOWN: // has to remove camera/hero move from here in order to run smoothly
        // if(game::GAME_STATE["GAME_ON"])
        // {
        //   handle_game_on_controls(event.key.keysym.sym);
        // } else if(game::GAME_STATE["NEW_GAME_MENU"])
        // {
        //   handle_new_game_name_input(event.key.keysym.sym);
        // } else if(game::GAME_STATE["LOAD_GAME_MENU"])
        // {
        //   handle_load_game_menu_input(event.key.keysym.sym);
        // }
        break;
      case SDL_TEXTINPUT:
        // if(game::GAME_STATE["NEW_GAME_MENU"])
        // {
        //   if(menu::validate_input(event.text.text) && menu::NewGameName.size() < 8)
        //   {
        //     menu::NewGameName += event.text.text;
        //     game::CHANGE_STATE_TRIGGER = true;
        //   }
        //   break;
        // }
        break;
      };
    };
  };

}
