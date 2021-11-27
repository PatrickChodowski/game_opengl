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

### clear
Clear [mobs](mobs.md#mobs)
```c++
void clear();
```

### spawn
Create new [entity](entity.md#entities) of mob type
```c++
void spawn(int map_id);
```


## Tests
- 