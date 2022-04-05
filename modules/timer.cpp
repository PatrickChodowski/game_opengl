#include "timer.h"

#include <chrono>
#include <ctime> 
#include <iostream>
#include <string>

namespace timer
{
  std::time_t get_current_time()
  {
    auto time_now_start = std::chrono::system_clock::now();
    std::time_t time_now_start_time = std::chrono::system_clock::to_time_t(time_now_start);
    // std::string time_now_start_time_string = std::ctime(&time_now_start_time);
    // logger::print("current time: " + time_now_start_time_string);
    return time_now_start_time;
  }

  float get_elapsed_time(std::chrono::time_point<std::chrono::high_resolution_clock> start_time)
  {
    std::chrono::time_point<std::chrono::high_resolution_clock> end = timer::get_current_hrc_time();
    std::chrono::duration<double> elapsed_seconds = end-start_time;
    float elapsed_time = elapsed_seconds.count();
    return elapsed_time;
  }

  float get_elapsed_time_ms(std::chrono::time_point<std::chrono::high_resolution_clock> start_time)
  {
    std::chrono::time_point<std::chrono::high_resolution_clock> end = timer::get_current_hrc_time();
    std::chrono::duration<double> elapsed_seconds = end-start_time;
    float elapsed_time = elapsed_seconds.count()*1000;
    return elapsed_time;
  }

  void print_elapsed_time(std::chrono::time_point<std::chrono::high_resolution_clock> start_time, 
                          std::string message)
  {
    std::chrono::time_point<std::chrono::high_resolution_clock> end = timer::get_current_hrc_time();
    std::chrono::duration<double> elapsed_seconds = end-start_time;
    std::string time_string = std::to_string(elapsed_seconds.count());
    std::string full_message = message + " " + time_string;
    // logger::print(full_message, -1);
  }


  int get_fps_delay(std::chrono::time_point<std::chrono::high_resolution_clock> start_time,
                    std::chrono::time_point<std::chrono::high_resolution_clock> end_time,
                    float delay)
  {
    std::chrono::duration<double> elapsed_seconds = end_time-start_time;
    // std::cout << "end time: " << elapsed_seconds.count() + delay << std::endl;
    int fps = 1000/(elapsed_seconds.count() + delay);
    return fps;
  }

  float get_delay_ms(std::chrono::time_point<std::chrono::high_resolution_clock> start_time,
                     std::chrono::time_point<std::chrono::high_resolution_clock> end_time)
  {
    std::chrono::duration<double> elapsed_seconds = end_time-start_time;
    float elapsed_seconds_ = elapsed_seconds.count();
    //std::cout << " elapsed seconds: " << elapsed_seconds_ << std::endl;
    float delay = 17.0 - elapsed_seconds_;
    return delay;
  }

  std::chrono::milliseconds get_current_ms_time()
  {
    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::system_clock::now().time_since_epoch()
    );
    return ms;
  }

  float get_elapsed_ms(std::chrono::milliseconds start)
  {
    std::chrono::milliseconds now_time = timer::get_current_ms_time();
    float elapsed_ms = now_time.count() - start.count();
    return elapsed_ms;
  }

  std::chrono::time_point<std::chrono::high_resolution_clock> get_current_hrc_time()
  {
    return std::chrono::high_resolution_clock::now();
  }

}