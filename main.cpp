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

  // adding font texture to texture catalog
  textures::TextureData FontTD = fonts::init();
  textures::Catalog.insert({FontTD.opengl_texture_id, FontTD});
  textures::BoundTextures.push_back(FontTD.opengl_texture_id);

  int NEW_GAME = true;
  int MAIN_MENU_ON = true;
  if(NEW_GAME)
  {
    int MAP_ID = 0;
    levels::init(MAP_ID, maps::Catalog[MAP_ID].default_player_x, maps::Catalog[MAP_ID].default_player_y);
  }
  levels::LevelQuads = quads::assign_vertices(levels::LevelQuads);
  std::vector<quads::Quad> text_quads = fonts::render_text("chuj", 200, 300, FontTD);
  // finish temporary
  while(RUNNING)
  {
    auto game_loop_start_time = std::chrono::system_clock::now();
    qm::accumulate(menu::MenuQuads, levels::LevelQuads, fonts::TextQuads);
    // if(MAIN_MENU_ON)
    // {
    //   //levels::update(fonts::TextQuads);
    //   // qm::update(qm::AllQuads);
    //   // levels::update(levels::LevelQuads);
    //   levels::update(fonts::TextQuads);
    // }

    
    SDL_Event event;
    events::handle_events(event, levels::ScaledLevelQuads, levels::LevelQuads);
    // levels::update(levels::LevelQuads);
    levels::update(text_quads);
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