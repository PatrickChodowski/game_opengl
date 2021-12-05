# Quads
Path: ./modules/quads.cpp   ./modules/quads.h

## Objects

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

### QuadIndex
Index of unique used quad id's
```c++
std::vector<int> QuadIndex
```

### VertexIndex
Index of unique used vertex id's
```c++
std::vector<int> VertexIndex
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


### REQ_SIZE_BUFFER
For debugging only, required size of buffer for given AllQuads
```c++
int REQ_SIZE_BUFFER
```

## Functions

### make_quads
Method called by multiple other modules to create quads for specific game objects ->
entities, texts, buttons, gui objects etc. Any piece of data that need to be rendered using quads will call make_quads while providing catalog (map) of data objects

```c++
template <typename T>
std::vector<quads2::QuadData> make_quads(std::map<int, T> data, int object_type_id)
```

### accumulate
Collects all QuadData vectors into one vector -> [quads::AllQuads](quads.md#AllQuads)
```c++
void accumulate()
```

### gen_quad_id
Generate next available quad id. Used when creating new quad
```c++
int gen_quad_id();
```

### gen_vertex_id
Generate next available vertex id. Used when creating new vertex

```c++
int gen_vertex_id();
```

### clear
Method for clearing all current quads (indexes, AllQuads) data
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

### _find_next_quad_id
Hidden method for finding next available vertex_id
```c++
int _find_next_quad_id();
```

### _find_next_vertex_id
Hidden method for finding next available vertex_id
```c++
int _find_next_vertex_id();
```


## Tests
-
