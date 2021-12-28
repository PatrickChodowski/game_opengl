#include "buffer.h"
#include "debug.h"
#include "quads.h"
#include "utils.h"


#include <cmath>
#include <iostream>
#include <fstream>
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

#include "../dictionary.h"

namespace buffer
{
  
  unsigned int VBO, VAO, EBO;
  int COUNT_VERTEX_ATTRIBUTES = 14;
  // 28 800 * 14 * 4 * 4 bytes = 6451200 bytes = 6451,2 kilobytes = 6.45 megabytes
  int VBO_size = quads::MAX_QUADS*buffer::COUNT_VERTEX_ATTRIBUTES*sizeof(float)*4;
  int VBO_array_size;
  float VBO_buffer_usage;
  int EBO_size =  quads::MAX_QUADS*sizeof(float)*6;
  int EBO_array_size;
  float EBO_buffer_usage;

  void init()
  {
    // OpenGL options:
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Generate buffers:
    glGenVertexArrays(1, &buffer::VAO);
    glGenBuffers(1, &buffer::VBO);
    glGenBuffers(1, &buffer::EBO);

    // Bind buffers
    glBindVertexArray(buffer::VAO);
    glBindBuffer(GL_ARRAY_BUFFER, buffer::VBO);
    glBufferData(GL_ARRAY_BUFFER, buffer::VBO_size, nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer::EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer::EBO_size, nullptr, GL_DYNAMIC_DRAW);

    // Set attributes
    // position attribute:
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, buffer::COUNT_VERTEX_ATTRIBUTES * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute:
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, buffer::COUNT_VERTEX_ATTRIBUTES * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // frame_id attribute
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, buffer::COUNT_VERTEX_ATTRIBUTES * sizeof(float), (void*)(7 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // in texture coordinates attribute
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, buffer::COUNT_VERTEX_ATTRIBUTES * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(3);

    // in texture id attribute
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, buffer::COUNT_VERTEX_ATTRIBUTES * sizeof(float), (void*)(10 * sizeof(float)));
    glEnableVertexAttribArray(4);

    // is_clicked  attribute
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, buffer::COUNT_VERTEX_ATTRIBUTES * sizeof(float), (void*)(11 * sizeof(float)));
    glEnableVertexAttribArray(5);

    // quad type id  attribute
    glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, buffer::COUNT_VERTEX_ATTRIBUTES * sizeof(float), (void*)(12 * sizeof(float)));
    glEnableVertexAttribArray(6);

    // camera_type attribute
    glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, buffer::COUNT_VERTEX_ATTRIBUTES * sizeof(float), (void*)(13 * sizeof(float)));
    glEnableVertexAttribArray(7);

  }

  void _make_vertex_array_from_quads(std::vector<quads::QuadData>& quads, float* arr)
  {
    int cva = buffer::COUNT_VERTEX_ATTRIBUTES; 
    for(int t=0; t<quads.size(); t++)
    {
      int start_position = t*cva*4;
      arr[(start_position)] = quads[t].v.v1_x;
      arr[(start_position+1)] = quads[t].v.v1_y;
      arr[(start_position+2)] = quads[t].v.v1_z;
      arr[(start_position+3)] = quads[t].color.r;
      arr[(start_position+4)] = quads[t].color.g;
      arr[(start_position+5)] = quads[t].color.b;
      arr[(start_position+6)] = quads[t].color.a;
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
      arr[(start_position+(cva+3))] = quads[t].color.r;
      arr[(start_position+(cva+4))] = quads[t].color.g;
      arr[(start_position+(cva+5))] = quads[t].color.b;
      arr[(start_position+(cva+6))] = quads[t].color.a;
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
      arr[(start_position+(cva*2) + 3)] = quads[t].color.r;
      arr[(start_position+(cva*2) + 4)] = quads[t].color.g;
      arr[(start_position+(cva*2) + 5)] = quads[t].color.b;
      arr[(start_position+(cva*2) + 6)] = quads[t].color.a;
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
      arr[(start_position+(cva*3) + 3)] = quads[t].color.r;
      arr[(start_position+(cva*3) + 4)] = quads[t].color.g;
      arr[(start_position+(cva*3) + 5)] = quads[t].color.b;
      arr[(start_position+(cva*3) + 6)] = quads[t].color.a;
      arr[(start_position+(cva*3) + 7)] = quads[t].frame_id;
      arr[(start_position+(cva*3) + 8)] = quads[t].v.v4_tx_x;
      arr[(start_position+(cva*3) + 9)] = quads[t].v.v4_tx_y;
      arr[(start_position+(cva*3) + 10)] = quads[t].texture_id;
      arr[(start_position+(cva*3) + 11)] = quads[t].is_clicked;
      arr[(start_position+(cva*3) + 12)] = quads[t].object_type_id;
      arr[(start_position+(cva*3) + 13)] = quads[t].camera_type;

    }
  }

  void _make_index_array_from_quads(std::vector<quads::QuadData>& quads, unsigned int* arr)
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

  void _make_vertex_array_from_3d_meshes(std::vector<models::ModelMeshVertexData> &mesh_vertices, float* arr)
  {
    int cva = buffer::COUNT_VERTEX_ATTRIBUTES; 
    for(int i=0; i<mesh_vertices.size(); i++)
    {
      int start_position = i*cva;
      arr[(start_position)] = mesh_vertices[i].x;
      arr[(start_position+1)] = mesh_vertices[i].y;
      arr[(start_position+2)] = mesh_vertices[i].z;
      arr[(start_position+3)] = mesh_vertices[i].r;
      arr[(start_position+4)] = mesh_vertices[i].g;
      arr[(start_position+5)] = mesh_vertices[i].b;
      arr[(start_position+6)] = mesh_vertices[i].a;
      arr[(start_position+7)] = mesh_vertices[i].frame_id;
      arr[(start_position+8)] = mesh_vertices[i].tx_x;
      arr[(start_position+9)] = mesh_vertices[i].tx_y;
      arr[(start_position+10)] = mesh_vertices[i].texture_id;
      arr[(start_position+11)] = mesh_vertices[i].is_clicked;
      arr[(start_position+12)] = mesh_vertices[i].object_type;
      arr[(start_position+13)] = mesh_vertices[i].camera_type;
    }
  }


  void _make_index_array_from_3d_meshes(std::vector<models::ModelMeshData> &meshes, unsigned int* arr)
  {
    int offset = 0;
    for(int m=0; m<meshes.size(); m++)
    {
      for(int i=0; i < meshes[m].indices.size(); i++)
      {
        arr[offset] = meshes[m].indices[i];
        offset++;
      }
    }
  }

  void _make_vertex_array_from_lines(std::vector<debug::LineData>& lines, float* arr)
  {
    int cva = buffer::COUNT_VERTEX_ATTRIBUTES; 
    for(int t=0; t<lines.size(); t++)
    {
      int start_position = t*cva*2;
      arr[(start_position)] = lines[t].x1;
      arr[(start_position+1)] = lines[t].y1;
      arr[(start_position+2)] = 0.0f; // z
      arr[(start_position+3)] = lines[t].r;
      arr[(start_position+4)] = lines[t].g;
      arr[(start_position+5)] = lines[t].b;
      arr[(start_position+6)] = lines[t].a;
      arr[(start_position+7)] = 0; //frame_id
      arr[(start_position+8)] = 0; // tx_x
      arr[(start_position+9)] = 0; // tx_y
      arr[(start_position+10)] = 0; // texture_id
      arr[(start_position+11)] = 0; // is_clicked
      arr[(start_position+12)] = OBJECT_TYPE_DEBUG;
      arr[(start_position+13)] = CAMERA_DYNAMIC;

      arr[(start_position+cva)] = lines[t].x2;
      arr[(start_position+(cva+1))] = lines[t].y2;
      arr[(start_position+(cva+2))] = 0.0f; // z
      arr[(start_position+(cva+3))] = lines[t].r;
      arr[(start_position+(cva+4))] = lines[t].g;
      arr[(start_position+(cva+5))] = lines[t].b;
      arr[(start_position+(cva+6))] = lines[t].a;
      arr[(start_position+(cva+7))] = 0; //frame_id
      arr[(start_position+(cva+8))] = 0; // tx_x
      arr[(start_position+(cva+9))] = 0; // tx_y
      arr[(start_position+(cva+10))] = 0; // texture_id
      arr[(start_position+(cva+11))] = 0; // is_clicked
      arr[(start_position+(cva+12))] = OBJECT_TYPE_DEBUG;
      arr[(start_position+(cva+13))] = CAMERA_DYNAMIC;
    }
  }

  void update_quads(std::vector<quads::QuadData>& quads)
  {
    int n_vertex_array = buffer::COUNT_VERTEX_ATTRIBUTES*quads.size()*4;
    float vertex_array[n_vertex_array];
    int n_index_array = 3*quads.size()*2;
    unsigned int index_array[n_index_array];

    buffer::_make_vertex_array_from_quads(quads, vertex_array);
    buffer::_make_index_array_from_quads(quads, index_array);

    //buffer::log(vertex_array, n_vertex_array);

    buffer::VBO_array_size = sizeof(float)*n_vertex_array;
    buffer::EBO_array_size = sizeof(float)*n_index_array;

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, buffer::VBO_array_size, vertex_array);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, buffer::EBO_array_size, index_array);
  }

  void update_lines(std::vector<debug::LineData>& lines)
  {
    int n_vertex_array = buffer::COUNT_VERTEX_ATTRIBUTES*lines.size()*2;
    float vertex_array[n_vertex_array];
    buffer::VBO_array_size = sizeof(float)*n_vertex_array;
    buffer::_make_vertex_array_from_lines(lines, vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, buffer::VBO_array_size, vertex_array);
  }

  void update_models(std::vector<models::ModelMeshVertexData>& mesh_vertices, std::vector<models::ModelMeshData>& meshes)
  {
    int n_vertex_array = buffer::COUNT_VERTEX_ATTRIBUTES*mesh_vertices.size();
    float vertex_array[n_vertex_array];

    int n_index_array = mesh_vertices.size();
    unsigned int index_array[n_index_array];

    buffer::VBO_array_size = sizeof(float)*n_vertex_array;
    buffer::EBO_array_size = sizeof(float)*n_index_array;

    //buffer::_make_vertex_array_from_3d_meshes(mesh_vertices, vertex_array);
    buffer::_make_index_array_from_3d_meshes(meshes, index_array);

    buffer::log(vertex_array, n_vertex_array, buffer::COUNT_VERTEX_ATTRIBUTES, "model_vertex_array");
    buffer::log(index_array, n_index_array, 3, "model_index_array");

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, buffer::VBO_array_size, vertex_array);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, buffer::EBO_array_size, index_array);
  }





  void drop()
  {
    glDeleteVertexArrays(1, &buffer::VAO);
    glDeleteBuffers(1, &buffer::VBO);
    glDeleteBuffers(1, &buffer::EBO);
  }


  template <typename T>
  //void log(float* arr, int arr_size, int row_len, const char* arr_name);
  void log(T* arr, int arr_size, int row_len, const char* arr_name)
  {
    std::string log_path_str = "logs/";
    log_path_str += arr_name;
    log_path_str += ".txt";
    const char *log_path = log_path_str.c_str();

    std::ofstream data_file (log_path);
    if (data_file.is_open())
    {
      for(int i = 0; i < arr_size; i++)
      {

        //if(i % buffer::COUNT_VERTEX_ATTRIBUTES == 0 && i !=0 )
        if(i % row_len == 0 && i != 0)
        {
          data_file << " \n";
        }
        data_file << arr[i] << " ";
        //std::cout << arr[i] << " ";
      }
      data_file.close();
    }
  }




}
