#include <vector>
#include <map>

#include "collisions.h"


#ifndef MODULES_QUAD_H
#define MODULES_QUAD_H

// Frame data. Table of quads and all operations on this table
namespace quads
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

    // normalized start and normalized end for textures
    float norm_x_start, norm_x_end;
    float norm_y_start, norm_y_end;

    bool is_clicked;

  };

  // Manages range of quads per object it and switches the reset of them 
  struct QuadSetting
  {
    int min_index;
    int max_index;
    bool needs_reset;
  };

  extern const int MAX_QUADS;
  extern quads::QuadData quads[];
  extern std::vector<QuadData> AllQuads;
  extern float VERTEX_OFFSET;
  extern int COUNT_QUADS;
  extern int REQ_SIZE_BUFFER;

  // Contains quads settings per object_type_id
  extern std::map<int, quads::QuadSetting> QuadsManager;

  // Setup of Quads manager
  void init();

  // Takes some quad information and produces vertex data struct to be added to quad;
  VertexData _fill_quad_vertex_data(quads::QuadData& q, int n);

  // Finds next available quad id
  int _find_next_quad_id();

  // Generate new quad id
  int gen_quad_id();

  // Clear all quads data
  void clear();

  // Accumulate all quad vectors from different components
  void accumulate();

  // Makes quads out of the object catalog data - entities, text, menu, debug, gui, maps etc.
  template <typename T>
  std::vector<quads::QuadData> make_quads(std::map<int, T> data, int object_type_id);

  // Writes down the quads data to ./logs/all_quads.json on every frame
  void log();

}

#endif