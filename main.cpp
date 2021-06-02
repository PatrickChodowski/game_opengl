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



  // Init data for Maps, Textures etc.
  
  maps::init();
  shaders::init();
  textures::init();

  // temporary, want to load level
  // if new level then
  int NEW_GAME = true;
  if(NEW_GAME)
  {
    int MAP_ID = 0;
    levels::init(MAP_ID, maps::Catalog[MAP_ID].default_player_x, maps::Catalog[MAP_ID].default_player_y);
  }
  // finish temporary


  // main game loop
  while(RUNNING)
  {
    auto game_loop_start_time = std::chrono::system_clock::now();


    SDL_Event event;
    events::handle_events(event, levels::ScaledLevelQuads, levels::LevelQuads);
    levels::update();
    SDL_GL_SwapWindow(WINDOW);
    SDL_Delay(1000/60);


    timer::print_elapsed_time(game_loop_start_time, "Game Loop duration:");
  }

  textures::drop();
  shaders::drop();
  buffer::drop();
  SDL_GL_DeleteContext(GLCONTEXT); 
  SDL_DestroyWindow(WINDOW);
  SDL_Quit();
  return 0;
}