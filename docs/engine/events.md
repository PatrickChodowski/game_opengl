# Events
Path: ./modules/events.cpp   ./modules/events.h

Module handling the user input -> mouse, keyboard. Receives SDL_event and passes updates to other modules if needed. Has different handlers based on the game scene we are in (gameplay, main menu, new game, load game, etc. -> they require different set of keys to have different functionality).

## Objects

### EventsHandler
Events Handler is the map of the function pointers to different events handlers
```c++
std::map <int , sig_ptr> EventsHandler
```


## Functions

### init
Fills out the EventsHandler object with pairs of (scene_id, function pointer) 
```c++
void init();
```


### handle_events
Takes the event as argument and passes it to correct function from EventsHandler based on the current [game::IS_MENU](game.md#IS_MENU) variable which contains information if we are in the menu or ingame.

```c++
void handle_events(SDL_Event event);
```

### _handle_menu
Handles input when in menu: basically mouse click and clicking on Quit
```c++
void _handle_menu(SDL_Event event);
```


### _handle_in_game
Handles input when in game scene: Mouse, Quiting, KeyDown, KeyboardScan
```c++
void _handle_in_game(SDL_Event event);
```


### _handle_new_game
Handles input when in new_game_menu scene: Mouse, quit, name input backspace, name validation, start new game
```c++
void _handle_new_game(SDL_Event event);
```

### _handle_load_game
Handles input when in load_game_menu scene: Mouse, quit, start new game from drop down list of saved games

```c++
void _handle_load_game(SDL_Event event);
```

### _scan_for_camera_move
Scan the [game::KEYBOARD](game.md#KEYBOARD) if any of the arrows (left, right, up, down) are being pressed, and updates the camera accordingly.

```c++
void _scan_for_camera_move());
```


## Tests
- 
