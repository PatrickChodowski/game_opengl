
#include "debug.h"
#include "quads.h"
#include "utils.h"
#include "../dictionary.h"
#include "../dependencies/parallel_hashmap/phmap.h"

#include <fstream>
#include <iostream>
#include <vector>

namespace debug
{
  std::vector<debug::LineData> lines = {};

  void render_line(float x1, float y1, float x2, float y2, float r, float g, float b, float a)
  {
    debug::LineData ldd;
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
    quads::QuadData quad;
    quad.entity_id = -1;
    quad.model_id = -1;
    quad.frame_id = -1;
    quad.entity_type_id = ENTITY_TYPE_COLOR;
    quad.x = x;
    quad.y = y;
    quad.z = 0.99f;
    quad.r = r;
    quad.g = g;
    quad.b = b;
    quad.a = a;
    quad.w = 10;
    quad.h = 10;
    quad.camera_type = CAMERA_DYNAMIC;
    quad.is_clicked = false;
    quads::AllQuads.push_back(quad);
  };

  void render_square(float x, float y, float w, float h, float r, float g, float b, float a)
  { 
    // top
    debug::render_line(x, y, x+w, y, r, g, b, a);

    // right
    debug::render_line(x+w, y, x+w, y+h, r, g, b, a);

    // bottom
    debug::render_line(x, y+h, x+w, y+h, r, g, b, a);

    // left
    debug::render_line(x, y, x, y+h, r, g, b, a);

  };

  void clear()
  {
    lines.clear();
  };


}


