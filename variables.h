
#ifndef VARS_H
#define VARS_H

// file for defining variables that should be accessible by multiple files 



namespace quads
{

  #define QUAD_TYPE_LEVEL 0.0f
  #define QUAD_TYPE_MENU 1.0f
  #define QUAD_TYPE_TEXT 2.0f

  int COUNT_VERTEX_ATTRIBUTES = 13;
  int VERTEX_OFFSET = 1;



  
  // vertex index
  struct Vindex
  {  
    int a; 
    int b;
    int c;
  };


  // Vertex information
  struct Vertex
  {  
    // info send to GPU:
    double x_pos; 
    double y_pos;
    double z_pos;

    double r_col;
    double g_col;
    double b_col;
    double a_col;

    double frame_id;
    double tex_coord_x;
    double tex_coord_y;
    double texture_id;

    double is_clicked;
    double type_id;

    /// not counted as VERTEX ATTRIBUTES
    int tile_id;
    int vertex_id;
  };

 


  // Quad will contain information about:
  // - original position (x,y of top left corner)
  // - type, solid, id
  // - vertex ids for a,b,c,d
  // - Vindices with vertex ids
  // - actual vertex information

  struct Quad
  {
    int id;
    int x;
    int y;
    int w;
    int h;
    bool solid;
    bool is_clicked;

    // vertex IDS:
    int a;
    int b;
    int c;
    int d;

    // a b
    // c d

    // Vertex information
    Vertex v_a;
    Vertex v_b;
    Vertex v_c;
    Vertex v_d;

    // vindices
    struct Vindex i_left;
    // a b
    // c
    struct Vindex i_right;
    //   b
    // c d

    // for assigning texture and frame in assign_vertices:
    int texture_id;
    int frame_id;

    // for passing colors
    float r_col;
    float g_col;
    float b_col;
    float a_col;
    
    // for different shader calls:
    float type_id; // 0 - level, 1 - menu, 2 - text


  };

  struct ScaledQuad
  {
    int id;
    float x;
    float y;
    float w;
    float h;
  };


  void print_out_quads(std::vector<Quad> quads)
  {
    for(int t=0; t<quads.size(); t++)
    {
      std::cout << "Quad ID: " << quads[t].id << std::endl; 
      std::cout << "Texture ID: " << quads[t].texture_id << std::endl;
      std::cout << "Frame ID: " << quads[t].frame_id << std::endl;
      std::cout << "X: " << quads[t].x << std::endl; 
      std::cout << "Y: " << quads[t].y << std::endl; 
      std::cout << "Width: " << quads[t].w << std::endl; 
      std::cout << "Height: " << quads[t].h << std::endl; 
      std::cout << "Type: " << quads[t].type_id << std::endl; 

      std::cout << "Vertices positions:"  << std::endl;
      std::cout << "Vertex ID: " << quads[t].a << " X:" << quads[t].v_a.x_pos << ", Y:" << quads[t].v_a.y_pos << ", tcX:" << quads[t].v_a.tex_coord_x << ", tcY:" << quads[t].v_a.tex_coord_y << std::endl;
      std::cout << "Vertex ID: " << quads[t].b << " X:" << quads[t].v_b.x_pos << ", Y:" << quads[t].v_b.y_pos << ", tcX:" << quads[t].v_b.tex_coord_x << ", tcY:" << quads[t].v_b.tex_coord_y << std::endl;
      std::cout << "Vertex ID: " << quads[t].c << " X:" << quads[t].v_c.x_pos << ", Y:" << quads[t].v_c.y_pos << ", tcX:" << quads[t].v_c.tex_coord_x << ", tcY:" << quads[t].v_c.tex_coord_y << std::endl;
      std::cout << "Vertex ID: " << quads[t].d << " X:" << quads[t].v_d.x_pos << ", Y:" << quads[t].v_d.y_pos << ", tcX:" << quads[t].v_d.tex_coord_x << ", tcY:" << quads[t].v_d.tex_coord_y << std::endl;

      std::cout << "Vindices:" << std::endl <<
      quads[t].i_left.a << " " << quads[t].i_left.b << " " << quads[t].i_left.c << std::endl <<
      quads[t].i_right.a << " " << quads[t].i_right.b << " " << quads[t].i_right.c << std::endl;
    }
  }
}


namespace fonts
{
  std::vector<quads::Quad> TextQuads;
}



namespace levels
{
  std::vector<quads::Quad> LevelQuads;
  std::vector<quads::ScaledQuad> ScaledLevelQuads;
}

namespace menu
{
  std::vector<quads::Quad> MenuQuads;
}

namespace qm 
{
  /*
    AllQuads will be a vector containing all quads to be sent to buffer on each iteration.
    It gathers levels::LevelQuads, menu::MenuQuads, fonts::TextQuads into one chunk of information to be send on the update buffer

    Meta information about quads sent will be stored in map Quads Summary - currently only name of the type and size
  */
  std::vector<quads::Quad> AllQuads;
  std::map<std::string, int> QuadsSummary;
  std::vector<int> UsedQuadIds = {};
  std::vector<int> UsedVertexIds = {};

  int find_next_quad_id()
  {
    /*
      Algorithm to find next available quad id 
    */
    
    int n = qm::UsedQuadIds.size();
    bool found = false;

    // for whole vector, find value that would be bigger than (index + 1)
    for (int i = 0; i < n; i++)
    {
      if (qm::UsedQuadIds[i] > (i+1)){
        return i+1;
      }
    }
    return n+1;
  }

  int find_next_vertex_id()
  {
    /*
      Algorithm to find next available vertex id 
    */
    
    int n = qm::UsedVertexIds.size();
    bool found = false;

    // for whole vector, find value that would be bigger than (index + 1)
    for (int i = 0; i < n; i++)
    {
      if (qm::UsedVertexIds[i] > i){
        return i;
      }
    }
    return n;
  }


  int gen_quad_id()
  {
    int next_quad_id = qm::find_next_quad_id();
    std::cout << "Next Quad id: " << next_quad_id << std::endl;
    qm::UsedQuadIds.push_back(next_quad_id);
    return next_quad_id;
  }

   int gen_vertex_id()
  {
    int next_vertex_id = qm::find_next_vertex_id();
    std::cout << "Next vertex id: " << next_vertex_id << std::endl;
    qm::UsedVertexIds.push_back(next_vertex_id);
    return next_vertex_id;
  }
}





#endif