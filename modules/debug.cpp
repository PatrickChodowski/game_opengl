
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
  std::vector<quads::QuadData> DebugQuads = {};
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
    pdd.norm_x_start = 0;
    pdd.norm_x_end = 0;
    pdd.camera_type = CAMERA_DYNAMIC;
    points[pdd.id] = pdd;
  };

  void render()
  {
    debug::DebugQuads.clear();
    debug::DebugQuads = quads::make_quads(debug::points, OBJECT_TYPE_DEBUG);
  }

  void clear()
  {
    debug::Index.clear();
    debug::DebugQuads.clear();
    lines.clear();
    points.clear();
  };

}


