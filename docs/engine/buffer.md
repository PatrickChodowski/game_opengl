# Buffer
Path: ./modules/buffer.cpp   ./modules/buffer.h


## Objects

### VBO
Vertex Buffer Object - Main vertex buffer
```c++
unsigned int VBO
```

### VAO
Vertex Array Object - Main buffer (join of VBO and EBO)
```c++
unsigned int VAO
```
### EBO
Index Buffer
```c++
unsigned int EBO
```

### COUNT_VERTEX_ATTRIBUTES
Specified number of attributes defining each Vertex
```c++
int COUNT_VERTEX_ATTRIBUTES
```

### VBO_size
Size of VBO on each update
```c++
int VBO_size
```

### VBO_array_size
Size of VBO Array (count of elements) on each update
```c++
int VBO_array_size
```

### VBO_buffer_usage
Usage of VBO buffer on each update
```c++
float VBO_buffer_usage;
```

### EBO_size
Size of EBO on each update
```c++
int EBO_size
```

### EBO_array_size
Size of EBO Array (count of elements) on each update
```c++
int EBO_array_size
```

### EBO_buffer_usage
Usage of EBO buffer on each update
```c++
float EBO_buffer_usage;
```

### MAX_QUADS
Defines maximum amount of quads engine can render
```c++
int MAX_QUADS;
```

## Functions
### init
Initialize VBO, VAO, EBO buffers, vertex attributes and all that dark magic
```c++
void init();
```
### drop
Drop buffers
```c++
void drop();
```
### update_quads
Send [quads::AllQuads data](quads.md#AllQuads) to the buffer
```c++
void update_quads(std::vector<quads2::QuadData>& quads);
```
### update_lines
Send [debug::lines data](debug.md#lines) to the buffer
```c++
void update_lines(std::vector<debug2::LineData>& lines);
```
### _make_vertex_array_from_quads
Make VBO array from [quads::AllQuads data](quads.md#AllQuads)
```c++
void _make_vertex_array_from_quads(std::vector<quads2::QuadData>& quads, float* arr);
```
### _make_index_array_from_quads
Make EBO array from [quads::AllQuads data](quads.md#AllQuads)
```c++
void _make_index_array_from_quads(std::vector<quads2::QuadData>& quads, unsigned int* arr);
```
### _make_vertex_array_from_lines
Make VBO array from [debug::lines data](debug.md#lines)
```c++
void _make_vertex_array_from_lines(std::vector<debug2::LineData>& lines, float* arr);
```


## Tests
- _make_vertex_array_from_quads
- _make_index_array_from_quads
- _make_vertex_array_from_lines
