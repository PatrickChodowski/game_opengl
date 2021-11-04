
#include "modules/buffer.h"
#include "modules/camera.h"
#include "modules/collisions.h"
#include "modules/entity.h"
#include "modules/events.h"
#include "modules/hero.h"
#include "modules/maps.h"
#include "modules/mouse.h"
#include "modules/quads.h"
#include "modules/shaders.h"
#include "modules/textures.h"
#include "modules/timer.h"
#include "modules/utils.h"

#include "dictionary.h"

int FPS;
int TILE_DIM = 96;
auto GAME_START_TIME = std::chrono::system_clock::now();
int WINDOW_VERTEX_WIDTH = 10;
int WINDOW_VERTEX_HEIGHT = 8;
int WINDOW_WIDTH = WINDOW_VERTEX_WIDTH*TILE_DIM;
int WINDOW_HEIGHT = WINDOW_VERTEX_HEIGHT*TILE_DIM;
Uint32 flags = SDL_WINDOW_OPENGL;
const Uint8 *KEYBOARD = SDL_GetKeyboardState(NULL);
bool RUNNING = true;

int main()
{
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

  // game::init_game_states();
  // game::init();
  // items::init();

  // game::set_state("MAIN_MENU");
  // menu::load_menu({0,1,2,3});
  // quads::accumulate();
  buffer2::init();

  while(RUNNING)
  {
    auto game_loop_start_time = std::chrono::system_clock::now();
    SDL_Event event;
    events2::handle_events(event);
    // game::handle_game_state();
    quads2::accumulate();

    // game::update(quads::AllQuads);
    
    SDL_GL_SwapWindow(WINDOW);

    auto game_loop_end_time = std::chrono::system_clock::now();
    float delay = timer::get_delay_ms(game_loop_start_time, game_loop_end_time);
    SDL_Delay(delay);
    FPS = timer::get_fps_delay(game_loop_start_time, game_loop_end_time, delay);
  }

  // cleanup after the game
  textures2::drop();
  shaders2::drop();
  buffer2::drop();
  SDL_GL_DeleteContext(GLCONTEXT); 
  SDL_DestroyWindow(WINDOW);
  SDL_Quit();
  return 0;
}
