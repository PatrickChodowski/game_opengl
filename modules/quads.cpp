
#include <cmath> 
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "debug.h"
#include "entity.h"
#include "fonts.h"
#include "gui.h"
#include "maps.h"
#include "menu.h"
#include "quads.h"
#include "textures.h"
#include "../dictionary.h"


namespace quads2
{
  float VERTEX_OFFSET = 1;
  int COUNT_QUADS = 0;
  int REQ_SIZE_BUFFER = 0;

  std::vector<quads2::QuadData> AllQuads;
  std::vector<int> UsedQuadIds = {};
  std::vector<int> UsedVertexIds = {};

  VertexData _fill_quad_vertex_data(quads2::QuadData& q)
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
    v.v2_x = q.x + q.w - quads2::VERTEX_OFFSET;
    v.v2_y = q.y;
    v.v2_tx_x = q.norm_x_end;
    v.v2_tx_y = 0.0f;

    // C
    v.v3_x = q.x;
    v.v3_y = q.y + q.h - quads2::VERTEX_OFFSET;
    v.v3_tx_x = q.norm_x_start;
    v.v3_tx_y = 1.0f;

    // D
    v.v4_x = q.x + q.w - quads2::VERTEX_OFFSET;
    v.v4_y = q.y + q.h - quads2::VERTEX_OFFSET;
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
    int n = quads2::UsedQuadIds.size();
    // for whole vector, find value that would be bigger than (index + 1)
    for (int i = 0; i < n; i++)
    {
      if (quads2::UsedQuadIds[i] > (i+1)){
        return i+1;
      }
    }
    return n+1;
  }

  int gen_quad_id()
  {
    int next_quad_id = quads2::_find_next_quad_id();
    quads2::UsedQuadIds.push_back(next_quad_id);
    return next_quad_id;
  }

  int _find_next_vertex_id()
  {
    int n = quads2::UsedVertexIds.size();
    // for whole vector, find value that would be bigger than (index + 1)
    for (int i = 0; i < n; i++)
    {
      if (quads2::UsedVertexIds[i] > i){
        return i;
      }
    }
    return n;
  }

  int gen_vertex_id()
  {
    int next_vertex_id = quads2::_find_next_vertex_id();
    quads2::UsedVertexIds.push_back(next_vertex_id);
    return next_vertex_id;
  }

  void clear()
  {
    quads2::AllQuads.clear();
    quads2::UsedVertexIds.clear();
    quads2::UsedQuadIds.clear();
  }

  void accumulate()
  {
    quads2::AllQuads.clear();
    quads2::UsedVertexIds.clear();
    quads2::UsedQuadIds.clear();

    // // assign menu quads
    if(menu2::MenuQuads.size() > 0)
    {
      quads2::AllQuads.insert(quads2::AllQuads.end(), menu2::MenuQuads.begin(), menu2::MenuQuads.end());
    }

    // assign map quads
    if(maps2::MapQuads.size() > 0)
    {
      quads2::AllQuads.insert(quads2::AllQuads.end(), maps2::MapQuads.begin(), maps2::MapQuads.end());
    }

    // assign entity quads
    if(entity::EntityQuads.size() > 0)
    {
      quads2::AllQuads.insert(quads2::AllQuads.end(), entity::EntityQuads.begin(), entity::EntityQuads.end());
    }

    if(debug2::DebugQuads.size() > 0)
    {
      quads2::AllQuads.insert(quads2::AllQuads.end(), debug2::DebugQuads.begin(), debug2::DebugQuads.end());
    }

    if(gui2::GuiQuads.size() > 0)
    {
      quads2::AllQuads.insert(quads2::AllQuads.end(), gui2::GuiQuads.begin(), gui2::GuiQuads.end());
    }

    if(fonts2::TextQuads.size() > 0)
    {
      quads2::AllQuads.insert(quads2::AllQuads.end(), fonts2::TextQuads.begin(), fonts2::TextQuads.end());
    }

    // Assigning vertex index and vertex positions here, on the final table
    for(int q=0; q < quads2::AllQuads.size(); q++ )
    { 
      quads2::_fill_quad_vertex_data(quads2::AllQuads[q]);
    }

    quads2::COUNT_QUADS = quads2::AllQuads.size();
    quads2::REQ_SIZE_BUFFER = COUNT_QUADS*6*sizeof(float);
    // std::cout << "count entity quads: " << entity::EntityQuads.size() << std::endl;

  }


  template <typename T>
  std::vector<quads2::QuadData> make_quads(std::map<int, T> data, int object_type_id)
  {
    std::vector<quads2::QuadData> quads = {};
    for (auto const& [k, v] : data)
    {
      quads2::QuadData quad;
      quad.id = quads2::gen_quad_id();
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
      for(int i = 0; i < quads2::AllQuads.size(); i++)
      {
        if(i == (quads2::AllQuads.size() - 1))
        {
          end_str = " } \n";
        }

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


  template std::vector<quads2::QuadData> quads2::make_quads<debug2::PointData>(std::map<int, debug2::PointData>, int);
  template std::vector<quads2::QuadData> quads2::make_quads<entity::EntityData>(std::map<int, entity::EntityData>, int);
  template std::vector<quads2::QuadData> quads2::make_quads<fonts2::TextData>(std::map<int, fonts2::TextData>, int);
  template std::vector<quads2::QuadData> quads2::make_quads<gui2::GuiData>(std::map<int, gui2::GuiData>, int);
  template std::vector<quads2::QuadData> quads2::make_quads<maps2::TileData>(std::map<int, maps2::TileData>, int);
  template std::vector<quads2::QuadData> quads2::make_quads<menu2::ButtonData>(std::map<int, menu2::ButtonData>, int);
  

}