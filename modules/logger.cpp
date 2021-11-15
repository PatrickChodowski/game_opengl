
#include "logger.h"
#include "quads.h"

#include <fstream>
#include <iostream>
#include <string>

namespace logger2
{

  void all_quads_to_json()
  {
    std::string file_path = "./logs/all_quads.json";
    std::ofstream quads_file (file_path.c_str());
    std::string end_str = " }, \n";
    if (quads_file.is_open())
    {
      quads_file << "[ \n";
      for(int i = 0; i < quads2::AllQuads.size(); i++)
      {
        if(i == (quads2::AllQuads.size() - 1))
        {
          end_str = " } \n";
        }

      // if(quads2::AllQuads[i].id   == 1)
      // {
      //   std::cout << " world: " << quads2::AllQuads[i].x << "," << quads2::AllQuads[i].y << "," << quads2::AllQuads[i].h << std::endl;
      //   std::cout << " window: " << quads2::AllQuads[i].window_x << "," << quads2::AllQuads[i].window_y << "," << quads2::AllQuads[i].window_h << std::endl;
      // }

        quads_file << " { \n" <<
                      "    \"id\": " << quads2::AllQuads[i].id                         << ",\n"
                      "    \"object_id\": " << quads2::AllQuads[i].object_id           << ",\n"
                      "    \"texture_id\": " << quads2::AllQuads[i].texture_id         << ",\n"
                      "    \"frame_id\": " << quads2::AllQuads[i].frame_id             << ",\n"
                      "    \"r\": " << quads2::AllQuads[i].r                           << ",\n"
                      "    \"g\": " << quads2::AllQuads[i].g                           << ",\n"
                      "    \"b\": " << quads2::AllQuads[i].b                           << ",\n"
                      "    \"a\": " << quads2::AllQuads[i].a                           << ",\n"
                      "    \"x\": " << quads2::AllQuads[i].x                           << ",\n"
                      "    \"y\": " << quads2::AllQuads[i].y                           << ",\n"
                      "    \"w\": " << quads2::AllQuads[i].w                           << ",\n"
                      "    \"h\": " << quads2::AllQuads[i].h                           << ",\n"
                      "    \"object_type_id\": " << quads2::AllQuads[i].object_type_id << ",\n"
                      "    \"camera_type\": " << quads2::AllQuads[i].camera_type       << ",\n"
                      "    \"window_x\": " << quads2::AllQuads[i].window_x             << ",\n"
                      "    \"window_y\": " << quads2::AllQuads[i].window_y             << ",\n"
                      "    \"window_w\": " << quads2::AllQuads[i].window_w             << ",\n"
                      "    \"window_h\": " << quads2::AllQuads[i].window_h             << ",\n"
                      "    \"norm_x_start\": " << quads2::AllQuads[i].norm_x_start     << ",\n"
                      "    \"norm_x_end\": " << quads2::AllQuads[i].norm_x_end         << ",\n"
                      "    \"is_clicked\": " << quads2::AllQuads[i].is_clicked         << "\n"
                      << end_str;
      }
      quads_file << "] \n";
      quads_file.close();
    }
  }

}