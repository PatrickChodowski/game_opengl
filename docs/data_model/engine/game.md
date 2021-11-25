# Game
Path: ./modules/game.cpp   ./modules/game.h

Module controlling all other modules in the game. Responsible for correct initializion, de-initialization, clearance and update of all the objects and modules in the game. Constains Global variables controlling state and flow of the game, accessed by set of other modules.


## Objects
Game module contains set of Global variables used to control the flow and state of the game

### PAUSE
Pause will impact certain modules to not update during the frame. False by defualt
```c++
bool PAUSE;
```
### RUNNING
Main loop argument, if true, the game is going on
```c++
bool RUNNING;
```
### IS_DEBUG_MODE
Checks if the game is in DEBUG MODE
```c++
bool IS_DEBUG_MODE;
```
### SCENE_ID
Scene, 0-100 are in game levels, over 100 are main menu, new game menu etc.
```c++
int SCENE_ID;
```
### IS_MENU 
Derivative of the SCENE_ID, keeps 200 for any in-game scene and menu_id for menu's
```c++
int IS_MENU;
```
### CURRENT_SHADER_ID 
Current shader used in the game. I guess that will be removed once I add shaders per object type etc.
```c++
int CURRENT_SHADER_ID;
```
### WINDOW_WIDTH 
Window width is set to 960 by default
```c++
float WINDOW_WIDTH;
```
### WINDOW_HEIGHT 
Window height is set to 768 by default
```c++
float WINDOW_HEIGHT;
```
### KEYBOARD 
Keyboard pointer for the events
```c++
const Uint8 *KEYBOARD;
```
### GAME_START_TIME
Filled on the start of the game
```c++
std::chrono::time_point<std::chrono::high_resolution_clock> GAME_START_TIME;
```


## Functions

### init
```c++
void init();
```
Initialize all modules having **::init()** method and also [game::init_scene](game.md#init_scene) with defualt scene_id

### update
```c++
void update();
```
1) Calls **::render()** of each module having render() method
2) [quads::accumulate](quads.md#accumulate) to get all meshes together into one vector
3) [camera::scale_quads](camera.md#scale_quads) to have scaled parameters of the quads
4) [logger::log_data](logger.md#log_data) to log recent frame to temporary log files
5) [textures::bind](textures.md#bind) to bind correct textures to opengl
6) [buffer::update_quads](buffer.md#update_quads)
7) Opengl calls to clear the screen
8) Create texture sampler
9) Create camera MVP objects
10) Create Lights sources object
11) Set uniform variables for shader
12) Pick shader
13) Draw Quads
14) [buffer::update_lines](buffer.md#update_lines)
15) Draw debug lines


### drop
```c++
void drop();
```
Calls ::drop method on every module that requires it - mostly the OpenGL communicating modules -> buffer, shaders, textures

### init_scene
```c++
void init_scene(int scene_id, bool is_new_game);
```
Starts new scene based on scene_id and on is_new_game boolean. If its new, it will call [hero::create_new()](hero.md#create_new). If scene_id is in_game scene, it will initialize map and spawn entities.


### switch_scene
```c++
void switch_scene(int scene_id, bool is_new_game);
```
Clears current scene with game::clear_scene and initializes new scene

### clear_scene
```c++
void clear_scene();
```
Calls ::clear() method on every module that requires it. Clear() methods are supposed to clean the Permanent and Temporary data objets -> all Indexes, Catalogs etc.


### _check_if_menu
```c++
void _check_if_menu();
```
Updates the game::IS_MENU based on the game::SCENE_ID. If the scene_id is under 100, then it receives 200 value (in_game_scene_id)

## Tests
