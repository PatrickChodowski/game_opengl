
#ifndef VARS_H
#define VARS_H

// file for defining variables that should be accessible by multiple files 



namespace quads
{

  #define QUAD_TYPE_LEVEL 0.0f
  #define QUAD_TYPE_MENU 1.0f
  #define QUAD_TYPE_TEXT 2.0f


  
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

  // next quad id to assign, should start from 0, but the next function call will return 1;
  int next_quad_id = 0;
}





#endif