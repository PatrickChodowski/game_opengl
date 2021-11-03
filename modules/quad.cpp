#include "quad.h"
#include <cmath> 
#include "entity.h"

namespace quad
{
  int COUNT_VERTEX_ATTRIBUTES = 14;
  float VERTEX_OFFSET = 1;
  int COUNT_QUADS = 0;
  int REQ_SIZE_BUFFER = 0;

  std::vector<QuadData> AllQuads;
  std::vector<int> UsedQuadIds = {};
  std::vector<int> UsedVertexIds = {};

  VertexData _fill_quad_vertex_data(QuadData& q)
  {
    VertexData v;
    v.v1_id = gen_vertex_id();
    v.v2_id = gen_vertex_id();
    v.v3_id = gen_vertex_id();
    v.v4_id = gen_vertex_id();

    // A
    v.v1_x = q.x;
    v.v1_y = q.y;
    // v.v1_tx_x = norm_x_start;
    // v.v1_tx_y = 0.0f;

    // B
    v.v2_x = q.x + q.w - quad::VERTEX_OFFSET;
    v.v2_y = q.y;
    // v.v2_tx_x = norm_x_end;
    // v.v2_tx_y = 0.0f;


    // C
    v.v3_x = q.x;
    v.v3_y = q.y + q.h - quad::VERTEX_OFFSET;
    // v.v3_tx_x = norm_x_start;
    // v.v3_tx_y = 1.0f;

    // D
    v.v4_x = q.x + q.w - quad::VERTEX_OFFSET;
    v.v4_y = q.y + q.h - quad::VERTEX_OFFSET;
    // v.v4_tx_x = norm_x_end;
    // v.v4_tx_y = 1.0f;

    return v;
  }

  int _find_next_quad_id()
  {
    int n = quad::UsedQuadIds.size();
    // for whole vector, find value that would be bigger than (index + 1)
    for (int i = 0; i < n; i++)
    {
      if (quad::UsedQuadIds[i] > (i+1)){
        return i+1;
      }
    }
    return n+1;
  }

  int gen_quad_id()
  {
    int next_quad_id = quad::_find_next_quad_id();
    quad::UsedQuadIds.push_back(next_quad_id);
    return next_quad_id;
  }

  int _find_next_vertex_id()
  {
    int n = quad::UsedVertexIds.size();
    // for whole vector, find value that would be bigger than (index + 1)
    for (int i = 0; i < n; i++)
    {
      if (quad::UsedVertexIds[i] > i){
        return i;
      }
    }
    return n;
  }

  int gen_vertex_id()
  {
    int next_vertex_id = quad::_find_next_vertex_id();
    quad::UsedVertexIds.push_back(next_vertex_id);
    return next_vertex_id;
  }

  int find_quad_id(int quad_id, std::vector<quad::QuadData> quads)
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

  float get_distance_between_quads(QuadData q1, QuadData q2)
  {
    float distance = std::sqrt(std::pow((q2.s_x - q1.s_x), 2) + std::pow((q2.s_y-q1.s_y), 2));
    return distance;
  }

  void accumulate()
  {
    quad::AllQuads.clear(); // resetting vector

    // // assign menu quads
    // if(menu::MenuQuads.size() > 0){
    //   quad::AllQuads.insert(quad::AllQuads.end(), menu::MenuQuads.begin(), menu::MenuQuads.end());
    // }

    // // assign map quads
    // if(maps::MapQuads.size() > 0){
    //   quad::AllQuads.insert(quad::AllQuads.end(), maps::MapQuads.begin(), maps::MapQuads.end());
    // }

    // assign entity quads
    //std::cout << "entities size: " << ent::EntityQuads.size() << std::endl;
    if(entity::EntityQuads.size() > 0){
      quad::AllQuads.insert(quad::AllQuads.end(), entity::EntityQuads.begin(), entity::EntityQuads.end());
    }

    // if(debug::DebugQuads.size() > 0){
    //   quad::AllQuads.insert(quad::AllQuads.end(), debug::DebugQuads.begin(), debug::DebugQuads.end());
    // }

    // // assign gui quads
    // if(gui::GuiQuads.size() > 0){
    //   quad::AllQuads.insert(quad::AllQuads.end(), gui::GuiQuads.begin(), gui::GuiQuads.end());
    // }

    // if(fonts::TextQuads.size() > 0){
    //   quad::AllQuads.insert(quad::AllQuads.end(), fonts::TextQuads.begin(), fonts::TextQuads.end());
    // }

    quad::COUNT_QUADS = quad::AllQuads.size();
    quad::REQ_SIZE_BUFFER = COUNT_QUADS*6*sizeof(float);
  }

}