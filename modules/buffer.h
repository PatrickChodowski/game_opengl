
#include "quad.h"
#include <vector>

#ifndef MODULES_BUFFER_H
#define MODULES_BUFFER_H

// Buffer functionality
namespace buffer2
{
  extern unsigned int VBO, VAO, EBO;
  extern int VBO_size;
  extern int VBO_array_size;
  extern float VBO_buffer_usage;
  extern int EBO_size;
  extern int EBO_array_size;
  extern float EBO_buffer_usage;
  extern int MAX_QUADS;


  // Method converting vector of quads to VertexArray
  void _quads_to_array(std::vector<quad::QuadData>& quads, float* arr);

  // Gets main quads vector, transforms it and uses it to update the main gpu buffer
  void update(std::vector<quad::QuadData>& quads);












}


#endif