// Setup file with all globals, settings, states etc.
// Moved from main.cpp just to clear the file
// included in main.cpp

// Packages
#include <iostream> // adds std::cout, std::endl
#include <SDL2/SDL.h> // SDL is needed to initialize Opengl context on any platform
#include <string>
#include <vector>
#include <map>

// Opengl packages
#include <GL/glew.h> 
#ifdef TARGET_OS_MAC
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#endif

#ifdef __linux__
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif


// Globals (big rule - they cant be modified anywhere)
int TILE_DIM = 96;

// how many quads to show in window
int WINDOW_VERTEX_WIDTH = 10;
int WINDOW_VERTEX_HEIGHT = 8;
int WINDOW_WIDTH = WINDOW_VERTEX_WIDTH*TILE_DIM;
int WINDOW_HEIGHT = WINDOW_VERTEX_HEIGHT*TILE_DIM;

// Global for now, but I think it should depend on map
int MAP_VERTEX_WIDTH = 20;
int MAP_VERTEX_HEIGHT = 16;
int MAP_WIDTH = TILE_DIM * MAP_VERTEX_WIDTH;
int MAP_HEIGHT = TILE_DIM * MAP_VERTEX_HEIGHT;

// logging level
int LOGGING = 0;

// SDL setup
Uint32 flags = SDL_WINDOW_OPENGL;
const Uint8 *KEYBOARD = SDL_GetKeyboardState(NULL);
bool RUNNING = true;


// Components
#include "utils/logger.h"

#include "opengl/camera.h"
#include "opengl/events.h"
// #include "opengl/textures.h"
#include "opengl/shaders.h"

// #include "opengl/quads.h"

#include "dependencies/json.hpp"

using json nlohmann::json;

// Program setup, but has to be inside a function, called from main.cpp
void initialize_opengl_context()
{
  SDL_Init(SDL_INIT_VIDEO);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);


}
