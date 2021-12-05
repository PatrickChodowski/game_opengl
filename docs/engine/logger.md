# Logger
Path: ./modules/logger.cpp   ./modules/logger.h

## Objects
### LOG_PATH_RUN
Path for the logs for unique run of the game (based on date and timestamp)
```c++
std::string LOG_PATH_RUN;
```
### LOG_PATH
Path for the logs in general
```c++
std::string LOG_PATH;
```
### LOG_LEVEL
LOG_LEVEL of the engine (debug, info, error)
```c++
int LOG_LEVEL;
```
### function_start_time
Function start time if the log message is set to be of type start_function
```c++
std::chrono::time_point<std::chrono::high_resolution_clock> function_start_time;
```
### LogMessage
Struct with all the information about the single log message sent to unique game run file
```c++
struct LogMessage
```

## Functions
### init
Initialize logger data and logic
```c++
void init();
```

### log_data
Logs data files every frame of each module able to dump data file (jsons from maps and vectors)
```c++
void log_data();
```

### print
Prints input message to the console
```c++
template<typename T>
void print(T message);
```

### log
Logs logmessage object to runtime log file and prints it to the console as well
```c++
 void log(int log_level, 
           std::string msg,
           std::string module,
           std::string file,
           int line,
           int type
           );
```


## Tests
- 

