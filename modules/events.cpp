#include <iostream>
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

  void scan_for_camera_move()
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


  void _handle_game_on_controls(SDL_Keycode key)
  {
    switch (key)
    { 
      case SDLK_ESCAPE:
        game2::switch_scene(MAIN_MENU_SCENE_ID);    
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
  };

  void _handle_new_game_name_input(SDL_Keycode key)
  {
    switch (key)
    {
      case SDLK_BACKSPACE:
        if(menu2::NewGameName.size() > 0)
        {
          menu2::NewGameName.pop_back();
        }
      break;

      case SDLK_RETURN:
        if(menu2::_validate_name())
        {

        }
        break;
      case SDLK_ESCAPE:
          game2::switch_scene(MAIN_MENU_SCENE_ID);  
        break;
    }
    std::cout << "tutaj?" << std::endl;
    fonts2::labels[fonts2::NEW_GAME_LABEL_ID].text = menu2::NewGameName;
  };

  void _handle_load_game_menu_input(SDL_Keycode key)
  {
    switch (key)
    {
      case SDLK_ESCAPE:
        game2::switch_scene(MAIN_MENU_SCENE_ID);  
      break;
    }
  };

  int _check_if_menu(int scene_id)
  { 
    if(scene_id < MAIN_MENU_SCENE_ID)
    {
      return IN_GAME_SCENE_ID;
    } else 
    {
      return scene_id;
    }
  }

  void handle_events(SDL_Event event)
  {
    // Possible values: 100, 101, 102, 103, 200
    int scene_id = _check_if_menu(game2::SCENE_ID);

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
        std::cout << "scene_id: " << scene_id << std::endl;
          switch (scene_id)
          {
            case IN_GAME_SCENE_ID:
              events2::_handle_game_on_controls(event.key.keysym.sym);
            break;
            case NEWGAME_MENU_SCENE_ID:
              events2::_handle_new_game_name_input(event.key.keysym.sym);
            break;
            case LOADGAME_MENU_SCENE_ID:
              events2::_handle_load_game_menu_input(event.key.keysym.sym);
            break;
          };
        break;

        case SDL_TEXTINPUT:
          if(scene_id == NEWGAME_MENU_SCENE_ID)
          {
            if(menu2::_validate_input(event.text.text) && menu2::NewGameName.size() < 8)
            {
              menu2::NewGameName += event.text.text;
            }
          }
        break;
      };
    };
  };
}
