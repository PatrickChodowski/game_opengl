
#include <iostream>

#include "../dependencies/pybind11/pybind11.h"


namespace py = pybind11;

char const* greet()
{ 
  std::cout << "hello world2" << std::endl;
  return "hello, world2";
};

PYBIND11_MODULE(scripts, m) {
  m.doc() = "pybind11 example plugin"; // optional module docstring
  m.def("greet", &greet, "A function hello world");


// struct Pet{
//     Pet(const std::string &name) : name(name){}
//     void setName(const std::string &name_){
//         name = name_;
//     }
//     const std::string &getName() const { return name; }

//     std::string name;
// };

    // py::class_<Pet>(m, "Pet")
    //         .def(py::init<const std::string &>())   // our constructor
    //         .def("setName", &Pet::setName)      // Expose member methods
    //         .def("getName", &Pet::getName)      // Think about the syntax "&Pet then "::" and the method name
    //         .def_readwrite("name",&Pet::name);  // Expose member variables
}
