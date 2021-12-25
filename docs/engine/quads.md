# Quads
Path: ./modules/quads.cpp   ./modules/quads.h

## Objects

### Position
Universal position struct ->  x,y,z
```c++
struct Position
```

### Color
Universal color struct ->  r, g, b, a
```c++
struct Color
```

### Dims
Universal dimensions struct ->  w,h
```c++
struct Dims
```

### Norm
Universal normalized vector struct ->  x_start, x_end, y_start, y_end
```c++
struct Norm
```

### VertexData
Vertex specific data that is contained in QuadData. 
```c++
struct VertexData
```

### VertexIndex
Set of 3 ids to contain Vertex Indices for each quad
```c++
struct VertexIndex
```

### QuadData
Main QuadData - everything that is needed by shader to render quad on the screen
```c++
struct QuadData
```

### AllQuads
Main Quads vector on each frame. All the quads inside AllQuads will be rendered on given frame

```c++
std::vector<QuadData> AllQuads
```


### VERTEX_OFFSET
Offset between quads on the screen
```c++
float VERTEX_OFFSET
```

### COUNT_QUADS
For debugging only, count of quads in AllQuads
```c++
int COUNT_QUADS
```

### MAX_QUADS
Maximum number of allowed quads
```c++
int MAX_QUADS = 29000;
```


### REQ_SIZE_BUFFER
For debugging only, required size of buffer for given AllQuads
```c++
int REQ_SIZE_BUFFER
```

## Functions

### add_quads
Method called by multiple other modules to create quads for specific game objects ->
entities, texts, buttons, gui objects etc. Any piece of data that need to be rendered using quads will call make_quads while providing catalog (map) of data objects

```c++
template <typename T>
std::vector<quads::QuadData> add_quads(phmap::flat_hash_map<int, T> data, int object_type_id)
```

### make_quad
Makes a quad out of single data struct and adds it to allQuads. Returns quad_id
```c++
template <typename T>
int make_quad(T& data, int object_type_id)
```

### update
Calls [_fill_quad_vertex_data](quads.md#_fill_quad_vertex_data) on each quad
```c++
void update();
```

### clear
Method for clearing all current quads AllQuads data
```c++
void clear();
```

### log
Method to log QuadsData to logs/quads.json on every frame basis
```c++
void log();
```

### _fill_quad_vertex_data
Hidden method for filling up quads' vertices  specific data (VertexData) struct
```c++
struct VertexData _fill_quad_vertex_data(quads2::QuadData& q);
```



## Tests
-
