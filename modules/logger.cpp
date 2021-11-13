
#include "logger.h"
#include "quads.h"

#include <string>
#include <fstream>


namespace log2
{

  void all_quads_to_json()
  {
    std::string file_path = "./logs/all_quads.json";
    std::ofstream quads_file (file_path.c_str());
    std::string end_str = " }, \n";
    if (quads_file.is_open())
    {
      quads_file << "[ \n";
      for(int i = 0; i < quads::AllQuads.size(); i++)
      {
        if(i == (quads::AllQuads.size() - 1))
        {
          end_str = " } \n";
        }
        quads_file << " { \n" <<
                      "    \"id\": " << quads::AllQuads[i].id                         << ",\n"
                      "    \"x\": " << quads::AllQuads[i].x                           << ",\n"
                      "    \"y\": " << quads::AllQuads[i].y                           << ",\n"
                      "    \"w\": " << quads::AllQuads[i].w                           << ",\n"
                      "    \"h\": " << quads::AllQuads[i].h                           << ",\n"
                      "    \"solid\": " << quads::AllQuads[i].solid                   << ",\n"
                      "    \"coll\": " << quads::AllQuads[i].coll                     << ",\n"
                      "    \"is_clicked\": " << quads::AllQuads[i].is_clicked         << ",\n"
                      "    \"a\": " << quads::AllQuads[i].a                           << ",\n"
                      "    \"b\": " << quads::AllQuads[i].b                           << ",\n"
                      "    \"c\": " << quads::AllQuads[i].c                           << ",\n"
                      "    \"d\": " << quads::AllQuads[i].d                           << ",\n"
                      "    \"texture_id\": " << quads::AllQuads[i].texture_id         << ",\n"
                      "    \"frame_id\": " << quads::AllQuads[i].frame_id             << ",\n"
                      "    \"r_col\": " << quads::AllQuads[i].r_col                   << ",\n"
                      "    \"g_col\": " << quads::AllQuads[i].g_col                   << ",\n"
                      "    \"b_col\": " << quads::AllQuads[i].b_col                   << ",\n"
                      "    \"a_col\": " << quads::AllQuads[i].a_col                   << ",\n"
                      "    \"type_id\": " << quads::AllQuads[i].type_id               << ",\n"
                      "    \"is_static\": " << quads::AllQuads[i].is_static           << ",\n"
                      "    \"entity_type_id\": " << quads::AllQuads[i].entity_type_id << ",\n"
                      "    \"alive\": " << quads::AllQuads[i].alive                   << ",\n"
                      "    \"s_x\": " << quads::AllQuads[i].s_x                       << ",\n"
                      "    \"s_y\": " << quads::AllQuads[i].s_y                       << ",\n"
                      "    \"s_w\": " << quads::AllQuads[i].s_w                       << ",\n"
                      "    \"s_h\": " << quads::AllQuads[i].s_h                       << ",\n"
                      "    \"s_diag\": " << quads::AllQuads[i].s_diag                 << "\n" 
                      << end_str;
      }
      quads_file << "] \n";
      quads_file.close();
    }
  }

}