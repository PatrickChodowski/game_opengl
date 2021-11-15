
#include <cmath> 
#include <iostream>
#include <map>
#include <vector>
#include "entity.h"
#include "fonts.h"
#include "maps.h"
#include "menu.h"
#include "quads.h"
#include "textures.h"
#include "../dictionary.h"


namespace quads2
{
  int COUNT_VERTEX_ATTRIBUTES = 14;
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

  int find_quad_id(int quad_id, std::vector<quads2::QuadData> quads)
  {
    int quad_index = -1;
    for(int q = 0; q < quads.size(); q++)
    {
      if(quad_id == quads[q].id)
      {
        quad_index = q;
        break;
      }
    }
    return quad_index;
  }
  
  void delete_quad_id(int quad_id)
  {
    quads2::UsedQuadIds.erase(std::remove(quads2::UsedQuadIds.begin(), 
                                          quads2::UsedQuadIds.end(), quad_id), 
                                          quads2::UsedQuadIds.end());
  }

  void delete_vertex_id(int vertex_id)
  {
    quads2::UsedVertexIds.erase(std::remove(quads2::UsedVertexIds.begin(), 
                                            quads2::UsedVertexIds.end(), vertex_id), 
                                            quads2::UsedVertexIds.end());

  }

  void clear_quads_data(std::vector<quads2::QuadData>& vq)
  {
    for(int q=0; q < vq.size(); q++)
    {
      quads2::QuadData qdd = vq[q];
      quads2::delete_quad_id(qdd.id);
    }
    vq.clear();
  };

  void accumulate()
  {
    quads2::AllQuads.clear();
    quads2::UsedVertexIds.clear();

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

  // if(debug::DebugQuads.size() > 0){
    //   quad::AllQuads.insert(quad::AllQuads.end(), debug::DebugQuads.begin(), debug::DebugQuads.end());
    // }

    // // assign gui quads
    // if(gui::GuiQuads.size() > 0){
    //   quad::AllQuads.insert(quad::AllQuads.end(), gui::GuiQuads.begin(), gui::GuiQuads.end());
    // }

    if(fonts2::TextQuads.size() > 0)
    {
      quads2::AllQuads.insert(quads2::AllQuads.end(), fonts2::TextQuads.begin(), fonts2::TextQuads.end());
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


  template std::vector<quads2::QuadData> quads2::make_quads<entity::EntityData>(std::map<int, entity::EntityData>, int);
  template std::vector<quads2::QuadData> quads2::make_quads<fonts2::TextData>(std::map<int, fonts2::TextData>, int);
  template std::vector<quads2::QuadData> quads2::make_quads<maps2::TileData>(std::map<int, maps2::TileData>, int);
  template std::vector<quads2::QuadData> quads2::make_quads<menu2::ButtonData>(std::map<int, menu2::ButtonData>, int);
  

}