# Entity
Path: ./modules/entity.cpp   ./modules/entity.h

## Objects

### EntityData
Struct of all needed Entity Attributes
```c++
struct EntityData
```

### entities
Catalog of [Entity Data](entity.md#EntityData)
```c++
phmap::flat_hash_map<int, EntityData> entities;
```

### EntityQuads
Quads made out of the [entities](entity.md#entities). Vector of [quads](quads.md#QuadData)
```c++
std::vector<quads::QuadData> EntityQuads;
```

### Index
Index of unique Entity IDs
```c++
std::vector<int> Index;
```

### menu_entity_type_map
Assigning different info function based on the entity type
```c++
typedef std::vector<std::string> (*sig_ptr)(int entity_id);
phmap::flat_hash_map<int, sig_ptr> menu_entity_type_map;
```


## Functions
### create
Create new [Entity Data](entity.md#EntityData) and add it to [entities](entity.md#entities)
```c++
template <typename T>
int create(T data, int entity_type_id, float camera_type);
```

### render
Populates [EntityQuads](entity.md#EntityQuads) from [entities](entity.md#entities)
```c++
void render();
```

### clear
Clears [EntityQuads](entity.md#EntityQuads), [Index](entity.md#Index), [entities](entity.md#entities)
```c++
void clear();
```

### drop
Delete entity from  [entities](entity.md#entities) and [Index](entity.md#Index)
```c++
void drop(int entity_id);
```

### update_frame
Update frame and normalization coordinates of the frame for entity
```c++
void update_frame(int entity_id, int frame_id);

```

### update_position
Update position of entity
```c++
void update_position(int entity_id, float x, float y);

```

### info
Returns vector of information strings (id, position etc.)
```c++
std::vector<std::string> info(int entity_id);

```

### hide
Hide entity (delete from the entities table, but keep entity_id)
```c++
void hide(int entity_id);

```


## Tests
-
