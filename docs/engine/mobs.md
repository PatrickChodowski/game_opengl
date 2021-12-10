# Mobs
Path: ./modules/mobs.cpp   ./modules/mobs.h


## Objects
### MobData
Single mob type data
```c++
struct MobData
```
### mobs
Catalog of mob types
```c++
std::map<int, MobData> mobs;
```

### spawnedmobs
Spawned mobs information
```c++
std::map<int, MobData> spawnedmobs;
```

### Index
Index of spawned mobs
```c++
std::vector<int> Index;
```

## Functions

### read_data
Read [MobData](mobs.md#MobData) to [mobs](mobs.md#mobs) by name of the type
```c++
void read_data(std::string name);
```

### init
Read in all mob type datas
```c++
void init();
```

### refresh
Resets the data read in from files in the game
```c++
void refresh();
```

### clear
Clear [spawnedmobs](mobs.md#spawnedmobs)
```c++
void clear();
```

### spawn_from_nest
Create new [entity](entity.md#entities) of mob type
```c++
void spawn_from_nest(int map_id);
```


## Tests
- 