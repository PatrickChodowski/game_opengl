
## Entities

### Info
Contains information about current entities (hero, mobs, items etc.) loaded into memory. Table is initialized on game start

### Format
**std::map<EntityID; EntityData>** entities;

### Entity Data Struct

```c++

  struct EntityData
  {
    int entity_id;
    int x;
    int y;
    int texture_id;
    int frame_id;
    int event_id;
    int status;
  };


```


### Add record to table
```c++
int entity::spawn();
```

### Update record in the table
```c++
int entity::update();
```

### Delete record from the table
```c++
int entity::kill();
```

### Clear table
```c++
void entity::clear();
```
