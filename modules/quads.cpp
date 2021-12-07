
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
#include "logger.h"
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
  const int MAX_QUADS = 28800;


  // Array of quads
  quads::QuadData quads[MAX_QUADS];

  std::vector<quads::QuadData> AllQuads;
  std::map<int, quads::QuadSetting> QuadsManager;


  void init()
  {
    // min index, max index, size, needs reset
    quads::QuadSetting qs_menu = {0, 19, 20, true}; // MAX 20 menu quads
    quads::QuadSetting qs_buttons = {20, 119, 100, true}; // MAX 100 button quads
    quads::QuadSetting qs_doints = {120, 199, 80, true}; // MAX 80 debug points
    quads::QuadSetting qs_text = {200, 1199, 1000, true}; // MAX 1000 letters
    quads::QuadSetting qs_ent = {1200, 3199, 2000, true}; // MAX 2000 entities
    quads::QuadSetting qs_maps = {3200, 28799, 25600, true}; // MAX 25600 map tiles (160x160)

    quads::QuadsManager[OBJECT_TYPE_MENU] = qs_menu;
    quads::QuadsManager[OBJECT_TYPE_BUTTON] = qs_buttons;
    quads::QuadsManager[OBJECT_TYPE_DEBUG] = qs_doints;
    quads::QuadsManager[OBJECT_TYPE_TEXT] = qs_text;
    quads::QuadsManager[OBJECT_TYPE_ENTITY] = qs_ent;
    quads::QuadsManager[OBJECT_TYPE_MAP] = qs_maps;

    std::cout <<  sizeof(quads::QuadData) << std::endl;
  }



  template <typename T>
  void add_quads(std::map<int, T> data, int object_type_id)
  {
    if(quads::QuadsManager[object_type_id].needs_reset)
    {
      if(data.size()>quads::QuadsManager[object_type_id].size)
      {
        logger::log(LOG_LVL_ERROR, " ERROR: Data to render exceeding its quads space", "quads", __FILE__, __LINE__, LOG_LVL_INFO);
      }

      //quads::clear_quads(quads::QuadsManager[object_type_id].min_index, quads::QuadsManager[object_type_id].max_index);
      int n = quads::QuadsManager[object_type_id].min_index;
      for (auto const& [k, v] : data)
      {
        quads::QuadData quad;
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
        quad.norm_y_start = v.norm_y_start;
        quad.norm_y_end = v.norm_y_end;

        quad.is_clicked = v.is_clicked;
        quad.is_deleted = false;
        //quads::quads[n] = quad;
        n++;
        quads::AllQuads.push_back(quad);
      }
    }
  }


  void clear_quads(int min_index, int max_index)
  {
    for(int i=min_index; i < (max_index+1); i++)
    {
      quads::quads[i].is_deleted = true;
    }
  }




  VertexData _fill_quad_vertex_data(quads::QuadData& q, int n)
  {
    VertexData v;
    v.v1_id = (n*4);
    v.v2_id = (n*4) + 1;
    v.v3_id = (n*4) + 2;
    v.v4_id = (n*4) + 3;

    // A
    v.v1_x = q.x;
    v.v1_y = q.y;
    v.v1_tx_x = q.norm_x_start;
    v.v1_tx_y = q.norm_y_start;
    //v.v1_tx_y = 0.0f;

    // B
    v.v2_x = q.x + q.w - quads::VERTEX_OFFSET;
    v.v2_y = q.y;
    v.v2_tx_x = q.norm_x_end;
    v.v2_tx_y = q.norm_y_start;
    //v.v2_tx_y = 0.0f;

    // C
    v.v3_x = q.x;
    v.v3_y = q.y + q.h - quads::VERTEX_OFFSET;
    v.v3_tx_x = q.norm_x_start;
    v.v3_tx_y = q.norm_y_end;
    //v.v3_tx_y = 1.0f;

    // D
    v.v4_x = q.x + q.w - quads::VERTEX_OFFSET;
    v.v4_y = q.y + q.h - quads::VERTEX_OFFSET;
    v.v4_tx_x = q.norm_x_end;
    v.v4_tx_y = q.norm_y_end;
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
    // quads::AllQuads.clear();

    // // assign map quads
    // if(maps::MapQuads.size() > 0)
    // {
    //   quads::AllQuads.insert(quads::AllQuads.end(), maps::MapQuads.begin(), maps::MapQuads.end());
    // }

    // // assign entity quads
    // if(entity::EntityQuads.size() > 0)
    // {
    //   quads::AllQuads.insert(quads::AllQuads.end(), entity::EntityQuads.begin(), entity::EntityQuads.end());
    // }

    // if(debug::DebugQuads.size() > 0)
    // {
    //   quads::AllQuads.insert(quads::AllQuads.end(), debug::DebugQuads.begin(), debug::DebugQuads.end());
    // }

    // // assign menu quads
    // if(menu::MenuQuads.size() > 0)
    // {
    //   quads::AllQuads.insert(quads::AllQuads.end(), menu::MenuQuads.begin(), menu::MenuQuads.end());
    // }

    // if(buttons::ButtonQuads.size() > 0)
    // {
    //   quads::AllQuads.insert(quads::AllQuads.end(), buttons::ButtonQuads.begin(), buttons::ButtonQuads.end());
    // }

    // if(fonts::TextQuads.size() > 0)
    // {
    //   quads::AllQuads.insert(quads::AllQuads.end(), fonts::TextQuads.begin(), fonts::TextQuads.end());
    // }

    // Assigning vertex index and vertex positions here, on the final table

     //std::cout <<" before fill_quad_vertex_data " << std::endl;
    for(int q=0; q < quads::AllQuads.size(); q++ )
    { 
      quads::_fill_quad_vertex_data(quads::AllQuads[q], q);
    }

    quads::COUNT_QUADS = quads::AllQuads.size();
    quads::REQ_SIZE_BUFFER = COUNT_QUADS*6*sizeof(float);
    // std::cout << "count entity quads: " << entity::EntityQuads.size() << std::endl;

    //std::cout <<" finished accumulate " << std::endl;
  }


  template <typename T>
  std::vector<quads::QuadData> make_quads(std::map<int, T> data, int object_type_id)
  {
    std::vector<quads::QuadData> quads = {};
    for (auto const& [k, v] : data)
    {
      quads::QuadData quad;
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
      quad.norm_y_start = v.norm_y_start;
      quad.norm_y_end = v.norm_y_end;


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
                      "    \"norm_y_start\": " << quads::AllQuads[i].norm_y_start     << ",\n"
                      "    \"norm_y_end\": " << quads::AllQuads[i].norm_y_end         << ",\n"
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

  template void quads::add_quads<buttons::ButtonData>(std::map<int, buttons::ButtonData>, int);
  template void quads::add_quads<menu::MenuData>(std::map<int, menu::MenuData>, int);
  template void quads::add_quads<debug::PointData>(std::map<int, debug::PointData>, int);
  template void quads::add_quads<entity::EntityData>(std::map<int, entity::EntityData>, int);
  template void quads::add_quads<fonts::TextData>(std::map<int, fonts::TextData>, int);
  template void quads::add_quads<maps::TileData>(std::map<int, maps::TileData>, int);

}