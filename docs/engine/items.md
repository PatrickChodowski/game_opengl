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
std::map<int, GeneratedItemData> GeneratedItems;
```

### items
Catalog of item types [ItemData](items.md#ItemData)
```c++
std::map<int, ItemData> items;
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

### put_item_on_ground
Generates the item - adds to [GeneratedItems](items.md#GeneratedItems)
```c++
void put_item_on_ground(int item_id, float x, float y);
```

### refresh
Resets the data read in from files in the game
```c++
void refresh();
```



## Tests
-
