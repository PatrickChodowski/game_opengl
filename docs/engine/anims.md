# Anims
Path: ./modules/anims.cpp   ./modules/anims.h


## Objects

### Animation
Animation Data struct
```c++
struct Animation
```

### anims
Catalog of all available animations
```c++
std::map<int, anims::Animation> anims;
```

### animsplayed
Catalog of all active animations
```c++
std::map<int, anims::Animation> animsplayed;
```

### anims_to_stop
List of animations to stop
```c++
std::vector<int> anims_to_stop;
```

## Functions

### init
Reads in all the data
```c++
  void init();
```

### read_data
Reads data for selected animation
```c++
void read_data(std::string& name);
```

### clear
Clear temporary data
```c++
void clear();
```

### update
Updates all current animations played
```c++
void update();
```

### drop
Delete animation from current anims
```c++
void drop(int entity_id);
```

### refresh
Clear permanent data
```c++
void refresh();
```

### _check_if_entity_in_anim
Checks if given entity already is playing animation
```c++
bool _check_if_entity_in_anim(int entity_id);
```

### _check_if_entity_in_anim_same_type
Checks if given entity has animation and if this is the same type
```c++
bool _check_if_entity_in_anim_same_type(int anim_type_id, int entity_id);
```

### start
Start animation for given entity
```c++
void start(int anim_type_id, int entity_id);
```

### play
Runs the single animation from animsplayed
```c++
void play(int entity_id);
```


## Tests
- 
