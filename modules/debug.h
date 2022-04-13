
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
    
  extern std::vector<debug::LineData> lines;
 
  // Renders straight line between start and endpoint
  void render_line(float x1, float y1, float x2, float y2, float r, float g, float b, float a);

  // Render point (small quad in specific place)
  void render_point(float x, float y, float r, float g, float b, float a);

  // Well, it renders 4 lines
  void render_square(float x, float y, float w, float h, float r, float g, float b, float a);

  // Remove debug data after drawing, called from buffer
  void clear();

}


#endif
