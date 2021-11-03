
#include "quads.h"
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

  // Initialize opengl buffers, attributes etc.
  void init();

  // Method propagating vertex array from quads inside buffer::update
  void _make_vertex_array(std::vector<quads2::QuadData>& quads, float* arr);

  // Method propagating index array from quads inside buffer::update
  void _make_index_array(std::vector<quads2::QuadData>& quads, unsigned int* arr);

  // Gets main quads vector, transforms it and uses it to update the main gpu buffer
  void update(std::vector<quads2::QuadData>& quads);

  // Drop buffers
  void drop();

}


#endif