# Saves
Path: ./modules/saves.cpp   ./modules/saves.h


## Objects

### SaveData
All information that has to be preserved on save
```c++
struct SaveData
```

## Functions
### save_game
Saves game data to [SaveData](saves.md#SaveData) and saves it further to the file
```c++
void save_game();
```

### load_game
Loads game data from file
```c++
void load_game();
```

### _write_save_json
Writes the save json file to saves directory
```c++
void _write_save_json(std::string& str_to_write);
```


## Tests
- 
