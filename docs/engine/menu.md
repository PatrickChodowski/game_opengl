# Menu
Path: ./modules/menu.cpp   ./modules/menu.h

This module probably will be refactored to use buttons module. Menu will become just a GUI object -> set of buttons and labels

## Objects

### ButtonData
(Temporary) Button Data for menu
```c++
struct ButtonData
```

### MenuData
(Temporary) Menu data -> id and buttons
```c++
struct MenuData
```

### ClickButton
(Temporary) Handler for different buttons 
```c++
std::map <int,sig_ptr> ClickButton;
```

### saves
List of available saves in /saves folder
```c++
std::vector<std::string> saves;
```

### menus
(Temporary) Menu's data
```c++
std::map<int, menu2::MenuData> menus;
```

### menubuttons
(Temporary) all menubuttons logic
```c++
std::map<int, menu2::ButtonData> menubuttons;
```

### CurrentMenuButtons
(Temporary) Menu buttons currently displayed
```c++
std::map<int, menu2::ButtonData> CurrentMenuButtons;
```

### MenuQuads
(Temporary) Quads made out of the CurrentMenuButtons. Vector of [quads](quads.md#QuadData)
```c++
std::vector<quads2::QuadData> MenuQuads;
```

### NewGameName
New game name string
```c++
std::string NewGameName;
```


## Functions

### init
Initialize [ClickButton](menu.md#ClickButton), read menu button data, read menu data, list saves
```c++
void init();
```

### load
Load [CurrentMenuButtons](menu.md#CurrentMenuButtons) based on the scene_id, reads  all the necessery buttons from menu data and load them to CurrentMenuButtons container
```c++
void load(int scene_id);
```

### render
Populates [MenuQuads](menu.md#MenuQuads) from [CurrentMenuButtons](menu.md#CurrentMenuButtons)
```c++
void render();
```

### list_saves
Populates [saves](menu.md#saves)
```c++
void list_saves();
```

### clear
Clears MenuQuads, CurrentMenuButtons and NewGameName
```c++
void clear();
```

### _read_button_data
Reads in data files to [ButtonData objects](menu.md#ButtonData)
```c++
void _read_button_data(std::string& name);
```

### _read_menu_data_
Reads in data files to [MenuData objects](menu.md#MenuData)
```c++
void _read_menu_data_(std::string& name);
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

### _validate_input
Validates new game name input
```c++
bool _validate_input(std::string input);
```

### _validate_name
Validates full new game name
```c++
bool _validate_name();
```

### _check_if_load_game
Checks if given button loads a saved game (might be temporary)
```c++
int _check_if_load_game(int button_id);
```

## Tests
- 
