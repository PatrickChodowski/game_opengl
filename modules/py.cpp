
#include <iostream>

#include <boost/python.hpp>

char const* greet()
{ 
    std::cout << "hello world" << std::endl;
   return "hello, world";
};

BOOST_PYTHON_MODULE(gamescripts)
{
    using namespace boost::python;
    def("greet", greet);
}

int main() { greet(); return 0;};