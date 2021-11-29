# Collisions
Path: ./modules/collisions.cpp   ./modules/collisions.h

## Objects

### AABB
AABB box collision box with min_x, min_y, max_x,max_y and id
```c++
struct AABB
```

### AABBsHandler
Catalog of functions for setting AABB per object_type
```c++
std::map <int,sig_ptr> AABBsHandler;
```

### Sensor
Sensor point with x,y and id
```c++
struct Sensor
```

### SolidLimits
Struct of vectors of borders on left, right, top, bottom
```c++
struct SolidLimits
```

### DistanceToObject
Struct of the distance from entity_id to another object
```c++
struct DistanceToObject
```

### SENSOR_COUNT 
Global count of sensors per entity
```c++
int SENSOR_COUNT
```

### SENSOR_OFFSET
Global sensor offset
```c++
int SENSOR_OFFSET
```

### ABS_COUNT
Global AABBs count  per entity
```c++
int ABS_COUNT
```

## Functions

### init
Fill out AABBsHandler
```c++
void init();
```

### handle_entity_collisions
Handle collisions for  [entity](entity.md#Entity)
```c++
void handle_entity_collisions(int entity_id);
```

### _set_sensors
Function to set [sensors](collisions.md#Sensor) for [entity](entity.md#Entity) for which we are checking collisions
```c++
void _set_sensors(int entity_id);
```

### _set_abs
Function to set [AABBs](collisions.md#AABB) for [entities](entity.md#Entity) and [map objects](maps.md#Map) that are within near distance
```c++
void _set_abs(std::vector<collisions::DistanceToObject>& near_distances);
```

### _set_abs_maps
Function to set [AABBs](collisions.md#AABB) for [map object](maps.md#Map)
```c++
void _set_abs_maps(int entity_id);
```

### _set_abs_entities
Function to set [AABBs](collisions.md#AABB) for [entity](entity.md#Entity)
```c++
void _set_abs_entities(int entity_id);
```

### _find_entity_broad_collisions
Function to retrieve [all near distances](collisions.md#DistanceToObject) from [entity](entity.md#Entity) to all collidables (maps and entities)
```c++
std::vector<collisions::DistanceToObject> _find_entity_broad_collisions(int entity_id);
```

### _resolve_solid_collisions
Resolves collisions for [all near distances](collisions.md#DistanceToObject) from [entity](entity.md#Entity) 
```c++
void _resolve_solid_collisions(std::vector<collisions::DistanceToObject>& near_distances);
```

### _get_entity_to_entity_distances
Function to retrieve [near distances](collisions.md#DistanceToObject) from [entity](entity.md#Entity) to another entities
```c++
std::vector<collisions::DistanceToObject> _get_entity_to_entity_distances(int entity_id);
```

### _get_entity_to_map_distances
Function to retrieve [near distances](collisions.md#DistanceToObject) from [entity](entity.md#Entity)to [map object](maps.md#Maps)
```c++
std::vector<collisions::DistanceToObject> _get_entity_to_map_distances(int entity_id);
```

## Tests
-

