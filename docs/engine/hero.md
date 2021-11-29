# Hero
Path: ./modules/hero.cpp   ./modules/hero.h



## Objects
### HeroData
Struct with all data needed for the hero. Used to create singleton [hero::hero](hero.md#hero)
```c++
struct HeroData
```

### HeroData
Singleton created from [HeroData struct](hero.md#HeroData)
```c++
hero::HeroData hero;
```

## Functions
### create_new
Creates new hero character
```c++
void create_new(std::string name, std::string type);
```

### revert_position_x
Revert to previous hero position x
```c++
void revert_position_x();
```

### revert_position_y
Revert to previous hero position y
```c++
void revert_position_y();
```
  
### set_position
Sets arbitrary position of the hero
```c++
void set_position(float x, float y);
```

### update_position
Updates position of the hero character in the ehro data and entity data
```c++
void update_position(float camera_move_x, float camera_move_y);
```

### _load_from_save
Loads data to [hero::hero](hero.md#hero) from selected save_name
```c++
void _load_from_save(std::string save_name);
```

### _read_data
Reads data from specified character type (like barbarian)
```c++
void _read_data(std::string char_type);
```


## Tests
-