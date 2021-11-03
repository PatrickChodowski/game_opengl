
#include "quad.h"
#include <vector>

#ifndef MODULES_BUFFER_H
#define MODULES_BUFFER_H

// Buffer functionality
namespace buffer
{
  unsigned int VBO, VAO, EBO;
  // Size of VBO buffer -> size of Vertex: 120*4, 4 vertices per quad, max quads is 2000. Buffer size is 960 000
  int VBO_size = MAX_QUADS*sizeof(quad::COUNT_VERTEX_ATTRIBUTES*sizeof(double))*4;
  int VBO_array_size = 0;
  float VBO_buffer_usage = 0.0f;

  int EBO_size = MAX_QUADS*sizeof(float)*6;
  int EBO_array_size = 0;
  float EBO_buffer_usage = 0.0f;


  // Method converting vector of quads to VertexArray
  void _quads_to_array(std::vector<quad::QuadData>& quads, float* arr);

  // Gets main quads vector, transforms it and uses it to update the main gpu buffer
  void update(std::vector<quad::QuadData>& quads);












}


#endif