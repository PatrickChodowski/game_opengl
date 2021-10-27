#include <vector>
#include <map>
#include "collisions.h"

#ifndef MODULES_QUAD_H
#define MODULES_QUAD_H

// Frame data. Table of quads and all operations on this table
namespace quad
{
  struct VertexData
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
    double is_static;

    /// not counted as VERTEX ATTRIBUTES
    int tile_id;
    int vertex_id;
  };

  struct VertexIndex
  {  
    int a; 
    int b;
    int c;
  };

  struct QuadLabel
  {
    std::string text;
    int x;
    int y;
    bool is_static;
    float scale;
  };

  struct QuadData
  {
    int id;
    int x;
    int y;
    int w;
    int h;
    
    // vertex IDS:
    int a;
    int b;
    int c;
    int d;
    // a b
    // c d

    // for assigning texture and frame in assign_vertices:
    int texture_id;
    int frame_id;

    // for entities logic:
    int entity_type_id;


    bool solid; // if true, will get collision with hero entity
    bool coll; // if true, will get distance to hero calculated
    bool is_clicked;

    // Vertex information
    VertexData v_a;
    VertexData v_b;
    VertexData v_c;
    VertexData v_d;

    // VertexIndex
    struct VertexIndex i_left;
    // a b
    // c
    struct VertexIndex i_right;
    //   b
    // c d
    
    // for passing colors
    float r_col;
    float g_col;
    float b_col;
    float a_col;

    // for different shader calls:
    float type_id; // 0 - level, 1 - menu, 2 - text, 3 - entity

    // to choose camera reaction:
    float is_static;

    // Scaled metrics For collisions and mouse events:
    float s_x;
    float s_y;
    float s_w;
    float s_h;

    // 'diagonal' is the distance from the middle of the quad to the corner
    // would call it a radius but we are in a quad not a circle, but its like a radius to me
    float s_diag; // 'diagonal' is the distance from the center of the quad to the corner

    // used if given entity has collision sensors
    std::map<int, collisions::Sensor> sensors;

    // its not abs, its AABBs, but this is what I do to entertain myself
    std::map<int, collisions::AABB> abs; 

    // label collection
    std::vector<QuadLabel> labels;
  };

  extern std::vector<QuadData> AllQuads;
  extern std::vector<int> UsedQuadIds;
  extern std::vector<int> UsedVertexIds;
  extern int COUNT_VERTEX_ATTRIBUTES;
  extern int VERTEX_OFFSET;
  extern int COUNT_QUADS;
  extern int REQ_SIZE_BUFFER;

  // Finds next available quad id
  int _find_next_quad_id();

  // Finds next available vertex id
  int _find_next_vertex_id();

  // Generate new quad id
  int gen_quad_id();

  // Generate new vertex id
  int gen_vertex_id();

  // Finds index of quad id in vector of quads
  int find_quad_id(int quad_id, std::vector<quad::QuadData> quads);

  // Find distance between quads
  float get_distance_between_quads(QuadData q1, QuadData q2);

  // Accumulate all quad vectors from different components
  void accumulate();
}

#endif