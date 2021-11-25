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
```c++
void init();
```
Fills out the EventsHandler object with pairs of (scene_id, function pointer) 


### handle_events

```c++
void handle_events(SDL_Event event);
```
Takes the event as argument and passes it to correct function from EventsHandler based on the current [game::IS_MENU](game.md#IS_MENU) variable which contains information if we are in the menu or ingame.


### _handle_menu

### _handle_in_game

### _handle_new_game

### _handle_load_game

### _scan_for_camera_move





## Tests
