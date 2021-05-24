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

  // reads-in all textures and maps data into catalogs
  textures::init();
  maps::init();

  // temporary, want to load level
  // if new level then
  int NEW_GAME = true;
  if(NEW_GAME)
  {
    int MAP_ID = 0;
    levels::load(MAP_ID, maps::Catalog[MAP_ID].default_player_x, maps::Catalog[MAP_ID].default_player_y);
    
  }
  // finish temporary




  // main game loop
  while(RUNNING)
  {
    SDL_Event event;
    events::handle_events(event);
  }

  return 0;
}