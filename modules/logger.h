
#include <chrono>
#include <ctime> 
#include <string>
#include "../dependencies/json_struct.h"

#ifndef MODULES_LOGGER_H
#define MODULES_LOGGER_H

namespace logger
{
  extern std::string LOG_PATH_RUN;
  extern std::string LOG_PATH;
  extern int LOG_LEVEL;
  extern std::chrono::time_point<std::chrono::high_resolution_clock> function_start_time;

  struct LogMessage
  {
    std::string file;
    std::string path;
    std::string msg;
    std::string module;

    float time_since_game_start;
    float function_time_elapsed;

    int line;
    int log_level;
    int type;

    std::time_t current_time;

    JS_OBJ(file, path, msg, module, time_since_game_start, 
           function_time_elapsed, line, log_level, type, current_time);
  };

  // Initialize logger logic
  void init();

  // Writes down the data from each component to files every frame
  void log_data();

  // Prints message to the console
  template<typename T>
  void print(T message);

  // Prints message in console and logs it to run file
  void log(int log_level, 
           std::string msg,
           std::string module,
           std::string file,
           int line,
           int type
           );

}

#endif