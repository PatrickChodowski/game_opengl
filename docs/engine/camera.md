# Camera
Path: ./modules/camera.cpp   ./modules/camera.h


## Objects
### Camera
All camera parameters in one struct
```c++
struct Camera;
```
### cam
Singleton instance of [struct Camera](camera.md#Camera). Main camera in the game
```c++
struct Camera cam;
```

### STATIC_MVP
MVP matrix for static objects
```c++
glm::mat4 STATIC_MVP;
```

### DYNAMIC_MVP
MVP matrix for dynamic objects
```c++
glm::mat4 DYNAMIC_MVP;
```

### ZOOM_MVP
MVP matrix for zoom-only objects
```c++
glm::mat4 ZOOM_MVP;
```

## Functions
### reset
Resets [camera::cam](camera.md#cam) to default parameters
```c++
void reset();
```

### scale_quads
Scales [quads::AllQuads](quads.md#AllQuads) vector of quads to the game window
```c++
void scale_quads(float camera_x, float camera_y, float camera_zoom);
```

### gen_dynamic_mvp
Generates [camera::DYNAMIC_MVP](camera.md#DYNAMIC_MVP) 
```c++
glm::mat4 gen_dynamic_mvp(float camera_move_x, float camera_move_y, float camera_zoom);
```
### gen_static_mvp
Generates [camera::STATIC_MVP](camera.md#STATIC_MVP) 
```c++
glm::mat4 gen_static_mvp();
```

### gen_zoom_only_mvp
Generates [camera::ZOOM_MVP](camera.md#ZOOM_MVP) 
```c++
glm::mat4 gen_zoom_only_mvp(float camera_zoom);
```

## Tests
- scale_quads
- reset