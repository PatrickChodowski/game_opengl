#include "setup.h"

int main()
{
  // Window, OpenGL, SDL initiatlization
  initialize_opengl_context();
  SDL_Window *WINDOW = SDL_CreateWindow("Pogromcy Karaluchow",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        WINDOW_WIDTH,
                                        WINDOW_HEIGHT,
                                        flags);
  SDL_GLContext GLCONTEXT = SDL_GL_CreateContext(WINDOW);
  Uint32 current_sdl_gl = SDL_GL_MakeCurrent(WINDOW, GLCONTEXT);
  SDL_GL_SetSwapInterval(1);
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  GLenum err = glewInit();
  utils::check_glew(err);

  game::init_game_states();
  game::init();
  items::init();

  game::set_state("MAIN_MENU");
  menu::load_menu({0,1,2,3});
  quads::accumulate();
  buffer::init(quads::AllQuads);

  while(RUNNING)
  {
    auto game_loop_start_time = std::chrono::system_clock::now();
    SDL_Event event;
    events::handle_events(event, quads::AllQuads);
    game::handle_game_state();
    quads::accumulate();
    quads::scale_move_quads(camera::x, camera::y, camera::zoom);
    nav::scale(camera::x, camera::y, camera::zoom);

    colls::handle();
    game::update(quads::AllQuads);
    
    SDL_GL_SwapWindow(WINDOW);

    auto game_loop_end_time = std::chrono::system_clock::now();
    float delay = timer::get_delay_ms(game_loop_start_time, game_loop_end_time);
    SDL_Delay(delay);
    FPS = timer::get_fps_delay(game_loop_start_time, game_loop_end_time, delay);
  }

  // cleanup after the game
  textures::drop();
  shaders::drop();
  buffer::drop();
  maps::drop_map();
  SDL_GL_DeleteContext(GLCONTEXT); 
  SDL_DestroyWindow(WINDOW);
  SDL_Quit();
  return 0;
}