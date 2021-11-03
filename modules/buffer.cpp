#include "buffer.h"
#include "quad.h"
#include <vector>
#include <cmath>

// Buffer functionality
namespace buffer
{
  // initialize variables
  int VBO_size = MAX_QUADS*sizeof(quad::COUNT_VERTEX_ATTRIBUTES*sizeof(double))*4;
  int VBO_array_size = 0;
  float VBO_buffer_usage = 0.0f;

  int EBO_size = MAX_QUADS*sizeof(float)*6;
  int EBO_array_size = 0;
  float EBO_buffer_usage = 0.0f;



  // Method converting vector of quads to VertexArray
  void _quads_to_array(std::vector<quad::QuadData>& quads, float* arr)
  {
    int cva = quad::COUNT_VERTEX_ATTRIBUTES; 
    for(int t=0; t<quads.size(); t++)
    {
      int start_position = t*cva*4;
      arr[(start_position)] = quads[t].v.v1_x;
      arr[(start_position+1)] = quads[t].v.v1_y;
      arr[(start_position+2)] = quads[t].v.v1_z;
      arr[(start_position+3)] = quads[t].r;
      arr[(start_position+4)] = quads[t].g;
      arr[(start_position+5)] = quads[t].b;
      arr[(start_position+6)] = quads[t].a;
      arr[(start_position+7)] = quads[t].frame_id;
      arr[(start_position+8)] = quads[t].v.v1_tx_x;
      arr[(start_position+9)] = quads[t].v.v1_tx_y;
      arr[(start_position+10)] = quads[t].texture_id;
      arr[(start_position+11)] = quads[t].is_clicked;
      arr[(start_position+12)] = quads[t].type_id;
      arr[(start_position+13)] = quads[t].is_static;

      arr[(start_position+cva)] = quads[t].v.v2_x;
      arr[(start_position+(cva+1))] = quads[t].v.v2_y;
      arr[(start_position+(cva+2))] = quads[t].v.v2_z;
      arr[(start_position+(cva+3))] = quads[t].r;
      arr[(start_position+(cva+4))] = quads[t].g;
      arr[(start_position+(cva+5))] = quads[t].b;
      arr[(start_position+(cva+6))] = quads[t].a;
      arr[(start_position+(cva+7))] = quads[t].frame_id;
      arr[(start_position+(cva+8))] = quads[t].v.v2_tx_x;
      arr[(start_position+(cva+9))] = quads[t].v.v2_tx_y;
      arr[(start_position+(cva+10))] = quads[t].texture_id;
      arr[(start_position+(cva+11))] = quads[t].is_clicked;
      arr[(start_position+(cva+12))] = quads[t].type_id;
      arr[(start_position+(cva+13))] = quads[t].is_static;

      arr[(start_position+(cva*2))] = quads[t].v.v3_x;
      arr[(start_position+(cva*2) + 1)] = quads[t].v.v3_y;
      arr[(start_position+(cva*2) + 2)] = quads[t].v.v3_z;
      arr[(start_position+(cva*2) + 3)] = quads[t].r;
      arr[(start_position+(cva*2) + 4)] = quads[t].g;
      arr[(start_position+(cva*2) + 5)] = quads[t].b;
      arr[(start_position+(cva*2) + 6)] = quads[t].a;
      arr[(start_position+(cva*2) + 7)] = quads[t].frame_id;
      arr[(start_position+(cva*2) + 8)] = quads[t].v.v3_tx_x;
      arr[(start_position+(cva*2) + 9)] = quads[t].v.v3_tx_y;
      arr[(start_position+(cva*2) + 10)] = quads[t].texture_id;
      arr[(start_position+(cva*2) + 11)] = quads[t].is_clicked;
      arr[(start_position+(cva*2) + 12)] = quads[t].type_id;
      arr[(start_position+(cva*2) + 13)] = quads[t].is_static;

      arr[(start_position+(cva*3))] = quads[t].v.v4_x;
      arr[(start_position+(cva*3) + 1)] = quads[t].v.v4_y;
      arr[(start_position+(cva*3) + 2)] = quads[t].v.v4_z;
      arr[(start_position+(cva*3) + 3)] = quads[t].r;
      arr[(start_position+(cva*3) + 4)] = quads[t].g;
      arr[(start_position+(cva*3) + 5)] = quads[t].b;
      arr[(start_position+(cva*3) + 6)] = quads[t].a;
      arr[(start_position+(cva*3) + 7)] = quads[t].frame_id;
      arr[(start_position+(cva*3) + 8)] = quads[t].v.v4_tx_x;
      arr[(start_position+(cva*3) + 9)] = quads[t].v.v4_tx_y;
      arr[(start_position+(cva*3) + 10)] = quads[t].texture_id;
      arr[(start_position+(cva*3) + 11)] = quads[t].is_clicked;
      arr[(start_position+(cva*3) + 12)] = quads[t].type_id;
      arr[(start_position+(cva*3) + 13)] = quads[t].is_static;
    }

    arr;
  }

  // Gets main quads vector, transforms it and uses it to update the main gpu buffer
  void update(std::vector<quad::QuadData>& quads)
  {
    int n_vertex = quads.size()*4;
    int n_vertex_array = quad::COUNT_VERTEX_ATTRIBUTES*n_vertex;
    float vertex_array[n_vertex_array];

    // transforms quads table into array
    _quads_to_array(quads, vertex_array);

    buffer::VBO_array_size = sizeof(float)*n_vertex_array;
    buffer::VBO_buffer_usage = std::round(((float)VBO_array_size/(float)VBO_size) * 1000.0)/1000.0;
    glBindBuffer(GL_ARRAY_BUFFER, VBO);




  }












}
