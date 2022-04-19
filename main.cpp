#include <chrono>
#include <ctime> 
#include <iostream>
#include <SDL2/SDL.h>
#include <string>

#include "modules/events.h"
#include "modules/game.h"
#include "modules/utils.h"
#include "modules/timer.h"

#include "dictionary.h"

int FPS;
Uint32 flags = SDL_WINDOW_OPENGL;
std::string GAME_CONFIG_PATH = "config.json";

int main()
{ 
  game::read_config(GAME_CONFIG_PATH);
  SDL_Init(SDL_INIT_VIDEO);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_Window *WINDOW = SDL_CreateWindow("Vikings",
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
  float min_delay = 20.0;

  while(game::RUNNING)
  {
    std::chrono::time_point<std::chrono::high_resolution_clock> game_loop_start_time = timer::get_current_hrc_time();
    SDL_Event event;
    events::handle_events(event);
    game::update();
    SDL_GL_SwapWindow(WINDOW);
    std::chrono::time_point<std::chrono::high_resolution_clock> game_loop_end_time = timer::get_current_hrc_time();
    float delay = timer::get_delay_ms(game_loop_start_time, game_loop_end_time);
    if(delay < min_delay){min_delay = delay; std::cout << "delay:" << delay << std::endl;}
    SDL_Delay(delay);
    //FPS = timer::get_fps_delay(game_loop_start_time, game_loop_end_time, delay);
  }

  game::drop();
  SDL_GL_DeleteContext(GLCONTEXT); 
  SDL_DestroyWindow(WINDOW);
  SDL_Quit();

  std::cout << " Worst Delay: " << min_delay << std::endl;
  return 0;
}
