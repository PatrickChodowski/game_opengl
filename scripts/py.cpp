
#include <fstream>
#include <iostream>
#include <string>

#include "py.h"
#include "../dependencies/pybind11/pybind11.h"
#include "../dependencies/pybind11/embed.h"

namespace py = pybind11;

namespace scripts
{
  py::scoped_interpreter guard{};

  // this can be good:
  // py::exec("print 1");


  void execute(std::string& script_name)
  {
    // auto kwargs = py::dict("name"_a="World", "number"_a=42);
    // auto message = "Hello, {name}! The answer is {number}"_s.format(**kwargs);

    // when executing the script, access variables from CPP, copy to python, use this to run python logic, return some data, use this data

    using namespace py::literals;
    auto locals = py::dict("name"_a="World", "number"_a=42);
    py::eval_file(script_name, py::globals(), locals);
    auto test_variable = locals["test_variable"].cast<int>();
    std::cout << "test variable: " << test_variable << std::endl;

  };

  void drop()
  {
    //delete &guard;
  };

  

}

