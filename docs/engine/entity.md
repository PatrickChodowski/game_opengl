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
std::map<int, EntityData> entities;
```

### EntityQuads
Quads made out of the [entities](entity.md#entities). Vector of [quads](quads.md#QuadData)
```c++
std::vector<quads2::QuadData> EntityQuads;
```

### Index
Index of unique Entity IDs
```c++
std::vector<int> Index;
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

### info
Returns vector of information strings (id, position etc.)
```c++
std::vector<std::string> info(int entity_id);

```

## Tests
-
