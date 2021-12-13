# Mobs
Path: ./modules/mobs.cpp   ./modules/mobs.h


## Objects
### MobData
Single mob type data
```c++
struct MobData
```
### mobs_data
Catalog of mob types
```c++
std::map<int, MobData> mobs_data;
```

### SpawnedMobs
Spawned mobs information
```c++
std::map<int, MobData> SpawnedMobs;
```

## Functions

### read_data
Read [MobData](mobs.md#MobData) to [mobs_data](mobs.md#mobs_data) by name of the type
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
Clear [SpawnedMobs](mobs.md#SpawnedMobs)
```c++
void clear();
```

### spawn_from_nest
Create  mob entities from map
```c++
void spawn_from_nest(int map_id);
```

### spawn
Creates new  mob [entity](entity.md#entities). Adds to [SpawnedMobs](mobs.md#SpawnedMobs)
```c++
void spawn(int mob_type_id, float x, float y);
```

### drop
Drops mob  from [entities](entity.md#entities) and [SpawnedMobs](mobs.md#SpawnedMobs)
```c++
void drop(int entity_id);
```


## Tests
- 