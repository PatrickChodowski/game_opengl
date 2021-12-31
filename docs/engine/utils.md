# Utils
Path: ./modules/utils.cpp   ./modules/utils.h


## Objects


## Functions

### read_text_file
Reads in the text file
```c++
std::string read_text_file(std::string path);
```

### has_ending
checks if string has specified ending
```c++
bool has_ending(std::string const &full_string, std::string const &ending);
```

### list_json_files
list all json files in directory that have metadata created, default path
```c++
std::vector<std::string> list_json_files(std::string path);
```

### list_gltf_files
list all gltf files in directory that have metadata created, default path
```c++
std::vector<std::string> list_gltf_files(std::string path);
```

### list_any_files
List all files in the directory
```c++
std::vector<std::string> list_any_files(std::string path);
```

### check_glew
Checks glew
```c++
void check_glew(GLenum err);
```

### get_random
Get random int from the range
```c++
int get_random(int min_int, int max_int);
```

### array_to_file
Float array saved to file
```c++
void array_to_file(std::string file_name, float* arr, int array_size, int new_line_count)
```

### _find_next_id
Find next available ID for provided IndexTable
```c++
int _find_next_id(std::vector<int>& used_ids_table);
```

### generate_id
Generate (find and insert) next available ID for provided IndexTable
```c++
int generate_id(std::vector<int>& used_ids_table);
```

### drop_id
Drop ID from provided IndexTable
```c++
void drop_id(std::vector<int>& used_ids_table, int id);
```

### get_distance_between_points
Calculates distance between two points
```c++
float get_distance_between_points(float x1, float y1, float x2, float y2);
```


## Tests
-
