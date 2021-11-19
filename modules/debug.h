
#include <vector>

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
  
  extern std::vector<debug2::LineData> lines;

  // Renders straight line between start and endpoint
  void render_line(float x1, float y1, float x2, float y2, float r, float g, float b, float a);

  // Render point (small quad in specific place)
  void render_point(float x, float y, float r, float g, float b, float a);

  void render_outline();

  void render_popup();

  // Remove debug data after drawing, called from buffer
  void clear();

}


#endif
