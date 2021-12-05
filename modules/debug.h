
#include <map>
#include <vector>

#include "quads.h"

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
    float x, y, z;
    float r, g, b, a;
    float w, h;

    float camera_type;
    float norm_x_start = 0.0f;
    float norm_x_end = 1.0f;
    float norm_y_start = 0.0f;
    float norm_y_end = 1.0f;

    // not used in this case:
    int texture_id = -1;
    int frame_id = -1;
    int id;
    
    bool is_clicked;
  };
  
  extern std::vector<debug::LineData> lines;
  extern std::map<int, debug::PointData> points;
  extern std::vector<quads::QuadData> DebugQuads;
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

}


#endif
