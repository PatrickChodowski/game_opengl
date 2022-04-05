#include <iostream>
#include <SDL2/SDL.h>

#include "anims.h"
#include "camera.h"
#include "collisions.h"
#include "events.h"
#include "fonts.h"
#include "game.h"
#include "hero.h"
#include "items.h"
#include "menu.h"
#include "mouse.h"
#include "saves.h"

#include "../scripts/py.h"


#include "../dictionary.h"
#include "../dependencies/parallel_hashmap/phmap.h"

namespace events
{

  phmap::flat_hash_map<int,sig_ptr> EventsHandler = {};

  void init()
  {
    events::EventsHandler[EVENT_HANDLER_MENU] = _handle_menu;
    events::EventsHandler[EVENT_HANDLER_NEW_GAME_MENU] = _handle_new_game;
    events::EventsHandler[EVENT_HANDLER_LOAD_GAME_MENU] = _handle_load_game;
    events::EventsHandler[EVENT_HANDLER_IN_GAME] = _handle_in_game;
    std::cout << "Events Initialized" << std::endl;
  }

  void _scan_for_camera_move()
  {
    camera::cam.move_x = 0;
    camera::cam.move_y = 0;


    if(game::KEYBOARD[SDL_SCANCODE_LEFT])
    {
      camera::cam.move_x -= hero::hero.speed;
      entity::entities.at(hero::hero.entity_id).side_id = ANIM_SIDE_LEFT;
      anims::start(ANIM_UNARMED_WALK_FORWARD, hero::hero.entity_id);

      //hero::animate_items(ANIM_MOVE_LEFT);
    } 
    else if(game::KEYBOARD[SDL_SCANCODE_RIGHT])
    {
      camera::cam.move_x += hero::hero.speed;
      entity::entities.at(hero::hero.entity_id).side_id = ANIM_SIDE_RIGHT;
      anims::start(ANIM_UNARMED_WALK_FORWARD, hero::hero.entity_id);
      //hero::animate_items(ANIM_MOVE_RIGHT);
    }
    else if(game::KEYBOARD[SDL_SCANCODE_UP])
    {
      camera::cam.move_y += hero::hero.speed;
      entity::entities.at(hero::hero.entity_id).side_id = ANIM_SIDE_BACK;
      anims::start(ANIM_UNARMED_WALK_FORWARD, hero::hero.entity_id);
      //hero::animate_items(ANIM_MOVE_UP);
    }
    else if(game::KEYBOARD[SDL_SCANCODE_DOWN])
    {
      camera::cam.move_y -= hero::hero.speed;
      entity::entities.at(hero::hero.entity_id).side_id = ANIM_SIDE_FRONT;
      anims::start(ANIM_UNARMED_WALK_FORWARD, hero::hero.entity_id);
      //hero::animate_items(ANIM_MOVE_DOWN);
    } else 
    {
      //anims::start(ANIM_IDLE, hero::hero.entity_id);
    }
    if(camera::cam.move_x != 0 | camera::cam.move_y !=0 )
    {

      camera::cam.previous_x = camera::cam.x;
      camera::cam.previous_y = camera::cam.y;
      camera::cam.x += camera::cam.move_x;
      camera::cam.y += camera::cam.move_y;

      hero::update_position(camera::cam.move_x, camera::cam.move_y);
    }

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
          mouse::handle_mouse(event.motion, event.button);
        break;

        case SDL_QUIT:
          game::RUNNING = false;
        break;

        case SDL_KEYDOWN: 
          switch (event.key.keysym.sym)
          { 
            case SDLK_ESCAPE:
              game::switch_scene(SCENE_ID_MAIN_MENU, false);   
            break;
            
            case SDLK_x:
              //hero::attack_state = true;
              //hero::update_frame(ATTACK);
            break; 

            case SDLK_r:
              game::refresh();
            break;

            case SDLK_s:
              saves::save_game();
            break;

            case SDLK_p:
              game::PAUSE = !game::PAUSE;    
            break;

            case SDLK_d:
              game::IS_DEBUG_MODE = !game::IS_DEBUG_MODE;
            break;

            case SDLK_i:
              entity::print_entity_data();
            break;

            case SDLK_l:
              game::LOG_TO_FILES = !game::LOG_TO_FILES;
            break;

            case SDLK_e:
              for(int c=0; c<collisions::near_items.size(); c++)
              {
                items::pickup(collisions::near_items[c]);
                items::put_in_hand(collisions::near_items[c]);
              }
            break;

            case SDLK_q:
              for(int e=0; e<hero::hero.equipped_items.size(); e++)
              {
                items::yeet(hero::hero.equipped_items[e], (hero::hero.x+(e*10)), (hero::hero.y+(e*10)));
              }
              hero::hero.equipped_items.clear();
            break;

            case SDLK_0:
              std::string script_name = "scripts/console.py";
              scripts::execute(script_name);
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
          mouse::handle_mouse(event.motion, event.button);
        break;

        case SDL_QUIT:
          game::RUNNING = false;
        break;

        case SDL_KEYDOWN: 
          switch (event.key.keysym.sym)
          {
            case SDLK_BACKSPACE:
              if(menu::NewGameName.size() > 0)
              {
                menu::NewGameName.pop_back();
                fonts::labels[fonts::NEW_GAME_LABEL_ID].text = menu::NewGameName;
              }
            break;

            case SDLK_RETURN:
              if(menu::_validate_name())
              {
                game::switch_scene(SCENE_ID_DUNGEON_LEVEL_2, SCENE_LOAD_FROM_NEW);  
              }
            break;

            case SDLK_ESCAPE:
              game::switch_scene(SCENE_ID_MAIN_MENU, false);  
            break;
          }

        break;

        case SDL_TEXTINPUT:
          if(menu::_validate_input(event.text.text) && menu::NewGameName.size() < 8)
          {
            menu::NewGameName += event.text.text;
            fonts::labels[fonts::NEW_GAME_LABEL_ID].text = menu::NewGameName;
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
          mouse::handle_mouse(event.motion, event.button);
        break;

        case SDL_QUIT:
          game::RUNNING = false;
        break;

        case SDL_KEYDOWN: 
          switch (event.key.keysym.sym)
          {     
            case SDLK_ESCAPE:
              game::switch_scene(SCENE_ID_MAIN_MENU, false);
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
          mouse::handle_mouse(event.motion, event.button);
        break;

        case SDL_QUIT:
          game::RUNNING = false;
        break;

        case SDL_KEYDOWN: 
          switch (event.key.keysym.sym)
          {     
            case SDLK_ESCAPE:
              game::switch_scene(SCENE_ID_MAIN_MENU, false);
            break;

            // case SDLK_0:
            //   std::string script_name = "scripts/console.py";
            //   scripts::execute(script_name);
            // break;
          }
        break;
      }
    }
  };

  void handle_events(SDL_Event event)
  {
    events::EventsHandler[game::EVENT_HANDLER_ID](event);
  }
}
