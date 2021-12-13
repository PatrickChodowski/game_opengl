
#include <fstream>
#include <iostream>
#include <string>

#include "py.h"
#include "../dependencies/pybind11/pybind11.h"
#include "../dependencies/pybind11/embed.h"

namespace py = pybind11;

namespace scripts
{
  py::scoped_interpreter guard;
  // void init()
  // {
  //   py::scoped_interpreter guard{};
  // }

  //py::exec("print 1");


  void execute_script(std::string& script_name)
  {
    std::ifstream script(script_name);
    std::string line;
    if(script.is_open())
    {
      while(std::getline(script, line))
      {
        py::exec(line.c_str());
      } 
      script.close();
    }
  };


  void drop()
  {
    delete &guard;
  };

  

}

