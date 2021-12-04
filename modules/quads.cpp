
#include <cmath> 
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "buttons.h"
#include "debug.h"
#include "entity.h"
#include "fonts.h"
#include "maps.h"
#include "menu.h"
#include "quads.h"
#include "textures.h"
#include "../dictionary.h"


namespace quads
{
  float VERTEX_OFFSET = 1;
  int COUNT_QUADS = 0;
  int REQ_SIZE_BUFFER = 0;

  std::vector<quads::QuadData> AllQuads;
  std::vector<int> QuadIndex = {};
  std::vector<int> VertexIndex = {};

  VertexData _fill_quad_vertex_data(quads::QuadData& q)
  {
    VertexData v;
    v.v1_id = gen_vertex_id();
    v.v2_id = gen_vertex_id();
    v.v3_id = gen_vertex_id();
    v.v4_id = gen_vertex_id();

    // A
    v.v1_x = q.x;
    v.v1_y = q.y;
    v.v1_tx_x = q.norm_x_start;
    v.v1_tx_y = 0.0f;

    // B
    v.v2_x = q.x + q.w - quads::VERTEX_OFFSET;
    v.v2_y = q.y;
    v.v2_tx_x = q.norm_x_end;
    v.v2_tx_y = 0.0f;

    // C
    v.v3_x = q.x;
    v.v3_y = q.y + q.h - quads::VERTEX_OFFSET;
    v.v3_tx_x = q.norm_x_start;
    v.v3_tx_y = 1.0f;

    // D
    v.v4_x = q.x + q.w - quads::VERTEX_OFFSET;
    v.v4_y = q.y + q.h - quads::VERTEX_OFFSET;
    v.v4_tx_x = q.norm_x_end;
    v.v4_tx_y = 1.0f;

    q.v = v;
    q.i_left.v1 =q.v.v1_id;
    q.i_left.v2 = q.v.v2_id;
    q.i_left.v3 = q.v.v3_id;
    q.i_right.v1 = q.v.v2_id;
    q.i_right.v2 = q.v.v3_id;
    q.i_right.v3 = q.v.v4_id;

    return v;
  }

  int _find_next_quad_id()
  {
    int n = quads::QuadIndex.size();
    // for whole vector, find value that would be bigger than (index + 1)
    for (int i = 0; i < n; i++)
    {
      if (quads::QuadIndex[i] > (i+1)){
        return i+1;
      }
    }
    return n+1;
  }

  int gen_quad_id()
  {
    int next_quad_id = quads::_find_next_quad_id();
    quads::QuadIndex.push_back(next_quad_id);
    return next_quad_id;
  }

  int _find_next_vertex_id()
  {
    int n = quads::VertexIndex.size();
    // for whole vector, find value that would be bigger than (index + 1)
    for (int i = 0; i < n; i++)
    {
      if (quads::VertexIndex[i] > i){
        return i;
      }
    }
    return n;
  }

  int gen_vertex_id()
  {
    int next_vertex_id = quads::_find_next_vertex_id();
    quads::VertexIndex.push_back(next_vertex_id);
    return next_vertex_id;
  }

  void clear()
  {
    quads::AllQuads.clear();
    quads::VertexIndex.clear();
    quads::QuadIndex.clear();
  }

  void accumulate()
  {
    quads::AllQuads.clear();
    quads::VertexIndex.clear();
    quads::QuadIndex.clear();

    // assign map quads
    if(maps::MapQuads.size() > 0)
    {
      quads::AllQuads.insert(quads::AllQuads.end(), maps::MapQuads.begin(), maps::MapQuads.end());
    }

    // assign entity quads
    if(entity::EntityQuads.size() > 0)
    {
      quads::AllQuads.insert(quads::AllQuads.end(), entity::EntityQuads.begin(), entity::EntityQuads.end());
    }

    if(debug::DebugQuads.size() > 0)
    {
      quads::AllQuads.insert(quads::AllQuads.end(), debug::DebugQuads.begin(), debug::DebugQuads.end());
    }

    // assign menu quads
    if(menu::MenuQuads.size() > 0)
    {
      quads::AllQuads.insert(quads::AllQuads.end(), menu::MenuQuads.begin(), menu::MenuQuads.end());
    }

    if(buttons::ButtonQuads.size() > 0)
    {
      quads::AllQuads.insert(quads::AllQuads.end(), buttons::ButtonQuads.begin(), buttons::ButtonQuads.end());
    }

    if(fonts::TextQuads.size() > 0)
    {
      quads::AllQuads.insert(quads::AllQuads.end(), fonts::TextQuads.begin(), fonts::TextQuads.end());
    }

    // Assigning vertex index and vertex positions here, on the final table
    for(int q=0; q < quads::AllQuads.size(); q++ )
    { 
      quads::_fill_quad_vertex_data(quads::AllQuads[q]);
    }

    quads::COUNT_QUADS = quads::AllQuads.size();
    quads::REQ_SIZE_BUFFER = COUNT_QUADS*6*sizeof(float);
    // std::cout << "count entity quads: " << entity::EntityQuads.size() << std::endl;

  }


  template <typename T>
  std::vector<quads::QuadData> make_quads(std::map<int, T> data, int object_type_id)
  {
    std::vector<quads::QuadData> quads = {};
    for (auto const& [k, v] : data)
    {
      quads::QuadData quad;
      quad.id = quads::gen_quad_id();
      quad.texture_id = v.texture_id;
      quad.frame_id = v.frame_id;

      quad.object_id = v.id;
      quad.object_type_id = object_type_id;
      quad.camera_type = v.camera_type;

      quad.r = v.r;
      quad.g = v.g;
      quad.b = v.b;
      quad.a = v.a;

      quad.x = v.x;
      quad.y = v.y;
      quad.z = 1.0f;
      quad.h = v.h;
      quad.w = v.w;

      quad.window_x = v.x;
      quad.window_y = v.y;
      quad.window_h = v.h;
      quad.window_w = v.w;

      quad.norm_x_start = v.norm_x_start;
      quad.norm_x_end = v.norm_x_end;

      quad.is_clicked = v.is_clicked;
      quads.push_back(quad);
    }
    return quads;
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
                      "    \"id\": " << quads::AllQuads[i].id                         << ",\n"
                      "    \"object_id\": " << quads::AllQuads[i].object_id           << ",\n"
                      "    \"texture_id\": " << quads::AllQuads[i].texture_id         << ",\n"
                      "    \"frame_id\": " << quads::AllQuads[i].frame_id             << ",\n"
                      "    \"r\": " << quads::AllQuads[i].r                           << ",\n"
                      "    \"g\": " << quads::AllQuads[i].g                           << ",\n"
                      "    \"b\": " << quads::AllQuads[i].b                           << ",\n"
                      "    \"a\": " << quads::AllQuads[i].a                           << ",\n"
                      "    \"x\": " << quads::AllQuads[i].x                           << ",\n"
                      "    \"y\": " << quads::AllQuads[i].y                           << ",\n"
                      "    \"w\": " << quads::AllQuads[i].w                           << ",\n"
                      "    \"h\": " << quads::AllQuads[i].h                           << ",\n"
                      "    \"object_type_id\": " << quads::AllQuads[i].object_type_id << ",\n"
                      "    \"camera_type\": " << quads::AllQuads[i].camera_type       << ",\n"
                      "    \"window_x\": " << quads::AllQuads[i].window_x             << ",\n"
                      "    \"window_y\": " << quads::AllQuads[i].window_y             << ",\n"
                      "    \"window_w\": " << quads::AllQuads[i].window_w             << ",\n"
                      "    \"window_h\": " << quads::AllQuads[i].window_h             << ",\n"
                      "    \"norm_x_start\": " << quads::AllQuads[i].norm_x_start     << ",\n"
                      "    \"norm_x_end\": " << quads::AllQuads[i].norm_x_end         << ",\n"
                      "    \"is_clicked\": " << quads::AllQuads[i].is_clicked         << "\n"
                      << end_str;
      }
      quads_file << "] \n";
      quads_file.close();
    }
  }

  template std::vector<quads::QuadData> quads::make_quads<buttons::ButtonData>(std::map<int, buttons::ButtonData>, int);
  template std::vector<quads::QuadData> quads::make_quads<menu::MenuData>(std::map<int, menu::MenuData>, int);
  template std::vector<quads::QuadData> quads::make_quads<debug::PointData>(std::map<int, debug::PointData>, int);
  template std::vector<quads::QuadData> quads::make_quads<entity::EntityData>(std::map<int, entity::EntityData>, int);
  template std::vector<quads::QuadData> quads::make_quads<fonts::TextData>(std::map<int, fonts::TextData>, int);
  template std::vector<quads::QuadData> quads::make_quads<maps::TileData>(std::map<int, maps::TileData>, int);

}