# Textures
Path: ./modules/textures.cpp   ./modules/textures.h

## Objects
### Frame
KeyFrame data -> id, position in the texture, dimensions and label
```c++
struct Frame
```

### TextureData
TextureData -> id, opengl id, dimensions, types, name, frames, anims
```c++
struct TextureData
```

### textures
Persistent table of texture data
```c++
phmap::btree_map<int, textures2::TextureData> textures;
```

### BoundTextures
Vector of OpenGL bound textures
```c++
std::vector<unsigned int> BoundTextures;
```

## Functions
### read_data
Reads data from texture file to [textures::TextureData](textures.md#TextureData) and adds it to [textures::textures](textures.md#textures)
```c++
void read_data(std::string name);
```

### load
Load texture to the catalog and OpenGL
```c++
void load(int texture_id);
```

### bind
Bind textures from Catalog to opengl
```c++
void bind();
```

### init
Init texture data and bind them to OpenGL
```c++
void init();
```

### drop
Drop Textures from OpenGL (based on boundTextures vector)
```c++
void drop();
```

### log
Log textures to logs/textures.json on every frame
```c++
void log();
```

### _get_normalized_frame_x_start
Get normalized (between 0 and 1) X start of the frame in the texture
```c++
float _get_normalized_frame_x_start(int texture_id, int frame_id);
```

### _get_normalized_frame_x_end
Get normalized (between 0 and 1) X end of the frame in the texture
```c++
float _get_normalized_frame_x_end(int texture_id, int frame_id);
```

### _get_normalized_frame_y_start
Get normalized (between 0 and 1) Y start of the frame in the texture
```c++
float _get_normalized_frame_y_start(int texture_id, int frame_id);
```

### _get_normalized_frame_y_end
Get normalized (between 0 and 1) Y end of the frame in the texture
```c++
float _get_normalized_frame_y_end(int texture_id, int frame_id);
```

## Tests
- 