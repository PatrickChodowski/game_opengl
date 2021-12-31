
#include <cmath> 
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


#include "buttons.h"
#include "debug.h"
#include "entity.h"
#include "fonts.h"
#include "logger.h"
#include "maps.h"
#include "menu.h"
#include "quads.h"
#include "textures.h"

#include "../dependencies/parallel_hashmap/phmap.h"
#include "../dictionary.h"

namespace quads
{
  float VERTEX_OFFSET = 1;
  int COUNT_QUADS = 0;
  int REQ_SIZE_BUFFER = 0;
  int MAX_QUADS = 35000;
  // 4000*312* // this can even go on stack if needed

// MAX 20 menu quads
// MAX 100 button quads
// MAX 80 debug points
// MAX 720 map tiles
// MAX 1000 letters
// MAX 2000 entities

  // Quad size -> 312bytes


  // vector of quads
  std::vector<quads::QuadData> AllQuads(MAX_QUADS);

  template <typename T>
  void add_quads(phmap::flat_hash_map<int, T>& data, int object_type_id)
  {
    for (auto const& [k, v] : data)
    {
      int quad_id = quads::make_quad(v, object_type_id);
    }
  }

  template <typename T>
  int make_quad(T& data, int object_type_id)
  {
    quads::QuadData quad;
    quad.id = (object_type_id+1)*100000 + data.id;
    quad.texture_id = data.texture_id;
    quad.frame_id = data.frame_id;
    quad.object_id = data.id;
    quad.object_type_id = object_type_id;
    quad.camera_type = data.camera_type;
    
    quad.color = data.color;
    quad.pos = data.pos;
    quad.dims =  data.dims;
    quad.pos.z = 1.0f;

    quad.window_x = data.pos.x;
    quad.window_y = data.pos.y;
    quad.window_h = data.dims.h;
    quad.window_w = data.dims.w;
    quad.norm = data.norm;
    quad.is_clicked = data.is_clicked;
    quad.is_deleted = false;
    quads::AllQuads.push_back(quad);
    return quad.id;
  }

  VertexData _fill_quad_vertex_data(quads::QuadData& q, int n)
  {
    VertexData v;
    v.v1_id = (n*4);
    v.v2_id = (n*4) + 1;
    v.v3_id = (n*4) + 2;
    v.v4_id = (n*4) + 3;

    // A
    v.v1_x = q.pos.x;
    v.v1_y = q.pos.y;
    v.v1_z = q.pos.z;
    v.v1_tx_x = q.norm.x_start;
    v.v1_tx_y = q.norm.y_start;
    //v.v1_tx_y = 0.0f;

    // B
    v.v2_x = q.pos.x + q.dims.w - quads::VERTEX_OFFSET;
    v.v2_y = q.pos.y;
    v.v2_z = q.pos.z;
    v.v2_tx_x = q.norm.x_end;
    v.v2_tx_y = q.norm.y_start;
    //v.v2_tx_y = 0.0f;

    // C
    v.v3_x = q.pos.x;
    v.v3_y = q.pos.y + q.dims.h - quads::VERTEX_OFFSET;
    v.v3_z = q.pos.z;
    v.v3_tx_x = q.norm.x_start;
    v.v3_tx_y = q.norm.y_end;
    //v.v3_tx_y = 1.0f;

    // D
    v.v4_x = q.pos.x + q.dims.w - quads::VERTEX_OFFSET;
    v.v4_y = q.pos.y + q.dims.h - quads::VERTEX_OFFSET;
    v.v4_z = q.pos.z;
    v.v4_tx_x = q.norm.x_end;
    v.v4_tx_y = q.norm.y_end;
    //v.v4_tx_y = 1.0f;

    q.v = v;
    q.i_left.v1 =q.v.v1_id;
    q.i_left.v2 = q.v.v2_id;
    q.i_left.v3 = q.v.v3_id;
    q.i_right.v1 = q.v.v2_id;
    q.i_right.v2 = q.v.v3_id;
    q.i_right.v3 = q.v.v4_id;

    return v;
  }

  void clear()
  {
    quads::AllQuads.clear();
  }

  void update()
  {
    // Assigning vertex index and vertex positions here, on the final table
    for(int q=0; q < quads::AllQuads.size(); q++ )
    { 
      quads::_fill_quad_vertex_data(quads::AllQuads[q], q);
    }
    quads::COUNT_QUADS = quads::AllQuads.size();
    quads::REQ_SIZE_BUFFER = COUNT_QUADS*6*sizeof(float);
  }


  void log()
  {
    const char* log_path = "logs/quads.json";
    std::ofstream quads_file (log_path);
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
                      "    \"quad_id\": " << quads::AllQuads[i].id                    << ",\n"
                      "    \"left_v1\": " << quads::AllQuads[i].i_left.v1             << ",\n"
                      "    \"left_v2\": " << quads::AllQuads[i].i_left.v2             << ",\n"
                      "    \"left_v3\": " << quads::AllQuads[i].i_left.v3             << ",\n"
                      "    \"right_v1\": " << quads::AllQuads[i].i_right.v1           << ",\n"
                      "    \"right_v2\": " << quads::AllQuads[i].i_right.v2           << ",\n"
                      "    \"right_v3\": " << quads::AllQuads[i].i_right.v3           << ",\n"
                      "    \"object_id\": " << quads::AllQuads[i].object_id           << ",\n"
                      "    \"texture_id\": " << quads::AllQuads[i].texture_id         << ",\n"
                      "    \"frame_id\": " << quads::AllQuads[i].frame_id             << ",\n"
                      "    \"r\": " << quads::AllQuads[i].color.r                     << ",\n"
                      "    \"g\": " << quads::AllQuads[i].color.g                     << ",\n"
                      "    \"b\": " << quads::AllQuads[i].color.b                     << ",\n"
                      "    \"a\": " << quads::AllQuads[i].color.a                     << ",\n"
                      "    \"x\": " << quads::AllQuads[i].pos.x                       << ",\n"
                      "    \"y\": " << quads::AllQuads[i].pos.y                       << ",\n"
                      "    \"w\": " << quads::AllQuads[i].dims.w                      << ",\n"
                      "    \"h\": " << quads::AllQuads[i].dims.h                      << ",\n"
                      "    \"object_type_id\": " << quads::AllQuads[i].object_type_id << ",\n"
                      "    \"camera_type\": " << quads::AllQuads[i].camera_type       << ",\n"
                      "    \"window_x\": " << quads::AllQuads[i].window_x             << ",\n"
                      "    \"window_y\": " << quads::AllQuads[i].window_y             << ",\n"
                      "    \"window_w\": " << quads::AllQuads[i].window_w             << ",\n"
                      "    \"window_h\": " << quads::AllQuads[i].window_h             << ",\n"
                      "    \"norm_x_start\": " << quads::AllQuads[i].norm.x_start     << ",\n"
                      "    \"norm_x_end\": " << quads::AllQuads[i].norm.x_end         << ",\n"
                      "    \"norm_y_start\": " << quads::AllQuads[i].norm.y_start     << ",\n"
                      "    \"norm_y_end\": " << quads::AllQuads[i].norm.y_end         << ",\n"
                      "    \"is_clicked\": " << quads::AllQuads[i].is_clicked         << "\n"
                      << end_str;
      }
      quads_file << "] \n";
      quads_file.close();
    }
  }
 // #include "../dependencies/parallel_hashmap/phmap.h"
  template void quads::add_quads<buttons::ButtonData>(phmap::flat_hash_map<int, buttons::ButtonData>&, int);
  template void quads::add_quads<menu::MenuData>(phmap::flat_hash_map<int, menu::MenuData>&, int);
  template void quads::add_quads<debug::PointData>(phmap::flat_hash_map<int, debug::PointData>&, int);
  template void quads::add_quads<entity::EntityData>(phmap::flat_hash_map<int, entity::EntityData>&, int);
  template void quads::add_quads<fonts::TextData>(phmap::flat_hash_map<int, fonts::TextData>&, int);
  template void quads::add_quads<maps::TileData>(phmap::flat_hash_map<int, maps::TileData>&, int);

  template int quads::make_quad<buttons::ButtonData>(buttons::ButtonData&, int);
  template int quads::make_quad<menu::MenuData>(menu::MenuData&, int);
  template int quads::make_quad<debug::PointData>(debug::PointData&, int);
  template int quads::make_quad<entity::EntityData>(entity::EntityData&, int);
  template int quads::make_quad<fonts::TextData>(fonts::TextData&, int);
  template int quads::make_quad<maps::TileData>(maps::TileData&, int);

}