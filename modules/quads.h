
#include <array>
#include <vector>
#include <map>

#include "collisions.h"


#ifndef MODULES_QUAD_H
#define MODULES_QUAD_H

// Frame data. Table of quads and all operations on this table
namespace quads
{
  // Color of the quad -> r,g,b,a
  struct Color
  {
    float r, g, b, a;
  };

  // Position -> x,y,z
  struct Position
  {
    float x, y;
    float z = 1.0f;
  };

  // Dimensions -> w,h
  struct Dims
  {
    float w, h;
  };

  // Normalized position of frame in texture -> x_start, x_end, y_start, y_end
  struct Norm
  {
    float x_start = 0.0f;
    float x_end = 1.0f;
    float y_start = 0.0f;
    float y_end = 1.0f;
  };


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

    quads::Color color;
    quads::Position pos;
    quads::Dims dims;
    quads::Norm norm;

    float object_type_id;
    float camera_type;

    // Window dimensions and coordinates for presenting in the window
    float window_x, window_y;
    float window_w, window_h;

    bool is_clicked;
    bool is_deleted;

  };


  extern int MAX_QUADS;
  extern std::vector<QuadData> AllQuads;
  extern float VERTEX_OFFSET;
  extern int COUNT_QUADS;
  extern int REQ_SIZE_BUFFER;

  // Takes some quad information and produces vertex data struct to be added to quad;
  VertexData _fill_quad_vertex_data(quads::QuadData& q, int n);

  // Clear all quads data
  void clear();

  // Accumulate all quad vectors from different components
  void update();

  // Makes quads out of the object catalog data - entities, text, menu, debug, gui, maps etc. Adds straight to quads array
  template <typename T>
  void add_quads(std::map<int, T>& data, int object_type_id);

  // Makes one quad out of single struct data. Returns quad_id
  template <typename T>
  int make_quad(T& data, int object_type_id);

  // Writes down the quads data to ./logs/all_quads.json on every frame
  void log();

}

#endif