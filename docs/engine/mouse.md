# Mouse
Path: ./modules/mouse.cpp   ./modules/mouse.h


## Objects
Game module contains set of Global variables used to control the flow and state of the game

### MouseData
Struct of mouse click data (position, timestamp)
```c++
struct MouseData
```

### LastClickData
Struct of mouse last click for mouse - containing world's x,y and window's x,y
```c++
struct LastClickData
```


### ClickData
Struct of mouse click on object -> object_id, quad_id, object_type, mouse_button, priority
```c++
struct ClickData
```

### last_click
Instance of [LastClickData](mouse.md#LastClickData)
```c++
float last_click_x
```



### ClickPriorities
Table of clickpriorities per object_type
```c++
phmap::flat_hash_map<int, int> ClickPriorities;
```

### click
Catalog of object_type_id and functions pointers to pick right function while clicking
```c++
phmap::flat_hash_map<int, sig_ptr> click;
```

## Functions
### init
Initializes [ClickPriorities](mouse.md#ClickPriorities) and [click](mouse.md#click)
```c++
void init();
```

### print_mouse
Prints information about the mouse click
```c++
void print_mouse(SDL_MouseMotionEvent e, const char* name);
```

### handle_mouse
Event handle of different mouse clicks
```c++
void handle_mouse(SDL_MouseMotionEvent e, SDL_MouseButtonEvent b);
```

### _find_clicked_quads
Search for the clicked quads from [quads::AllQuads](quads.md#AllQuads) and select them based on priority
```c++
void _find_clicked_quads(float click_x, float click_y, int mouse_button_id);
```

### _click_menu
Function triggered when clicking on [menu object](menu.md#Menu)
```c++
void _click_menu(int object_id, int mouse_button_id);
```

### _click_entity
Function triggered when clicking on [entity object](entity.md#Entity)
```c++
void _click_entity(int object_id, int mouse_button_id);
```

### _click_map
Function triggered when clicking on [map object](maps.md#Map)
```c++
void _click_map(int object_id, int mouse_button_id);
```

### _click_button
Function triggered when clicking on [buttons object](buttons.md#Buttons)
```c++
void _click_button(int object_id, int mouse_button_id);
```



## Tests
-

