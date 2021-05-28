#ifndef TIMER_H
#define TIMER_H

// timer function elapsed time print, get etc.

namespace timer
{
  // macro for timing function
  std::time_t get_current_time()
  {
    auto time_now_start = std::chrono::system_clock::now();
    std::time_t time_now_start_time = std::chrono::system_clock::to_time_t(time_now_start);
    // std::string time_now_start_time_string = std::ctime(&time_now_start_time);
    // logger::print("current time: " + time_now_start_time_string);
    return time_now_start_time;
  }

  template<typename T>
  float get_elapsed_time(T start_time)
  {
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start_time;
    float elapsed_time = elapsed_seconds.count();
    return elapsed_time;
  }

  template<typename T>
  void print_elapsed_time(T start_time, std::string message = "")
  {
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start_time;
    std::string time_string = std::to_string(elapsed_seconds.count());
    std::string full_message = message + " " + time_string;
    logger::print(full_message, -1);
  }

}

#endif
