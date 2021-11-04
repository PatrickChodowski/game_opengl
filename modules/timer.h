#include <string>
#include <chrono>
#include <ctime> 

#ifndef MODULES_TIMER_H
#define MODULES_TIMER_H

// timer functionality
namespace timer2
{
  std::time_t get_current_time();

  template<typename T>
  float get_elapsed_time(T start_time);

  template<typename T>
  void print_elapsed_time(T start_time, std::string message);

  template<typename T>
  int get_fps_delay(T start_time, T end_time, float delay);

  template<typename T>
  float get_delay_ms(T start_time, T end_time);

  std::chrono::milliseconds get_current_ms_time();

  float get_elapsed_ms(std::chrono::milliseconds start);

}

#endif
