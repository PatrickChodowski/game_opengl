
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
  fonts::init_font();
  fonts::no_idea();

  // temporary, want to load level
  // if new level then
  if(START_WITH_MENU)
  {
    logger::print("here will be menu init + render");
    menu::init();
  }

  int NEW_GAME = false;
  int MAIN_MENU_ON = true;
  if(NEW_GAME)
  {
    int MAP_ID = 0;
    levels::init(MAP_ID, maps::Catalog[MAP_ID].default_player_x, maps::Catalog[MAP_ID].default_player_y);
  }
  // finish temporary

  fonts::load_text_quads("Test tekst", 200, 300, 1.0, 1.0, 1.0, 1.0);
  // main game loop
  while(RUNNING)
  {
    // clearing all texts before assigning new ones
    fonts::clear_text();
    auto game_loop_start_time = std::chrono::system_clock::now();
    qm::accumulate(menu::MenuQuads, levels::LevelQuads, fonts::TextQuads);
    if(MAIN_MENU_ON)
    {
      levels::update(menu::MenuQuads);
    }

    SDL_Event event;
    events::handle_events(event, levels::ScaledLevelQuads, levels::LevelQuads);

    // levels::update(levels::LevelQuads);
    SDL_GL_SwapWindow(WINDOW);
    SDL_Delay(1000/60);


    timer::print_elapsed_time(game_loop_start_time, "Game Loop duration:");
  }

  // cleanup after the game
  textures::drop();
  shaders::drop();
  buffer::drop();
  SDL_GL_DeleteContext(GLCONTEXT); 
  SDL_DestroyWindow(WINDOW);
  SDL_Quit();
  return 0;
}