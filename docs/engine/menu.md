# Menu
Path: ./modules/menu.cpp   ./modules/menu.h

## Objects

### MenuData
Menu data -> id and buttons
```c++
struct MenuData
```

### MenuSlotData
Menu Slot Data -> information about in game menu slot
```c++
struct MenuSlotData
```

### saves
List of available saves in /saves folder
```c++
std::vector<std::string> saves;
```

### saves_buttons_map
Mapping of buttons to the save names
```c++
phmap::flat_hash_map<int, std::string> saves_buttons_map;
```

### menus
Current Menu's data
```c++
phmap::flat_hash_map<int, menu2::MenuData> menus;
```
### currentmenus
Current menu data
```c++
phmap::flat_hash_map<int, MenuData> currentmenus;
```

### currentmenuslots
Current menu slots data
```c++
phmap::flat_hash_map<int, menu::MenuSlotData> currentmenuslots = {}; 
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

### Index
Index of current menus
```c++
std::string Index;
```

## Functions

### init
Reads in all menu's data, all menu slot_data and lists saves
```c++
void init();
```

### add
Adds menu into currentmenus by menu_type_id, creates button and labels objects.
Returns unique menu_id
```c++
int add(int menu_type_id)
```

### add_to_slot
Add menu to free slot
```c++
int add_to_slot(int menu_type_id, int object_id);
```

### drop
Drops menu from [currentmenus](menu.md#currentmenus) by menu_id, removes assigned [buttons](buttons.md#ButtonData) and [labels](fonts.md#LabelData). Frees the menu slot. 
```c++
void drop(int menu_id)
```

### render
Populates [MenuQuads](menu.md#MenuQuads) from [currentmenus](menu.md#currentmenus)
```c++
void render();
```

### list_saves
Populates [saves](menu.md#saves)
```c++
void _list_saves();
```

### clear
Clears Index, MenuQuads, currentmenus, currentmenuslots, NewGameName, saves_buttons_map
```c++
void clear();
```

### refresh
Resets the data read in from files in the game
```c++
void refresh();
```

### read_menu_data
Reads in data files to [MenuData objects](menu.md#MenuData)
```c++
void read_menu_data(std::string& name);
```

### read_menu_slot_data
Reads in data files to [MenuSlotData objects](menu.md#MenuSlotData)
```c++
void read_menu_slot_data(std::string& name);
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

### _get_free_slot
Returns minimum free menu slot id
```c++
int _get_free_slot();
```

### _free_slot
Free slot occupied by given menu_id
```c++
void _free_slot(int menu_id);
```

## Tests
- 
