# Travel
Path: ./modules/travel.cpp   ./modules/travel.h


## Objects

### TravelData
TravelData object being created on start of the entity travelling to new location on the map
```c++
struct TravelData
```

### TravelPoint
Location(x,y)
```c++
struct TravelPoint
```

### travels
Catalog of [TravelData](travel.md#TravelData). One per entity
```c++
std::map<int, travel::TravelData> travels;
```

### travels_to_cancel
Vector of IDs of  [TravelData](travel.md#TravelData) that needs to be removed
```c++
std::vector<int> travels_to_cancel;
```


## Functions

### make_basic_plan
Make plan of the travel between start node and end node. Calls [pathfinder::find_path](pathfinder.md#find_path)
```c++
travel::TravelData make_basic_plan(int current_node_id, int target_node_id);
```

### go
Calls [travel::make_basic_plan](travel.md#make_basic_plan), then calculates distance and angle to the next point, finds next gate and updates the position of entity
```c++
void go(travel::TravelData tp);
```

### update
Manages the [travels](travel.md#travels). Calls [go](travel.md#go) on each [TravelData](travel.md#TravelData) and cancels finished or broken travels
```c++
void update();
```

### drop
Deletes [TravelData](travel.md#TravelData) of [travels](travel.md#travels) by travel_id
```c++
void drop(int travel_id);
```

### clear
Remove data from [travels](travel.md#travels) and [travels_to_cancel](travel.md#travels_to_cancel)
```c++
void clear();
```

### _get_angle_between_points
Calculates angle between 2 points
```c++
float _get_angle_between_points(float e_x, float e_y, float p_x, float p_y);
```

### _get_nearest_point_on_line
Finds nearest point on target NavGate
```c++
travel::TravelPoint _get_nearest_point_on_line(float l_x1, float l_y1, float l_x2, float l_y2, float p_x, float p_y);
```

### _sanity_check_replace_tp
Checks if current entity position is on correct node (node in travel) updates if its not
```c++
int _sanity_check_replace_tp(float x, float y, int tp_current_node_id);
```

### _check_if_entity_already_moving
Checks if entity has already a travelData assigned
```c++
bool _check_if_entity_already_moving(int entity_id);
```

## Tests
- check if replaces the TD if needed
- checks if deletes travel if entity is moving
- check angle calculation
- check nearest point of the line calculation

