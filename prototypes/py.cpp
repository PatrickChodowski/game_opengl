
#include <iostream>

#include "../dependencies/pybind11/pybind11.h"

namespace py = pybind11;

char const* greet2()
{ 
    std::cout << "hello world2" << std::endl;
   return "hello, world2";
};

PYBIND11_MODULE(gamescripts, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring
    m.def("greet2", &greet2, "A function hello world");
}

int main() { greet2(); return 0;};