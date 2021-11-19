
#include <map>
#include <vector>

#include "quads.h"

#ifndef MODULES_DEBUG_H
#define MODULES_DEBUG_H

namespace debug2
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

    // not used in this case:
    float norm_x_start, norm_x_end;
    int texture_id;
    int frame_id;
    int id;
    
    bool is_clicked;
  };
  
  extern std::vector<debug2::LineData> lines;
  extern std::map<int, debug2::PointData> points;
  extern std::vector<quads2::QuadData> DebugQuads;
  extern std::vector<int> UsedDebugIds;

  // Renders straight line between start and endpoint
  void render_line(float x1, float y1, float x2, float y2, float r, float g, float b, float a);

  // Render point (small quad in specific place)
  void render_point(float x, float y, float r, float g, float b, float a);

  // Renders debugquads from points
  void render();

  // Renders popup menu with information and functions about the game object
  void render_popup();

  // Remove debug data after drawing, called from buffer
  void clear();

}


#endif
