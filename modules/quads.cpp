
#include <cmath> 
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "debug.h"
#include "ecs.h"
#include "entity.h"
#include "fonts.h"
#include "logger.h"
#include "maps.h"
#include "models.h"
#include "quads.h"


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

  void init()
  {
    std::cout << "Quads Initialized" << std::endl;
  }


  
  int make_entity_quad(int entity_id)
  {
    quads::QuadData quad;
    quad.id = entity_id;
    quad.model_id = ecs::models.at(entity_id).model_id;
    quad.frame_id = ecs::models.at(entity_id).frame_id;
    if(quad.model_id > -1){
      quad.texture_id = models::SceneModels.at(quad.model_id);
      quad.norm = {models::models.at(quad.model_id).frames.at(quad.frame_id).norm_x_start,
                   models::models.at(quad.model_id).frames.at(quad.frame_id).norm_x_end,
                   models::models.at(quad.model_id).frames.at(quad.frame_id).norm_y_start,
                   models::models.at(quad.model_id).frames.at(quad.frame_id).norm_y_end};
    }
    quad.object_type_id = ecs::renderdatas.at(entity_id).object_type_id;
    quad.camera_type = ecs::renderdatas.at(entity_id).camera_type;
    quad.color.r = ecs::colors.at(entity_id).r;
    quad.color.g = ecs::colors.at(entity_id).g;
    quad.color.b = ecs::colors.at(entity_id).b;
    quad.color.a = ecs::colors.at(entity_id).a;
    quad.pos.x = ecs::positions.at(entity_id).x;
    quad.pos.y = ecs::positions.at(entity_id).y;
    quad.pos.z = ecs::positions.at(entity_id).z;
    quad.dims.w = ecs::dimensions.at(entity_id).w;
    quad.dims.h = ecs::dimensions.at(entity_id).h;
    quad.window_x = ecs::positions.at(entity_id).x;
    quad.window_y = ecs::positions.at(entity_id).y;
    quad.window_h = ecs::dimensions.at(entity_id).h;
    quad.window_w = ecs::dimensions.at(entity_id).w;
    quad.is_clicked = ecs::renderdatas.at(entity_id).is_clicked;
    quad.is_deleted = false;
    quads::AllQuads.push_back(quad);
    return quad.id;
  }


  template <typename T>
  int make_quad(T& data, int object_type_id)
  { 
    // if (object_type_id == OBJECT_TYPE_TEXT)
    // {
    //   std::cout << "Making quad id of type " << object_type_id << "MODEL ID:" << data.model_id<< std::endl;
    // }
    //std::cout << "Making quad for model id " << data.model_id << " object type id :" << object_type_id << std::endl;
    quads::QuadData quad;
    quad.id = (object_type_id+1)*100000 + data.id;
    quad.model_id = data.model_id;
    quad.frame_id = data.frame_id;

    //std::cout << "quad model id" << quad.model_id << "frame_id:" << quad.frame_id << std::endl;
    // Some quads dont have model, just using colors
    if(quad.model_id > -1){
      quad.texture_id = models::SceneModels.at(quad.model_id);
      quad.norm = {models::models.at(quad.model_id).frames.at(quad.frame_id).norm_x_start,
                   models::models.at(quad.model_id).frames.at(quad.frame_id).norm_x_end,
                   models::models.at(quad.model_id).frames.at(quad.frame_id).norm_y_start,
                   models::models.at(quad.model_id).frames.at(quad.frame_id).norm_y_end};
    }
    quad.object_id = data.id;
    quad.object_type_id = object_type_id;
    quad.camera_type = data.camera_type;
    
    quad.color = data.color;
    quad.pos = data.pos;
    quad.dims =  data.dims;
  
    quad.window_x = data.pos.x;
    quad.window_y = data.pos.y;
    quad.window_h = data.dims.h;
    quad.window_w = data.dims.w;

    quad.is_clicked = data.is_clicked;
    quad.is_deleted = false;
    quads::AllQuads.push_back(quad);

    // if (object_type_id == OBJECT_TYPE_TEXT)
    // {
    //   std::cout << "DONE Making quad id of type " << object_type_id << std::endl;
    // }

    //std::cout << "DONE Making quad" << std::endl;
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

  void print_quads_data()
  {
    std::cout << "### PRINT QUADS DATA ###" << std::endl;
      for(int i = 0; i < quads::AllQuads.size(); i++)
      {
        std::cout << " { \n" <<
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
                      << std::endl;
      }
  }



 // #include "../dependencies/parallel_hashmap/phmap.h"
  template void quads::add_quads<debug::PointData>(phmap::flat_hash_map<int, debug::PointData>&, int);
  template void quads::add_quads<entity::EntityData>(phmap::flat_hash_map<int, entity::EntityData>&, int);
  template void quads::add_quads<fonts::TextCharacterData>(phmap::flat_hash_map<int, fonts::TextCharacterData>&, int);
  template void quads::add_quads<maps::TileData>(phmap::flat_hash_map<int, maps::TileData>&, int);

  template int quads::make_quad<debug::PointData>(debug::PointData&, int);
  template int quads::make_quad<entity::EntityData>(entity::EntityData&, int);
  template int quads::make_quad<fonts::TextCharacterData>(fonts::TextCharacterData&, int);
  template int quads::make_quad<maps::TileData>(maps::TileData&, int);

}