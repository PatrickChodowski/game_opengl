// Setup file with all globals, settings, states etc.
// Moved from main.cpp just to clear the file
// included in main.cpp

// Packages
#include <iostream> // adds std::cout, std::endl
#include <SDL2/SDL.h> // SDL is needed to initialize Opengl context on any platform
#include <string>
#include <vector>
#include <map>
#include <fstream>  // for reading in the json files
#include <sstream>  // for reading in the json files
#include <dirent.h> // filesystem, listing files
#include <signal.h> // for the GlCall, in opengl utils
#include <chrono>   // time/clock
#include <ctime>    // time/clock
#include <regex>    // for shader parameters
#include <algorithm>

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

// logging level
int LOGGING = 0;

// SDL setup
Uint32 flags = SDL_WINDOW_OPENGL;
const Uint8 *KEYBOARD = SDL_GetKeyboardState(NULL);
bool RUNNING = true;
int CURRENT_SHADER_ID = 0;
bool MAIN_MENU_ON = false;
std::string FONT_NAME = "OpenSans";
int MAP_ID = 0;

//https://github.com/jorgen/json_struct
#include "dependencies/json_struct.h" // used for reading in texture sheets into catalog of textures meta
#define STB_IMAGE_IMPLEMENTATION
#include "dependencies/stb_image.h" // for reading-in images 



// glm includes
#include "dependencies/glm/glm.hpp"
#include "dependencies/glm/mat4x4.hpp"
#include "dependencies/glm/ext/matrix_transform.hpp"
#include "dependencies/glm/gtc/matrix_transform.hpp"
#include "dependencies/glm/gtc/type_ptr.hpp"
#include "dependencies/glm/gtx/string_cast.hpp"


// freetype includes
#include <ft2build.h>
#include FT_FREETYPE_H  

// Variables
#include "variables.h"

// Utils
#include "utils/logger.h"
#include "utils/utils.h"
#include "utils/timer.h"


// OpenGL
#include "opengl/utils.h"
#include "opengl/textures.h"
#include "opengl/shaders.h"
#include "opengl/quads.h"
#include "opengl/camera.h"
#include "opengl/buffer.h"
#include "opengl/mouse.h"



// game components
#include "components/items.h"
#include "components/mobs.h"
#include "components/hero.h"

// quads dependent
#include "fonts/fonts.h"
#include "menu/menu.h"
#include "maps/maps.h"

#include "opengl/events.h"

#include "game.h"


// Program setup, but has to be inside a function, called from main.cpp
void initialize_opengl_context()
{
  SDL_Init(SDL_INIT_VIDEO);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
}

