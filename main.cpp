#include <iostream>
#include <SDL2/SDL.h>
#include <chrono>
#include <ctime> 

#include "modules/collisions.h"
#include "modules/events.h"
#include "modules/game.h"
#include "modules/hero.h"
#include "modules/utils.h"
#include "modules/timer.h"
#include "modules/travel.h"

#include "dictionary.h"

int FPS;
Uint32 flags = SDL_WINDOW_OPENGL;

int main()
{
  SDL_Init(SDL_INIT_VIDEO);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_Window *WINDOW = SDL_CreateWindow("Pogromcy Karaluchow",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        game::WINDOW_WIDTH,
                                        game::WINDOW_HEIGHT,
                                        flags);
  SDL_GLContext GLCONTEXT = SDL_GL_CreateContext(WINDOW);
  Uint32 current_sdl_gl = SDL_GL_MakeCurrent(WINDOW, GLCONTEXT);
  SDL_GL_SetSwapInterval(1);
  glViewport(0, 0, game::WINDOW_WIDTH, game::WINDOW_HEIGHT);
  GLenum err = glewInit();
  utils::check_glew(err);
  game::init();

  while(game::RUNNING)
  {
    auto game_loop_start_time = std::chrono::high_resolution_clock::now();
    SDL_Event event;
    events::handle_events(event);
    // Mobs/NPCs movements here?
    travel::update();
    collisions::handle_entity_collisions(hero::hero.entity_id);
    game::update();
    SDL_GL_SwapWindow(WINDOW);
    auto game_loop_end_time = std::chrono::high_resolution_clock::now();
    float delay = timer::get_delay_ms(game_loop_start_time, game_loop_end_time);
    SDL_Delay(delay);
    FPS = timer::get_fps_delay(game_loop_start_time, game_loop_end_time, delay);
  }

  game::drop();
  SDL_GL_DeleteContext(GLCONTEXT); 
  SDL_DestroyWindow(WINDOW);
  SDL_Quit();
  return 0;
}
