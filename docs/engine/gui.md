# Gui
Path: ./modules/gui.cpp   ./modules/gui.h


## Objects

### GuiSlotData
InGame Gui slot data
```c++
struct GuiSlotData
```

### GuiData
Temporary(it should be menu) - GuiInformation quad
```c++
struct GuiData
```

### Index
Index of GUIs
```c++
std::vector<int> Index;
```

### guislots
Catalog of [gui slots](gui.md#GuiSlotData)
```c++
std::map<int, gui2::GuiSlotData> guislots;
```

### guis
Catalog of [guis](gui.md#GuiData)
```c++
std::map<int, gui2::GuiData> guis;
```

### GuiQuads
Quads made out of the [guis](gui.md#guis). Vector of [quads](quads.md#QuadData)
```c++
std::vector<quads::QuadData> GuiQuads;
```

### display
(Temporary) Catalog of display menu functions based on the object_id
```c++
extern std::map <int,sig_ptr> display;
```


## Functions
### init
Create [guislots](gui.md#guislots) and [display](gui.md#display) 
```c++
void init();
```
### clear
Remove data from [guislots](gui.md#guislots), [guis](gui.md#guis),  [GuiQuads](gui.md#GuiQuads) and  [Index](gui.md#Index)
```c++
void clear();
```
### drop
Drop GUI by id from [guis](gui.md#guis), delete index from [Index](gui.md#Index). Free slot from [guislots](gui.md#guislots). Drops associated [labels](fonts.md#drop) and [buttons](buttons.md#drop)
```c++
void drop(int gui_id);
```
### render
Populates [GuiQuads](gui.md#GuiQuads) from [guis](gui.md#guis)
```c++
void render();
```
### add_context_menu
Find free slot from [guislots](gui.md#guislots), generate new [GuiData](gui.md#GuiData) and add it to [guis](gui.md#guis). Add [labels](fonts.md#add) and [buttons](buttons.md#add)
```c++
int add_context_menu(int object_id, int object_type_id);
```
### _get_free_slot
Find first free gui slot
```c++
int _get_free_slot();
```
### _free_slot
Free gui slot after removing the gui
```c++
int _free_slot();
```


## Tests
- 