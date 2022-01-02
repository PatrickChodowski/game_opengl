# Items
Path: ./modules/items.cpp   ./modules/items.h


## Objects

### ItemData
Struct of all necessary item attributes
```c++
struct ItemData
```

### GeneratedItemData
Struct of all necessary set item attributes
```c++
struct GeneratedItemData
```

### GeneratedItems
Catalog of generated items [GeneratedItemData](items.md#GeneratedItemData)
```c++
phmap::flat_hash_map<int, GeneratedItemData> GeneratedItems;
```

### items
Catalog of item types [ItemData](items.md#ItemData)
```c++
phmap::flat_hash_map<int, ItemData> items;
```

### ItemsOnGround
Catalog of generated items that are dropped on the ground [GeneratedItemData](items.md#GeneratedItemData)
```c++
phmap::flat_hash_map<int, GeneratedItemData> ItemsOnGround;
```

### EquippedItems
Catalog of generated items that are equipped by the hero [GeneratedItemData](items.md#GeneratedItemData)
```c++
phmap::flat_hash_map<int, GeneratedItemData> EquippedItems;
```

## Functions
### init
Populate [items](items.md#items) with data
```c++
void init();
```

### read_data
Read the [ItemData](items.md#ItemData) and add it to [items](items.md#items)
```c++
void read_data(std::string name);
```

### spawn
Generates the item - adds to [GeneratedItems](items.md#GeneratedItems)
```c++
void spawn(int item_id, float x, float y);
```

### put_in_hand
Renders in hand item
```c++
void put_in_hand(int entity_id);
```

### pickup
Picks up item from the ground
```c++
void pickup(int entity_id);
```
### yeet
Drops item on the ground
```c++
void yeet(int entity_id, float x, float y);
```

### refresh
Resets the data read in from files in the game
```c++
void refresh();
```

### info
Returns vector of strings with item information
```c++
std::vector<std::string> info(int entity_id);
```



## Tests
-
