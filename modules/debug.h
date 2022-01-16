
#include <vector>

#include "quads.h"
#include "../dependencies/parallel_hashmap/phmap.h"

#ifndef MODULES_DEBUG_H
#define MODULES_DEBUG_H

namespace debug
{ 
  struct LineData
  {
    float x1, y1;
    float x2, y2;
    float r, g, b, a;
  };
  
  struct PointData
  {
    quads::Position pos;
    quads::Color color;
    quads::Dims dims;
    quads::Norm norm;

    float camera_type;
    
    // not used in this case:
    int texture_id = -1;
    int frame_id = -1;
    int id;
    
    bool is_clicked;
    bool is_reversed = false;
  };
  
  extern std::vector<debug::LineData> lines;
  extern phmap::flat_hash_map<int, debug::PointData> points;
  extern std::vector<int> Index;

  // Renders straight line between start and endpoint
  void render_line(float x1, float y1, float x2, float y2, float r, float g, float b, float a);

  // Render point (small quad in specific place)
  void render_point(float x, float y, float r, float g, float b, float a);

  // Well, it renders 4 lines
  void render_square(float x, float y, float w, float h, float r, float g, float b, float a);

  // Renders debugquads from points
  void render();

  // Remove debug data after drawing, called from buffer
  void clear();

  // Log lines information to file
  void log();

}


#endif
