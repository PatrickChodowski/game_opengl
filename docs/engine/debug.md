# Debug
Path: ./modules/debug.cpp   ./modules/debug.h


## Objects


### LineData
Debug Line data -> start, end points and color
```c++
struct LineData
```

### PointData
Debug Point data -> basically for quad
```c++
struct PointData
```

### lines
Vector of [line data](debug.md#LineData)
```c++
std::vector<debug::LineData> lines;
```

### points
Vector of [points data](debug.md#PointData)
```c++
std::vector<debug::PointData> points;
```

### DebugQuads
Quads made out of the [points](debug.md#points). Vector of [quads](quads.md#QuadData)
```c++
std::vector<quads::QuadData> DebugQuads;
```
### Index
Index of unique Debug IDs
```c++
std::vector<int> Index;
```



## Functions
### render_line
Render debug Line (creates new [line data](debug.md#LineData))
```c++
void render_line(float x1, float y1, float x2, float y2, float r, float g, float b, float a);

```

### render_point
Render debug point (creates new [point data](debug.md#PointData))
```c++
void render_point(float x, float y, float r, float g, float b, float a);
```

### render_square
Well, it renders 4 lines
```c++
void render_square(float x, float y, float w, float h, float r, float g, float b, float a);

```


### render
Populates [DebugQuads](debug.md#DebugQuads) from [points](debug.md#points)
```c++
void render();
```

### clear
Clears [DebugQuads](debug.md#DebugQuads), [Index](debug.md#Index), [lines](debug.md#lines), [points](debug.md#points)
```c++
void clear();
```


## Tests
- 
