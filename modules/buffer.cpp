#include "buffer.h"
#include "quads.h"
#include "utils.h"


#include <cmath>
#include <iostream>
#include <vector>
// Opengl packages
#include <GL/glew.h> 
#ifdef TARGET_OS_MAC
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#endif

#ifdef __linux__
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif

namespace buffer2
{
  
  int MAX_QUADS = 2000;
  unsigned int VBO, VAO, EBO;
  int VBO_size = buffer2::MAX_QUADS*quads2::COUNT_VERTEX_ATTRIBUTES*sizeof(float)*4;
  int VBO_array_size;
  float VBO_buffer_usage;
  int EBO_size =  buffer2::MAX_QUADS*sizeof(float)*6;
  int EBO_array_size;
  float EBO_buffer_usage;

  void init()
  {
    // std::cout << " quads2::COUNT_VERTEX_ATTRIBUTES on buffer init: " << quads2::COUNT_VERTEX_ATTRIBUTES << std::endl;
    // std::cout << " max quads on buffer init: " << buffer2::MAX_QUADS << std::endl;
    // std::cout << " VBO_size on buffer init: " << buffer2::VBO_size << std::endl;
    // std::cout << " EBO_size on buffer init: " << buffer2::EBO_size << std::endl;
    // OpenGL options:
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Generate buffers:
    glGenVertexArrays(1, &buffer2::VAO);
    glGenBuffers(1, &buffer2::VBO);
    glGenBuffers(1, &buffer2::EBO);

    // Bind buffers
    glBindVertexArray(buffer2::VAO);
    glBindBuffer(GL_ARRAY_BUFFER, buffer2::VBO);
    glBufferData(GL_ARRAY_BUFFER, buffer2::VBO_size, nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer2::EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer2::EBO_size, nullptr, GL_DYNAMIC_DRAW);

    // Set attributes
    // position attribute:
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, quads2::COUNT_VERTEX_ATTRIBUTES * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute:
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, quads2::COUNT_VERTEX_ATTRIBUTES * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // frame_id attribute
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, quads2::COUNT_VERTEX_ATTRIBUTES * sizeof(float), (void*)(7 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // in texture coordinates attribute
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, quads2::COUNT_VERTEX_ATTRIBUTES * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(3);

    // in texture id attribute
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, quads2::COUNT_VERTEX_ATTRIBUTES * sizeof(float), (void*)(10 * sizeof(float)));
    glEnableVertexAttribArray(4);

    // is_clicked  attribute
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, quads2::COUNT_VERTEX_ATTRIBUTES * sizeof(float), (void*)(11 * sizeof(float)));
    glEnableVertexAttribArray(5);

    // quad type id  attribute
    glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, quads2::COUNT_VERTEX_ATTRIBUTES * sizeof(float), (void*)(12 * sizeof(float)));
    glEnableVertexAttribArray(6);

    // is_static attribute
    glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, quads2::COUNT_VERTEX_ATTRIBUTES * sizeof(float), (void*)(13 * sizeof(float)));
    glEnableVertexAttribArray(7);

  }

  void _make_vertex_array(std::vector<quads2::QuadData>& quads, float* arr)
  {
    int cva = quads2::COUNT_VERTEX_ATTRIBUTES; 
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
      arr[(start_position+12)] = quads[t].object_type_id;
      arr[(start_position+13)] = quads[t].camera_type;

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
      arr[(start_position+(cva+12))] = quads[t].object_type_id;
      arr[(start_position+(cva+13))] = quads[t].camera_type;

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
      arr[(start_position+(cva*2) + 12)] = quads[t].object_type_id;
      arr[(start_position+(cva*2) + 13)] = quads[t].camera_type;

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
      arr[(start_position+(cva*3) + 12)] = quads[t].object_type_id;
      arr[(start_position+(cva*3) + 13)] = quads[t].camera_type;

    }
  }

  void _make_index_array(std::vector<quads2::QuadData>& quads, unsigned int* arr)
  {
    for(int t=0; t<quads.size(); t++)
    {
      int start_position = t*3*2;
      arr[(start_position+0)] = quads[t].i_left.v1;
      arr[(start_position+1)] = quads[t].i_left.v2;
      arr[(start_position+2)] = quads[t].i_left.v3;

      arr[(start_position+3)] = quads[t].i_right.v1;
      arr[(start_position+4)] = quads[t].i_right.v2;
      arr[(start_position+5)] = quads[t].i_right.v3;
    }
  }

  void update(std::vector<quads2::QuadData>& quads)
  { 
    // Assigning vertex index and vertex positions here, on the final table
    for(int q=0; q < quads.size(); q++ )
    { 
      quads2::_fill_quad_vertex_data(quads[q]);
    }

    int n_vertex_array = quads2::COUNT_VERTEX_ATTRIBUTES*quads.size()*4;
    float vertex_array[n_vertex_array];

    int n_index_array = 3*quads.size()*2;
    unsigned int index_array[n_index_array];

    buffer2::_make_vertex_array(quads, vertex_array);

    utils2::array_to_file("buffer_update_vertex_array", vertex_array, n_vertex_array, quads2::COUNT_VERTEX_ATTRIBUTES);

    buffer2::_make_index_array(quads, index_array);

    buffer2::VBO_array_size = sizeof(float)*n_vertex_array;
    buffer2::VBO_buffer_usage = std::round(((float)VBO_array_size/(float)VBO_size) * 1000.0)/1000.0;
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, buffer2::VBO_array_size, vertex_array);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    buffer2::EBO_array_size = sizeof(float)*n_index_array;
    buffer2::EBO_buffer_usage = std::round(((float)EBO_array_size/(float)EBO_size) * 1000.0)/1000.0;

    //std::cout << "VBO USAGE: " << buffer2::VBO_buffer_usage << std::endl;
    // std::cout << "VBO array size: " << buffer2::VBO_array_size << std::endl;
    // std::cout << "VBO size: " << buffer2::VBO_size << std::endl;

    //std::cout << "EBO USAGE: " << buffer2::EBO_buffer_usage << std::endl;

    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, buffer2::EBO_array_size, index_array);
  }

  void drop()
  {
    glDeleteVertexArrays(1, &buffer2::VAO);
    glDeleteBuffers(1, &buffer2::VBO);
    glDeleteBuffers(1, &buffer2::EBO);
  }


}
