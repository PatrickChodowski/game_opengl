#include <vector>
#include <map>
#include "collisions.h"


#ifndef MODULES_QUAD_H
#define MODULES_QUAD_H

// Frame data. Table of quads and all operations on this table
namespace quads2
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

    int object_id;
 
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
    float object_type_id;
    float camera_type;

    // Window dimensions and coordinates for presenting in the window
    float window_x, window_y;
    float window_w, window_h;

    // norm x start and norm x end for text textures
    float norm_x_start, norm_x_end;

    bool is_clicked;

  };

  extern std::vector<QuadData> AllQuads;
  extern std::vector<int> UsedQuadIds;
  extern std::vector<int> UsedVertexIds;
  extern int COUNT_VERTEX_ATTRIBUTES;
  extern float VERTEX_OFFSET;
  extern int COUNT_QUADS;
  extern int REQ_SIZE_BUFFER;

  // Takes some quad information and produces vertex data struct to be added to quad;
  VertexData _fill_quad_vertex_data(quads2::QuadData& q);

  // Finds next available quad id
  int _find_next_quad_id();

  // Finds next available vertex id
  int _find_next_vertex_id();

  // Generate new quad id
  int gen_quad_id();

  // Generate new vertex id
  int gen_vertex_id();

  // Delete quad id from used quads
  void delete_quad_id(int quad_id);

  // Delete vertex id from used vertices
  void delete_vertex_id(int vertex_id);

  // Clear out quads vector along with used Quad ID and used Vertex ID
  void clear_quads_data(std::vector<quads2::QuadData>& vq);

  // Finds index of quad id in vector of quads
  int find_quad_id(int quad_id, std::vector<quads2::QuadData> quads);

  // Accumulate all quad vectors from different components
  void accumulate();

  // Makes quads out of the object catalog data - entities, text, menu, debug, gui, maps etc.
  template <typename T>
  std::vector<quads2::QuadData> make_quads(std::map<int, T> data, int object_type_id);

  // Writes down the quads data to ./logs/all_quads.json on every frame
  void log();

}

#endif