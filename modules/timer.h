#include <string>
#include <chrono>
#include <ctime> 

#ifndef MODULES_TIMER_H
#define MODULES_TIMER_H

// timer functionality
namespace timer
{
  // Provides current time
  std::time_t get_current_time();

  // Get elapsed time from starting point
  float get_elapsed_time(std::chrono::time_point<std::chrono::high_resolution_clock> start_time);

  // Print elapsed time with provided message
  void print_elapsed_time(std::chrono::time_point<std::chrono::high_resolution_clock> start_time,
                          std::string message);

  // Get frames per second parameter
  int get_fps_delay(std::chrono::time_point<std::chrono::high_resolution_clock> start_time,
                    std::chrono::time_point<std::chrono::high_resolution_clock> end_time,
                    float delay);

  // Get frame delay in miliseconds
  float get_delay_ms(std::chrono::time_point<std::chrono::high_resolution_clock> start_time,
                     std::chrono::time_point<std::chrono::high_resolution_clock> end_time);

  // Get Current time in miliseconds
  std::chrono::milliseconds get_current_ms_time();

  // Get elapsed miliseconds from start time
  float get_elapsed_ms(std::chrono::milliseconds start);

}

#endif
