#ifndef BUFFER_H
#define BUFFER_H

namespace buffer
{

  unsigned int VBO, VAO, EBO;
  // Size of VBO buffer -> size of Vertex: 120*4, 4 vertices per quad, max quads is 2000. Buffer size is 960 000
  int VBO_size = MAX_QUADS*sizeof(quads::Vertex)*4;
  int VBO_array_size = 0;
  float VBO_buffer_usage = 0.0f;

  int EBO_size = MAX_QUADS*sizeof(float)*6;
  int EBO_array_size = 0;
  float EBO_buffer_usage = 0.0f;

  void init(std::vector<quads::Quad> quads)
  {
    // generate vertices array out of tiles struct
    int n_quads = quads.size();
    int n_vertices = n_quads*4;
    int vertices_array_count = quads::COUNT_VERTEX_ATTRIBUTES*n_vertices;
    float vertices_array[vertices_array_count];

    //std::cout << "Vertices array count: " << vertices_array_count << std::endl;
    //std::cout << "Vertex size: " << sizeof(quads::Vertex) << std::endl;
    //std::cout << "Vertices array size: " << sizeof(quads::Vertex)*n_vertices << std::endl;
    //std::cout << "Vertices array size (float based) " << sizeof(float)*vertices_array_count << std::endl;
    //std::cout << "Quads count: " << n_quads << std::endl;
    //std::cout << "Vertex count: " << n_vertices << std::endl;
    //std::cout << "VBO size: " << buffer::VBO_size << std::endl;
    //std::cout << "EBO size: " << buffer::EBO_size << std::endl;



    for(int t=0; t<n_quads; t++)
    {
      int start_position = t*quads::COUNT_VERTEX_ATTRIBUTES*4;
      int cva = quads::COUNT_VERTEX_ATTRIBUTES;

      vertices_array[(start_position)] = quads[t].v_a.x_pos;
      vertices_array[(start_position+1)] = quads[t].v_a.y_pos;
      vertices_array[(start_position+2)] = quads[t].v_a.z_pos;
      vertices_array[(start_position+3)] = quads[t].v_a.r_col;
      vertices_array[(start_position+4)] = quads[t].v_a.g_col;
      vertices_array[(start_position+5)] = quads[t].v_a.b_col;
      vertices_array[(start_position+6)] = quads[t].v_a.a_col;
      vertices_array[(start_position+7)] = quads[t].v_a.frame_id;
      vertices_array[(start_position+8)] = quads[t].v_a.tex_coord_x;
      vertices_array[(start_position+9)] = quads[t].v_a.tex_coord_y;
      vertices_array[(start_position+10)] = quads[t].v_a.texture_id;
      vertices_array[(start_position+11)] = quads[t].v_a.is_clicked;
      vertices_array[(start_position+12)] = quads[t].v_a.type_id;
      vertices_array[(start_position+13)] = quads[t].v_a.is_static;

      vertices_array[(start_position+cva)] = quads[t].v_b.x_pos;
      vertices_array[(start_position+(cva+1))] = quads[t].v_b.y_pos;
      vertices_array[(start_position+(cva+2))] = quads[t].v_b.z_pos;
      vertices_array[(start_position+(cva+3))] = quads[t].v_b.r_col;
      vertices_array[(start_position+(cva+4))] = quads[t].v_b.g_col;
      vertices_array[(start_position+(cva+5))] = quads[t].v_b.b_col;
      vertices_array[(start_position+(cva+6))] = quads[t].v_b.a_col;
      vertices_array[(start_position+(cva+7))] = quads[t].v_b.frame_id;
      vertices_array[(start_position+(cva+8))] = quads[t].v_b.tex_coord_x;
      vertices_array[(start_position+(cva+9))] = quads[t].v_b.tex_coord_y;
      vertices_array[(start_position+(cva+10))] = quads[t].v_b.texture_id;
      vertices_array[(start_position+(cva+11))] = quads[t].v_b.is_clicked;
      vertices_array[(start_position+(cva+12))] = quads[t].v_b.type_id;
      vertices_array[(start_position+(cva+13))] = quads[t].v_b.is_static;

      vertices_array[(start_position+(cva*2))] = quads[t].v_c.x_pos;
      vertices_array[(start_position+(cva*2) + 1)] = quads[t].v_c.y_pos;
      vertices_array[(start_position+(cva*2) + 2)] = quads[t].v_c.z_pos;
      vertices_array[(start_position+(cva*2) + 3)] = quads[t].v_c.r_col;
      vertices_array[(start_position+(cva*2) + 4)] = quads[t].v_c.g_col;
      vertices_array[(start_position+(cva*2) + 5)] = quads[t].v_c.b_col;
      vertices_array[(start_position+(cva*2) + 6)] = quads[t].v_c.a_col;
      vertices_array[(start_position+(cva*2) + 7)] = quads[t].v_c.frame_id;
      vertices_array[(start_position+(cva*2) + 8)] = quads[t].v_c.tex_coord_x;
      vertices_array[(start_position+(cva*2) + 9)] = quads[t].v_c.tex_coord_y;
      vertices_array[(start_position+(cva*2) + 10)] = quads[t].v_c.texture_id;
      vertices_array[(start_position+(cva*2) + 11)] = quads[t].v_c.is_clicked;
      vertices_array[(start_position+(cva*2) + 12)] = quads[t].v_c.type_id;
      vertices_array[(start_position+(cva*2) + 13)] = quads[t].v_c.is_static;

      vertices_array[(start_position+(cva*3))] = quads[t].v_d.x_pos;
      vertices_array[(start_position+(cva*3) + 1)] = quads[t].v_d.y_pos;
      vertices_array[(start_position+(cva*3) + 2)] = quads[t].v_d.z_pos;
      vertices_array[(start_position+(cva*3) + 3)] = quads[t].v_d.r_col;
      vertices_array[(start_position+(cva*3) + 4)] = quads[t].v_d.g_col;
      vertices_array[(start_position+(cva*3) + 5)] = quads[t].v_d.b_col;
      vertices_array[(start_position+(cva*3) + 6)] = quads[t].v_d.a_col;
      vertices_array[(start_position+(cva*3) + 7)] = quads[t].v_d.frame_id;
      vertices_array[(start_position+(cva*3) + 8)] = quads[t].v_d.tex_coord_x;
      vertices_array[(start_position+(cva*3) + 9)] = quads[t].v_d.tex_coord_y;
      vertices_array[(start_position+(cva*3) + 10)] = quads[t].v_d.texture_id;
      vertices_array[(start_position+(cva*3) + 11)] = quads[t].v_d.is_clicked;
      vertices_array[(start_position+(cva*3) + 12)] = quads[t].v_d.type_id;
      vertices_array[(start_position+(cva*3) + 13)] = quads[t].v_d.is_static;
    }
    //utils::array_to_file("buffer_init_vertex_array", vertices_array, vertices_array_count, quads::COUNT_VERTEX_ATTRIBUTES);
    

    // generate indices array out of vector of Indices:
    int n_vindices = n_quads*2;
    int vindices_array_count = 3*n_vindices; // its always 3 as it is a triangle
    unsigned int vindices_array[vindices_array_count];

    for(int t=0; t<n_quads; t++)
    {
      int start_position = t*3*2;
      vindices_array[(start_position+0)] = quads[t].i_left.a;
      vindices_array[(start_position+1)] = quads[t].i_left.b;
      vindices_array[(start_position+2)] = quads[t].i_left.c;

      vindices_array[(start_position+3)] = quads[t].i_right.a;
      vindices_array[(start_position+4)] = quads[t].i_right.b;
      vindices_array[(start_position+5)] = quads[t].i_right.c;
    }

    //utils::array_to_file("buffer_init_index_array", vindices_array, vindices_array_count, 3);

    GlCall(glEnable(GL_BLEND));
    GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
      
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // dynamic approach:
    glBufferData(GL_ARRAY_BUFFER, buffer::VBO_size, nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer::EBO_size, nullptr, GL_DYNAMIC_DRAW));

    // new version:
    // position attribute:
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, quads::COUNT_VERTEX_ATTRIBUTES * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // enable this attribute at the end

    // color attribute:
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, quads::COUNT_VERTEX_ATTRIBUTES * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1); // enable this attribute at the end

    // frame_id attribute
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, quads::COUNT_VERTEX_ATTRIBUTES * sizeof(float), (void*)(7 * sizeof(float)));
    glEnableVertexAttribArray(2); // enable this attribute at the end

    // in texture coordinates attribute
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, quads::COUNT_VERTEX_ATTRIBUTES * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(3); // enable this attribute at the end

    // in texture id attribute
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, quads::COUNT_VERTEX_ATTRIBUTES * sizeof(float), (void*)(10 * sizeof(float)));
    glEnableVertexAttribArray(4); // enable this attribute at the end

    // is_clicked  attribute
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, quads::COUNT_VERTEX_ATTRIBUTES * sizeof(float), (void*)(11 * sizeof(float)));
    glEnableVertexAttribArray(5); // enable this attribute at the end

    // quad type id  attribute
    glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, quads::COUNT_VERTEX_ATTRIBUTES * sizeof(float), (void*)(12 * sizeof(float)));
    glEnableVertexAttribArray(6); // enable this attribute at the end

    // is_static attribute
    glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, quads::COUNT_VERTEX_ATTRIBUTES * sizeof(float), (void*)(13 * sizeof(float)));
    glEnableVertexAttribArray(7); // enable this attribute at the end



    // position attribute
    /*
    glVertexAttribPointer(
       index - index of the generic vertex attribute, 
       size - count of elements per vertex,
       type - type of element, for example float
       normalize -  true/false, not needed for float
       stride - byte offset, amount of bytes between each vertex (size of each vertex)
       pointer - offset of attribute within vertex
       )
    */
  }

  void update(std::vector<quads::Quad> quads)
  {
    int n_quads = quads.size();
    int n_vertices = n_quads*4;
    int vertices_array_count = quads::COUNT_VERTEX_ATTRIBUTES*n_vertices;
    float vertices_array[vertices_array_count];

    for(int t=0; t<n_quads; t++)
    {
      // std::cout << "buffering quad" << quads[t].id << std::endl;
      // std::cout << t << std::endl;
      // std::cout << "" << std::endl;

      int start_position = t*quads::COUNT_VERTEX_ATTRIBUTES*4;
      int cva = quads::COUNT_VERTEX_ATTRIBUTES; 

      vertices_array[(start_position)] = quads[t].v_a.x_pos;
      vertices_array[(start_position+1)] = quads[t].v_a.y_pos;
      vertices_array[(start_position+2)] = quads[t].v_a.z_pos;
      vertices_array[(start_position+3)] = quads[t].v_a.r_col;
      vertices_array[(start_position+4)] = quads[t].v_a.g_col;
      vertices_array[(start_position+5)] = quads[t].v_a.b_col;
      vertices_array[(start_position+6)] = quads[t].v_a.a_col;
      vertices_array[(start_position+7)] = quads[t].v_a.frame_id;
      vertices_array[(start_position+8)] = quads[t].v_a.tex_coord_x;
      vertices_array[(start_position+9)] = quads[t].v_a.tex_coord_y;
      vertices_array[(start_position+10)] = quads[t].v_a.texture_id;
      vertices_array[(start_position+11)] = quads[t].v_a.is_clicked;
      vertices_array[(start_position+12)] = quads[t].v_a.type_id;
      vertices_array[(start_position+13)] = quads[t].v_a.is_static;

      vertices_array[(start_position+cva)] = quads[t].v_b.x_pos;
      vertices_array[(start_position+(cva+1))] = quads[t].v_b.y_pos;
      vertices_array[(start_position+(cva+2))] = quads[t].v_b.z_pos;
      vertices_array[(start_position+(cva+3))] = quads[t].v_b.r_col;
      vertices_array[(start_position+(cva+4))] = quads[t].v_b.g_col;
      vertices_array[(start_position+(cva+5))] = quads[t].v_b.b_col;
      vertices_array[(start_position+(cva+6))] = quads[t].v_b.a_col;
      vertices_array[(start_position+(cva+7))] = quads[t].v_b.frame_id;
      vertices_array[(start_position+(cva+8))] = quads[t].v_b.tex_coord_x;
      vertices_array[(start_position+(cva+9))] = quads[t].v_b.tex_coord_y;
      vertices_array[(start_position+(cva+10))] = quads[t].v_b.texture_id;
      vertices_array[(start_position+(cva+11))] = quads[t].v_b.is_clicked;
      vertices_array[(start_position+(cva+12))] = quads[t].v_b.type_id;
      vertices_array[(start_position+(cva+13))] = quads[t].v_b.is_static;

      vertices_array[(start_position+(cva*2))] = quads[t].v_c.x_pos;
      vertices_array[(start_position+(cva*2) + 1)] = quads[t].v_c.y_pos;
      vertices_array[(start_position+(cva*2) + 2)] = quads[t].v_c.z_pos;
      vertices_array[(start_position+(cva*2) + 3)] = quads[t].v_c.r_col;
      vertices_array[(start_position+(cva*2) + 4)] = quads[t].v_c.g_col;
      vertices_array[(start_position+(cva*2) + 5)] = quads[t].v_c.b_col;
      vertices_array[(start_position+(cva*2) + 6)] = quads[t].v_c.a_col;
      vertices_array[(start_position+(cva*2) + 7)] = quads[t].v_c.frame_id;
      vertices_array[(start_position+(cva*2) + 8)] = quads[t].v_c.tex_coord_x;
      vertices_array[(start_position+(cva*2) + 9)] = quads[t].v_c.tex_coord_y;
      vertices_array[(start_position+(cva*2) + 10)] = quads[t].v_c.texture_id;
      vertices_array[(start_position+(cva*2) + 11)] = quads[t].v_c.is_clicked;
      vertices_array[(start_position+(cva*2) + 12)] = quads[t].v_c.type_id;
      vertices_array[(start_position+(cva*2) + 13)] = quads[t].v_c.is_static;

      vertices_array[(start_position+(cva*3))] = quads[t].v_d.x_pos;
      vertices_array[(start_position+(cva*3) + 1)] = quads[t].v_d.y_pos;
      vertices_array[(start_position+(cva*3) + 2)] = quads[t].v_d.z_pos;
      vertices_array[(start_position+(cva*3) + 3)] = quads[t].v_d.r_col;
      vertices_array[(start_position+(cva*3) + 4)] = quads[t].v_d.g_col;
      vertices_array[(start_position+(cva*3) + 5)] = quads[t].v_d.b_col;
      vertices_array[(start_position+(cva*3) + 6)] = quads[t].v_d.a_col;
      vertices_array[(start_position+(cva*3) + 7)] = quads[t].v_d.frame_id;
      vertices_array[(start_position+(cva*3) + 8)] = quads[t].v_d.tex_coord_x;
      vertices_array[(start_position+(cva*3) + 9)] = quads[t].v_d.tex_coord_y;
      vertices_array[(start_position+(cva*3) + 10)] = quads[t].v_d.texture_id;
      vertices_array[(start_position+(cva*3) + 11)] = quads[t].v_d.is_clicked;
      vertices_array[(start_position+(cva*3) + 12)] = quads[t].v_d.type_id;
      vertices_array[(start_position+(cva*3) + 13)] = quads[t].v_d.is_static;
    }

    // utils::array_to_file("buffer_update_vertex_array", vertices_array, vertices_array_count, quads::COUNT_VERTEX_ATTRIBUTES);
    quads::all_quads_to_json();

    buffer::VBO_array_size = sizeof(float)*vertices_array_count;
    buffer::VBO_buffer_usage = std::round(((float)VBO_array_size/(float)VBO_size) * 1000.0)/1000.0;

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, buffer::VBO_array_size, vertices_array);

    int n_vindices = n_quads*2;
    int vindices_array_count = 3*n_vindices; // its always 3 as it is a triangle
    unsigned int vindices_array[vindices_array_count];

    for(int t=0; t<n_quads; t++)
    {
      int start_position = t*3*2;
      vindices_array[(start_position+0)] = quads[t].i_left.a;
      vindices_array[(start_position+1)] = quads[t].i_left.b;
      vindices_array[(start_position+2)] = quads[t].i_left.c;

      vindices_array[(start_position+3)] = quads[t].i_right.a;
      vindices_array[(start_position+4)] = quads[t].i_right.b;
      vindices_array[(start_position+5)] = quads[t].i_right.c;
    }
    // utils::array_to_file("buffer_update_index_array", vindices_array, vindices_array_count, 3);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    buffer::EBO_array_size = sizeof(float)*vindices_array_count;
    buffer::EBO_buffer_usage = std::round(((float)EBO_array_size/(float)EBO_size) * 1000.0)/1000.0;
    GlCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, buffer::EBO_array_size, vindices_array));

  }





  void drop()
  {
    glDeleteVertexArrays(1, &buffer::VAO);
    glDeleteBuffers(1, &buffer::VBO);
    glDeleteBuffers(1, &buffer::EBO);
  }

}



#endif