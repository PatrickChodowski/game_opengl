#include <vector>
#include <map>
#include "collisions.h"

#ifndef MODULES_QUAD_H
#define MODULES_QUAD_H

// Frame data. Table of quads and all operations on this table
namespace quad
{
  // Struct containing only vertex specific data - locations and texture querying params
  struct VertexData
  {
    int v1_id, v2_id, v3_id, v4_id;

    double v1_x, v1_y, v1_z;
    double v1_tx_x, v1_tx_y;
    
    double v2_x, v2_y, v2_z;
    double v2_tx_x, v2_tx_y;

    double v3_x, v3_y, v3_z;
    double v3_tx_x, v3_tx_y;

    double v4_x, v4_y, v4_z;
    double v4_tx_x, v4_tx_y;

  };

  // Set of Vertex IDs to form index
  struct VertexIndex
  {  
    int v1, v2, v3; 
  };

  // All data needed by buffer and shaders to render image properly
  struct QuadData
  {
    int id;
    int texture_id;
    int frame_id;

    bool is_clicked;

    // a b
    // c
    struct VertexIndex i_left;

    //   b
    // c d
    struct VertexIndex i_right;

    // Vertex specific data
    struct VertexData v;

    // Quad color
    double r, g, b, a;

    // Quad position
    float x, y, z;

    // Quad dimensions
    float w, h;

    // Quad type: 0 - level, 1 - menu, 2 - text, 3 - entity
    float type_id;

    // To choose camera behaviour
    float is_static;

    // Scaled metrics For collisions and mouse events:
    float s_x, s_y;
    float s_w, s_h;

  };

  extern std::vector<QuadData> AllQuads;
  extern std::vector<int> UsedQuadIds;
  extern std::vector<int> UsedVertexIds;
  extern int COUNT_VERTEX_ATTRIBUTES;
  extern float VERTEX_OFFSET;
  extern int COUNT_QUADS;
  extern int REQ_SIZE_BUFFER;

  // Takes some quad information and produces vertex data struct to be added to quad;
  VertexData _fill_quad_vertex_data(QuadData& q);

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