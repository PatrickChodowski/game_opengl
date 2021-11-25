# Quads

## Objects

### VertexData
```c++
struct VertexData
```

### VertexIndex
```c++
struct VertexIndex
```

### QuadData
```c++
struct QuadData
```

### AllQuads
```c++
std::vector<QuadData> AllQuads
```

### QuadIndex
```c++
std::vector<int> QuadIndex
```

### VertexIndex
```c++
std::vector<int> VertexIndex
```

### VERTEX_OFFSET
```c++
float VERTEX_OFFSET
```

### COUNT_QUADS
```c++
int COUNT_QUADS
```

### REQ_SIZE_BUFFER
```c++
int REQ_SIZE_BUFFER
```

## Functions

### make_quads
```c++
template <typename T>
std::vector<quads2::QuadData> make_quads(std::map<int, T> data, int object_type_id)
```

### accumulate
```c++
void accumulate()
```

### gen_quad_id
```c++
int gen_quad_id();
```

### gen_vertex_id
```c++
int gen_vertex_id();
```

### clear
```c++
void clear();
```

### log
```c++
void log();
```

### _fill_quad_vertex_data
```c++
struct VertexData _fill_quad_vertex_data(quads2::QuadData& q);
```

### _find_next_quad_id
```c++
int _find_next_quad_id();
```

### _find_next_vertex_id

```c++
int _find_next_vertex_id();
```


## Tests
