#include <iostream>
#include <map>
#include <SDL2/SDL.h>

#include "camera.h"
#include "events.h"
#include "fonts.h"
#include "game.h"
#include "hero.h"
#include "menu.h"
#include "mouse.h"
#include "saves.h"

#include "../dictionary.h"

namespace events2
{

  std::map <int , sig_ptr> EventsHandler = {};

  void init()
  {
    events2::EventsHandler[IN_GAME_SCENE_ID] = _handle_in_game;
    events2::EventsHandler[MAIN_MENU_SCENE_ID] = _handle_menu;
    events2::EventsHandler[NEWGAME_MENU_SCENE_ID] = _handle_new_game;
    events2::EventsHandler[LOADGAME_MENU_SCENE_ID] = _handle_load_game;
    events2::EventsHandler[SETTINGS_MENU_SCENE_ID] = _handle_menu;
  }

  void _scan_for_camera_move()
  {
    camera::cam.move_x = 0;
    camera::cam.move_y = 0;

    if(game2::KEYBOARD[SDL_SCANCODE_LEFT])
    {
      camera::cam.move_x -= camera::cam.speed;
      //hero::update_frame(MOVE_LEFT);
    } 
    else if(game2::KEYBOARD[SDL_SCANCODE_RIGHT])
    {
      camera::cam.move_x += camera::cam.speed;
    }
    else if(game2::KEYBOARD[SDL_SCANCODE_UP])
    {
      camera::cam.move_y += camera::cam.speed;
    }
    else if(game2::KEYBOARD[SDL_SCANCODE_DOWN])
    {
      camera::cam.move_y -= camera::cam.speed;
    } else 
    {
      //hero::update_frame(STAND_STILL);
    }
    camera::cam.previous_x = camera::cam.x;
    camera::cam.previous_y = camera::cam.y;
    camera::cam.x += camera::cam.move_x;
    camera::cam.y += camera::cam.move_y;

  }

  void _handle_in_game(SDL_Event event)
  {
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
        case SDL_MOUSEWHEEL:
          if(event.wheel.y > 0) 
          {
            camera::cam.zoom += camera::cam.zoom_speed;
          } else if(event.wheel.y < 0)
          {
            camera::cam.zoom -= camera::cam.zoom_speed;
          }
          break;

        case SDL_MOUSEBUTTONDOWN:
          mouse2::handle_mouse(event.motion, event.button);
        break;

        case SDL_QUIT:
          game2::RUNNING = false;
        break;

        case SDL_KEYDOWN: 
          switch (event.key.keysym.sym)
          { 
            case SDLK_ESCAPE:
              game2::switch_scene(MAIN_MENU_SCENE_ID, false);    
            break;
            
            case SDLK_x:
              //hero::attack_state = true;
              //hero::update_frame(ATTACK);
            break; 

            case SDLK_s:
              saves2::save_game();
            break;

            case SDLK_p:
              if(game2::PAUSE)
              {
                game2::PAUSE = false;
              } else 
              {
                game2::PAUSE = true;
              }      
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
        break;
      }
    }
    _scan_for_camera_move();
  };

  void _handle_new_game(SDL_Event event)
  {
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
        case SDL_MOUSEBUTTONDOWN:
          mouse2::handle_mouse(event.motion, event.button);
        break;

        case SDL_QUIT:
          game2::RUNNING = false;
        break;

        case SDL_KEYDOWN: 
          switch (event.key.keysym.sym)
          {
            case SDLK_BACKSPACE:
              if(menu2::NewGameName.size() > 0)
              {
                menu2::NewGameName.pop_back();
                fonts2::labels[fonts2::NEW_GAME_LABEL_ID].text = menu2::NewGameName;
              }
            break;

            case SDLK_RETURN:
              if(menu2::_validate_name())
              {
                game2::switch_scene(2, true);
              }
            break;

            case SDLK_ESCAPE:
              game2::switch_scene(MAIN_MENU_SCENE_ID, false);  
            break;
          }

        break;

        case SDL_TEXTINPUT:
          if(menu2::_validate_input(event.text.text) && menu2::NewGameName.size() < 8)
          {
            menu2::NewGameName += event.text.text;
            fonts2::labels[fonts2::NEW_GAME_LABEL_ID].text = menu2::NewGameName;
          }
        break;
      }
    }
  };

  void _handle_load_game(SDL_Event event)
 { //Will add mouse scrolling later for the a lot of saves
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
        case SDL_MOUSEBUTTONDOWN:
          mouse2::handle_mouse(event.motion, event.button);
        break;

        case SDL_QUIT:
          game2::RUNNING = false;
        break;

        case SDL_KEYDOWN: 
          switch (event.key.keysym.sym)
          {     
            case SDLK_ESCAPE:
              game2::switch_scene(MAIN_MENU_SCENE_ID, false);  
            break;
          }
        break;
      }
    }
  };

  void _handle_menu(SDL_Event event)
  { //Will add mouse scrolling later for the a lot of saves
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
        case SDL_MOUSEBUTTONDOWN:
          mouse2::handle_mouse(event.motion, event.button);
        break;

        case SDL_QUIT:
          game2::RUNNING = false;
        break;
      }
    }
  };

  void handle_events(SDL_Event event)
  {
    events2::EventsHandler[game2::IS_MENU](event);
  }
}
