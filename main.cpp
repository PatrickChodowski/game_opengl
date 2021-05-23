#include "setup.h"



int main()
{
  initialize_opengl_context();
  SDL_Window *WINDOW = SDL_CreateWindow("Pogromcy Karaluchow",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        WINDOW_WIDTH,
                                        WINDOW_HEIGHT,
                                        flags);

  // main game loop
  while(RUNNING)
  {
    SDL_Event event;
    events::handle_events(event);
  }

  return 0;
}