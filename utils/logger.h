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

  template<typename T>
  void print_vector(std::vector<T> v, std::string message="")
  {
    for(int i=0; i<v.size(); i++)
    {
      std::cout << message << " id: " << i << " value: " << v[i] << std::endl;
    }
  }

}






#endif