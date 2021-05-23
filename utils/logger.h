#ifndef LOGGER_H
#define LOGGER_H

namespace logger
{
  // debug 0 
  // info  1
  // error 2

  template<typename T>
  void print(T message, int level=1)
  {
    if(level >= LOGGING)
    {
      std::cout << message << std::endl;
    }
  }

}






#endif