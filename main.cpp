#include "setup.h"

// https://dokipen.com/page/2/

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



  int NEW_GAME = false;
  if(NEW_GAME)
  {
    maps::init_map(MAP_ID, maps::Catalog[MAP_ID].default_player_x, maps::Catalog[MAP_ID].default_player_y);
    // fonts::render_text("OpenGl Game Demo", 500, 100, textures::FontTD, 1.0f, 0.0f, 0.0f, 0.0f);
    NEW_GAME = false;
  } else {
    menu::load_menu({0,1,2,3});
    MAIN_MENU_ON = true;
  }
  quads::accumulate();
  buffer::init(quads::AllQuads);

  // finish temporary
  while(RUNNING)
  {
    auto game_loop_start_time = std::chrono::system_clock::now();
    SDL_Event event;
    events::handle_events(event, quads::ScaledAllQuads, quads::AllQuads);
    quads::accumulate();
    game::update(quads::AllQuads);
    
    SDL_GL_SwapWindow(WINDOW);
    SDL_Delay(1000/60);
    timer::print_elapsed_time(game_loop_start_time, "Game Loop duration:");
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