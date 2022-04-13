
#include <array>
#include <vector>
#include "../dependencies/parallel_hashmap/phmap.h"

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
    int entity_id;
    int texture_id;
    int frame_id;
    int entity_type_id;
    int model_id;
 
    // a b
    // c
    struct VertexIndex i_left;

    //   b
    // c d
    struct VertexIndex i_right;

    // Vertex specific data
    struct VertexData v;

    float r, g, b, a;
    float x, y, z;
    float w, h;
    float norm_x_start = 0.0f;
    float norm_x_end = 1.0f;
    float norm_y_start = 0.0f;
    float norm_y_end = 1.0f;

    int camera_type;

    // Window dimensions and coordinates for presenting in the window
    float window_x, window_y;
    float window_w, window_h;

    bool is_clicked;

  };

  extern int MAX_QUADS;
  extern std::vector<QuadData> AllQuads;
  extern float VERTEX_OFFSET;
  extern int COUNT_QUADS;
  extern int REQ_SIZE_BUFFER;

  // Takes some quad information and produces vertex data struct to be added to quad;
  VertexData _fill_quad_vertex_data(quads::QuadData& q, int n);

  // Initialize quads
  void init();

  // Clear all quads data
  void clear();

  // Accumulate all quad vectors from different components
  void update();

  // Makes quad out of entity data (requires Position, Dimension, Color or Model, Render components)
  void make_entity_quad(int entity_id);

  // Render all renderable entities
  void render();

  // Writes down the quads data to ./logs/all_quads.json on every frame
  void log();

  // Prints quads data in the console, used by scripter
  void print_quads_data();

}

#endif