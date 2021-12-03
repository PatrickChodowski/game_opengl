# Buttons
Path: ./modules/buttons.cpp   ./modules/buttons.h


## Objects
### ButtonData
Contains information about button
```c++
struct ButtonData
```

### ButtonFunctions
Catalog of buttons and ButtonFunction pointers
```c++
std::map <int,sig_ptr> ButtonFunctions;
```

### Index
Index of button IDS
```c++
std::vector<int> Index;
```

### buttons
Catalog of buttons
```c++
std::map <int, buttons::ButtonData> buttons;
```

### ButtonQuads
Quads made out of buttons. Vector of [quads](quads.md#QuadData)
```c++
std::vector<quads2::QuadData> ButtonQuads;
```

## Functions
### init
Builds [ButtonFunctions](buttons.md#ButtonFunctions). Assigns functionality to button_id
```c++
void init();
```

### add 
Add new [ButtonData](buttons.md#ButtonData) to [buttons](buttons.md#buttons) 
```c++
int add(std::string text, float x, float y, float w, float h, int button_function_id);
```

### render
Populates [ButtonQuads](buttons.md#ButtonQuads) from [buttons](buttons.md#buttons)
```c++
void render();
```

### drop
Drop Button ([remove the label](fonts.md#drop), erase from [buttons](buttons.md#buttons), remove button_id from [Index](buttons.md#Index))
```c++
void drop(int button_id);
```

### clear
Clears [Index](buttons.md#Index), [guibuttons](buttons.md#guibuttons) and [GuiButtonQuads](buttons.md#GuiButtonQuads)
```c++
void clear();
```

### _click_new_game
Functionality for NewGame Button. Calls [game::switch_scene](game.md#switch_scene) to switch to New Game Menu
```c++
void _click_new_game();
```

### _click_load_game
Functionality for LoadGame Button. Calls [game::switch_scene](game.md#switch_scene) to switch to Load Game Menu
```c++
void _click_load_game();
```

### _click_settings
Functionality for Settings Button. Calls [game::switch_scene](game.md#switch_scene) to switch to Settings Menu
```c++
void _click_settings();
```

### _click_exit
Functionality for Exit Button  sets [game::RUNNING](game.md#RUNNING) to false
```c++
void _click_exit();
```

### _click_newgame_name
Functionality for NewGame(name) Button. Calls [game::switch_scene](game.md#switch_scene) to start new game
```c++
void _click_newgame_name();
```

### _click_back
Functionality for Click Back Button. Calls [game::switch_scene](game.md#switch_scene) to go back to main menu
```c++
void _click_back();
```

### _button_travel
Functionality of the Travel Button. Starts [Travel Object](travel.md#TravelData)
```c++
void _button_travel(int object_id);
```


## Tests
-

