
#include "debug.h"
#include "quads.h"

#include <map>
#include <vector>

#ifndef MODULES_BUFFER_H
#define MODULES_BUFFER_H

// Buffer functionality
namespace buffer
{
  extern unsigned int VBO, VAO, EBO;
  extern int COUNT_VERTEX_ATTRIBUTES;
  extern int VBO_size;
  extern int VBO_array_size;
  extern float VBO_buffer_usage;
  extern int EBO_size;
  extern int EBO_array_size;
  extern float EBO_buffer_usage;

  // Initialize opengl buffers, attributes etc.
  void init();

  // Method propagating vertex array from quads inside buffer::update
  void _make_vertex_array_from_quads(std::vector<quads::QuadData>& quads, float* arr);

  // Method propagating index array from quads inside buffer::update
  void _make_index_array_from_quads(std::vector<quads::QuadData>& quads, unsigned int* arr);

  // Method propagating vertex array from lines inside buffer::update
  void _make_vertex_array_from_lines(std::vector<debug::LineData>& lines, float* arr);

  // Gets main quads vector, transforms it and uses it to update the main gpu buffer
  void update_quads(std::vector<quads::QuadData>& quads);

  // Gets main lines vector, transforms it and uses it to update the main gpu buffer
  void update_lines(std::vector<debug::LineData>& lines);

  // Drop buffers
  void drop();

  // Log buffer
  void log(float* arr, int arr_size);

}


#endif