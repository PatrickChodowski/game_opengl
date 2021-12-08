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

### ExternalConfigData
Struct for initializing external game config data
```c++
struct ExternalConfigData
```

### Config
Instance of config
```c++
game::ExternalConfigData Config;
```

### GAME_START_TIME
Filled on the start of the game
```c++
std::chrono::time_point<std::chrono::high_resolution_clock> GAME_START_TIME;
```

### SceneData
Data about a scene (scene is anything visible on the screen - menu, entities, map etc.)
```c++
struct SceneData 
```

### EVENT_HANDLER_ID 
Current event handler id
```c++
int EVENT_HANDLER_ID;
```

### MAP_ID 
Current map id
```c++
int MAP_ID;
```

### HERO_START_X 
Current hero start x
```c++
float HERO_START_X;
```

### HERO_START_Y 
Current hero start y
```c++
float HERO_START_Y;
```

### scenes 
Scenes catalog
```c++
std::map<int, game::SceneData> scenes;
```


## Functions

### init
Initialize all modules having **::init()** method and also [game::init_scene](game.md#init_scene) with defualt scene_id
```c++
void init();
```

### read_data
Reads [scene data](game.md#SceneData) to struct
```c++
void read_data(std::string &name);
```

### init_scenes
Reads all scenes data to scenes catalog
```c++
void init_scenes();
```

### update
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

```c++
void update();
```

### drop
Calls ::drop method on every module that requires it - mostly the OpenGL communicating modules -> buffer, shaders, textures
```c++
void drop();
```

### refresh
Calls ::refresh method on every module that requires it to reset the data read in from files in the game
```c++
void refresh();
```

### init_scene
Starts new scene based on scene_id and on from_save boolean. If scene_id is in_game scene, it will initialize map and spawn entities.
```c++
void load_scene(int scene_id, bool from_save);
```

### switch_scene
Clears current scene with game::clear_scene and initializes new scene
```c++
void switch_scene(int scene_id, bool is_new_game);
```

### clear_scene
Calls ::clear() method on every module that requires it. Clear() methods are supposed to clean the Permanent and Temporary data objets -> all Indexes, Catalogs etc.
```c++
void clear_scene();
```

### read_config
Reads in [config](game.md#Config)
```c++
void read_config(std::string& config_path);
```

## Tests
-