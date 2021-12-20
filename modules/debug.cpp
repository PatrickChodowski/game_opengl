
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
  phmap::flat_hash_map<int, debug::PointData> points = {};
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
    pdd.pos.x = x;
    pdd.pos.y = y;
    pdd.color.r = r;
    pdd.color.g = g;
    pdd.color.b = b;
    pdd.color.a = a;
    pdd.dims.w = 10;
    pdd.dims.h = 10;
    pdd.camera_type = CAMERA_DYNAMIC;
    points[pdd.id] = pdd;
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


  void log()
  {
    const char* log_path = "logs/lines.json";
    std::ofstream lines_file (log_path);
    std::string end_str = " }, \n";
    if (lines_file.is_open())
    {
      lines_file << "[ \n";
      for(int i = 0; i < debug::lines.size(); i++)
      {
        if(i == (debug::lines.size() - 1))
        {
          end_str = " } \n";
        }

        lines_file << " { \n" <<
                      "    \"r\": " << debug::lines[i].r                     << ",\n"
                      "    \"g\": " << debug::lines[i].g                     << ",\n"
                      "    \"b\": " << debug::lines[i].b                     << ",\n"
                      "    \"a\": " << debug::lines[i].a                     << ",\n"
                      "    \"x1\": " << debug::lines[i].x1                       << ",\n"
                      "    \"y1\": " << debug::lines[i].y1                       << ",\n"
                      "    \"x2\": " << debug::lines[i].x2                       << ",\n"
                      "    \"y2\": " << debug::lines[i].y2                       << ",\n"
                      << end_str;
      }
      lines_file << "] \n";
      lines_file.close();
    }
  }

}


