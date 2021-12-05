# Shaders
Path: ./modules/shaders.cpp   ./modules/shaders.h


## Objects

### ShaderData
All shader information: id, opengl_id, name
```c++
struct ShaderData
```

### shaders
Catalog of [ShaderData](shaders.md#ShaderData)
```c++
std::map<int, ShaderData> shaders;
```

### shading_program
Id of the shader for OpenGL
```c++
GLuint shading_program;
```

## Functions
### init
Gets list of shaders and reads data about them. Fills out [ShaderData](shaders.md#ShaderData)
```c++
void init();
```

### read_data
Fills out [ShaderData](shaders.md#ShaderData)
```c++
void read_data(std::string name);
```

### build
Build shader from custom files
```c++
GLuint build(const char *shader_name);
```

### drop
Delete shaders from OpenGL context
```c++
void drop();
```

### _read_file_as_string
Reads shaders file as string
```c++
std::string _read_file_as_string(const char *filename);
```

### _compile_shader
Compiles shader from read file
```c++
GLuint _compile_shader(GLenum type, GLsizei nsources, const char **sources);
```

### _shader_program_check
Performs shaders check
```c++
GLuint _shader_program_check(GLuint program);
```

### _get_shader
Loads a shader from file and returns the compiled shader
```c++
GLuint _get_shader(GLenum shader_type, const char *filename);
```



## Tests
-
