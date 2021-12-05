# Timer
Path: ./modules/timer.cpp   ./modules/timer.h


## Objects


## Functions
### get_current_time
Gets current time
```c++
std::time_t get_current_time();
```

### get_elapsed_time
Get elapsed time from starting point
```c++
float get_elapsed_time(std::chrono::time_point<std::chrono::high_resolution_clock> start_time);
```

### print_elapsed_time
Print elapsed time with provided message
```c++
void print_elapsed_time(std::chrono::time_point<std::chrono::high_resolution_clock> start_time,std::string message);
```

### get_fps_delay
Get FPS calculation
```c++
int get_fps_delay(std::chrono::time_point<std::chrono::high_resolution_clock> start_time,std::chrono::time_point<std::chrono::high_resolution_clock> end_time,
float delay);
```

### get_delay_ms
Get delay in miliseconds calculated based on start_time and end_time
```c++
float get_delay_ms(std::chrono::time_point<std::chrono::high_resolution_clock> start_time,std::chrono::time_point<std::chrono::high_resolution_clock> end_time);
```

### get_current_ms_time
Get Current time in miliseconds
```c++
std::chrono::milliseconds get_current_ms_time();
```

### get_elapsed_ms
Get elapsed miliseconds from start time
```c++
float get_elapsed_ms(std::chrono::milliseconds start);
```


## Tests
- 
