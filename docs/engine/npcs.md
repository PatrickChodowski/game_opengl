# NPCs
Path: ./modules/npcs.cpp   ./modules/npcs.h


## Objects

### NPCData
Struct of all necessary NPC type attributes
```c++
struct NPCData
```

### InteractionData
Struct of all necessary interaction data
```c++
struct InteractionData
```

### npcs_data
Catalog of all [npcs](npcs.md#NPCData)
```c++
phmap::flat_hash_map<int, NPCData> npcs_data;
```

### npcs
Current [npcs](npcs.md#NPCData)
```c++
phmap::flat_hash_map<int, NPCData> npcs;
```

### interactions
Table of history of [interactions](npcs.md#InteractionData)
```c++
std::vector<InteractionData> interactions;
```


## Functions

### read_data
Read [npcs](npcs.md#NPCData) data from the file to [npcs_data](npcs.md#npcs_data)
```c++
void read_data(std::string& name);
```

### init
Read all [npcs](npcs.md#NPCData) data files to [npcs_data](npcs.md#npcs_data)
```c++
void init();
```

### spawn
Spawn npc, adds instance to [npcs](npcs.md#npcs). Returns entity id
```c++
int spawn(int npc_id, float x, float y); 
```

### spawn_from_map
Spawn group of npcs assigned to the map
```c++
void spawn_from_map(int map_id);
```

### clear
Clear temporary data -> [npcs](npcs.md#npcs)
```c++
void clear();
```

### refresh
Clear catalog -> [npcs_data](npcs.md#npcs_data)
```c++
void refresh();
```

### interact
Make interaction with NPC
```c++
 void interact(int entity_id, float value);
```

### info
Returns vector of strings with npc information
```c++
std::vector<std::string> info(int entity_id);
```


## Tests
-
