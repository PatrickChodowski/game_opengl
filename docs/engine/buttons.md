# Buttons
Path: ./modules/buttons.cpp   ./modules/buttons.h


## Objects
### GuiButtonData
Contains information about button
```c++
struct GuiButtonData
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

### guibuttons
Catalog of GuiButtons
```c++
std::map <int, buttons::GuiButtonData> guibuttons;
```

### GuiButtonQuads
Quads made out of GuiButtons. Vector of [quads](quads.md#QuadData)
```c++
std::vector<quads2::QuadData> GuiButtonQuads;
```

## Functions
### init
Builds [ButtonFunctions](buttons.md#ButtonFunctions). Assigns functionality to button_id
```c++
void init();
```

### add 
Add new [GuiButtonData](buttons.md#GuiButtonData) to [guibuttons](buttons.md#guibuttons) 
```c++
int add(std::string text, float x, float y, int button_function_id);
```

### render
Populates [GuiButtonQuads](buttons.md#GuiButtonQuads) from [guibuttons](buttons.md#guibuttons)
```c++
void render();
```

### drop
Drop Button ([remove the label](fonts.md#drop), erase from [guibuttons](buttons.md#guibuttons), remove button_id from [Index](buttons.md#Index))
```c++
void drop(int button_id);
```

### clear
Clears [Index](buttons.md#Index), [guibuttons](buttons.md#guibuttons) and [GuiButtonQuads](buttons.md#GuiButtonQuads)
```c++
void clear();
```

### _button_travel
Functionality of the Travel Button. Starts [Travel Object](travel.md#TravelData)
```c++
void _button_travel(int object_id);
```


## Tests
-

