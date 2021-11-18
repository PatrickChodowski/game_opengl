
#include "debug.h"

namespace debug2
{

  std::vector<debug2::LineData> lines = {};

  void render_line(float x1, float y1, float x2, float y2, float r, float g, float b, float a)
  {
    debug2::LineData ldd;
    ldd.x1 = x1;
    ldd.y1 = y1;
    ldd.x2 = x2;
    ldd.y2 = y2;
    ldd.r = r;
    ldd.g = g;
    ldd.b = b;
    ldd.a = a;
    lines.push_back(ldd);
  };

  void render_point(float x, float y, float r, float g, float b, float a)
  {

  };

  void render_outline()
  {

  };

  void render_popup()
  {

  };

  void clear()
  {
    lines.clear();
  };

}


