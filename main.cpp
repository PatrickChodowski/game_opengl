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

  // temporary, want to load level
  // if new level then
  int NEW_GAME = true;
  if(NEW_GAME)
  {
    int MAP_ID = 0;
    levels::init(MAP_ID, maps::Catalog[MAP_ID].default_player_x, maps::Catalog[MAP_ID].default_player_y);
  }
  // finish temporary
  // problems seems to be on texture reading
  // is it not bound

    textures::init();
    textures::bind(0,0);



  // main game loop
  while(RUNNING)
  {
    SDL_Event event;
    events::handle_events(event);
    // buffer update here
    levels::update();


    SDL_GL_SwapWindow(WINDOW);
    SDL_Delay(1000 / 60);
  }

  textures::drop(0);
  shaders::drop();
  buffer::drop();
  SDL_GL_DeleteContext(GLCONTEXT); 
  SDL_DestroyWindow(WINDOW);
  SDL_Quit();
  return 0;
}