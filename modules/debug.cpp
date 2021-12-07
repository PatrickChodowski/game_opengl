
#include "debug.h"
#include "quads.h"
#include "utils.h"
#include "../dictionary.h"

#include <map>
#include <vector>

namespace debug
{

  std::vector<debug::LineData> lines = {};
  std::map<int, debug::PointData> points = {};
  std::vector<int> Index = {};

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
    debug::PointData pdd;
    pdd.id = utils::generate_id(debug::Index);
    pdd.x = x;
    pdd.y = y;
    pdd.r = r;
    pdd.g = g;
    pdd.b = b;
    pdd.a = a;
    pdd.w = 10;
    pdd.h = 10;
    pdd.camera_type = CAMERA_DYNAMIC;
    points[pdd.id] = pdd;
  };



  void render_square(float x, float y, float w, float h, float r, float g, float b, float a)
  { 
    // top
    debug::LineData ldd;
    ldd.x1 = x; //left top
    ldd.y1 = y;
    ldd.x2 = x+w; // right top
    ldd.y2 = y;
    ldd.r = r;
    ldd.g = g;
    ldd.b = b;
    ldd.a = a;
    lines.push_back(ldd);

    // right
    debug::LineData ldd1;
    ldd1.x1 = x+w;
    ldd1.y1 = y;
    ldd1.x2 = x+w;
    ldd1.y2 = y+h;
    ldd1.r = r;
    ldd1.g = g;
    ldd1.b = b;
    ldd1.a = a;
    lines.push_back(ldd1);

    // bottom
    debug::LineData ldd2;
    ldd2.x1 = x;
    ldd2.y1 = y+h;
    ldd2.x2 = x+w;
    ldd2.y2 = y+h;
    ldd2.r = r;
    ldd2.g = g;
    ldd2.b = b;
    ldd2.a = a;
    lines.push_back(ldd2);

    // left
    debug::LineData ldd3;
    ldd3.x1 = x;
    ldd3.y1 = y;
    ldd3.x2 = x;
    ldd3.y2 = y+h;
    ldd3.r = r;
    ldd3.g = g;
    ldd3.b = b;
    ldd3.a = a;
    lines.push_back(ldd3);
  };




  void render()
  {
    quads::add_quads(debug::points, OBJECT_TYPE_DEBUG);
  }

  void clear()
  {
    debug::Index.clear();
    lines.clear();
    points.clear();
  };

}


