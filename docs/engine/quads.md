# Quads

## Objects
struct VertexData
struct VertexIndex
struct QuadData
std::vector<QuadData> AllQuads;
std::vector<int> QuadIndex;
std::vector<int> VertexIndex;
float VERTEX_OFFSET;
int COUNT_QUADS;
int REQ_SIZE_BUFFER;


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
VertexData _fill_quad_vertex_data(quads2::QuadData& q);
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
