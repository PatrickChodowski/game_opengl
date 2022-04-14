
#include <cmath> 
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "ecs.h"
#include "logger.h"
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

  void init()
  {
    std::cout << "Quads Initialized" << std::endl;
  }
  
  void make_entity_quad(int entity_id)
  {
    quads::QuadData quad;
    quad.entity_id = entity_id;
    quad.model_id = ecs::models.at(entity_id).model_id;
    quad.frame_id = ecs::models.at(entity_id).frame_id;
    if(quad.model_id > -1){
      quad.texture_id = models::SceneModels.at(quad.model_id);
      quad.norm_x_start = models::models.at(quad.model_id).frames.at(quad.frame_id).norm_x_start;
      quad.norm_x_end = models::models.at(quad.model_id).frames.at(quad.frame_id).norm_x_end;
      quad.norm_y_start = models::models.at(quad.model_id).frames.at(quad.frame_id).norm_y_start;
      quad.norm_y_end = models::models.at(quad.model_id).frames.at(quad.frame_id).norm_y_end;
    }
    quad.entity_type_id = ecs::entities.at(entity_id).entity_type_id;
    quad.camera_type = ecs::renderdatas.at(entity_id).camera_type;
    ecs::PositionComponent pos = ecs::positions.at(entity_id);
    ecs::ColorComponent color = ecs::colors.at(entity_id);
    quad.r = color.r;
    quad.g = color.g;
    quad.b = color.b;
    quad.a = color.a;
    quad.x = pos.x;
    quad.y = pos.y;
    quad.z = pos.z;
    quad.w = pos.w;
    quad.h = pos.h;
    quad.window_x = pos.x;
    quad.window_y = pos.y;
    quad.window_h = pos.h;
    quad.window_w = pos.w;
    quad.is_clicked = ecs::renderdatas.at(entity_id).is_clicked;
    quads::AllQuads.push_back(quad);
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
    v.v1_z = q.z;
    v.v1_tx_x = q.norm_x_start;
    v.v1_tx_y = q.norm_y_start;
    //v.v1_tx_y = 0.0f;

    // B
    v.v2_x = q.x + q.w - quads::VERTEX_OFFSET;
    v.v2_y = q.y;
    v.v2_z = q.z;
    v.v2_tx_x = q.norm_x_end;
    v.v2_tx_y = q.norm_y_start;
    //v.v2_tx_y = 0.0f;

    // C
    v.v3_x = q.x;
    v.v3_y = q.y + q.h - quads::VERTEX_OFFSET;
    v.v3_z = q.z;
    v.v3_tx_x = q.norm_x_start;
    v.v3_tx_y = q.norm_y_end;
    //v.v3_tx_y = 1.0f;

    // D
    v.v4_x = q.x + q.w - quads::VERTEX_OFFSET;
    v.v4_y = q.y + q.h - quads::VERTEX_OFFSET;
    v.v4_z = q.z;
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

  void render()
  {
    for (auto const& [entity_id, render_data]: ecs::renderdatas)
    {
      quads::make_entity_quad(entity_id);
    }
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
                      "    \"entity_id\": " << quads::AllQuads[i].entity_id           << ",\n"
                      "    \"entity_type_id\": " << quads::AllQuads[i].entity_type_id << ",\n"
                      "    \"left_v1\": " << quads::AllQuads[i].i_left.v1             << ",\n"
                      "    \"left_v2\": " << quads::AllQuads[i].i_left.v2             << ",\n"
                      "    \"left_v3\": " << quads::AllQuads[i].i_left.v3             << ",\n"
                      "    \"right_v1\": " << quads::AllQuads[i].i_right.v1           << ",\n"
                      "    \"right_v2\": " << quads::AllQuads[i].i_right.v2           << ",\n"
                      "    \"right_v3\": " << quads::AllQuads[i].i_right.v3           << ",\n"
                      "    \"texture_id\": " << quads::AllQuads[i].texture_id         << ",\n"
                      "    \"frame_id\": " << quads::AllQuads[i].frame_id             << ",\n"
                      "    \"r\": " << quads::AllQuads[i].r                     << ",\n"
                      "    \"g\": " << quads::AllQuads[i].g                     << ",\n"
                      "    \"b\": " << quads::AllQuads[i].b                     << ",\n"
                      "    \"a\": " << quads::AllQuads[i].a                     << ",\n"
                      "    \"x\": " << quads::AllQuads[i].x                       << ",\n"
                      "    \"y\": " << quads::AllQuads[i].y                       << ",\n"
                      "    \"z\": " << quads::AllQuads[i].z                       << ",\n"
                      "    \"w\": " << quads::AllQuads[i].w                      << ",\n"
                      "    \"h\": " << quads::AllQuads[i].h                      << ",\n"
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

  void print_quads_data()
  {
    std::cout << "### PRINT QUADS DATA ###" << std::endl;
      for(int i = 0; i < quads::AllQuads.size(); i++)
      {
        std::cout << " { \n" <<
              "    \"entity_id\": " << quads::AllQuads[i].entity_id           << ",\n"
              "    \"entity_type_id\": " << quads::AllQuads[i].entity_type_id << ",\n"
              "    \"left_v1\": " << quads::AllQuads[i].i_left.v1             << ",\n"
              "    \"left_v2\": " << quads::AllQuads[i].i_left.v2             << ",\n"
              "    \"left_v3\": " << quads::AllQuads[i].i_left.v3             << ",\n"
              "    \"right_v1\": " << quads::AllQuads[i].i_right.v1           << ",\n"
              "    \"right_v2\": " << quads::AllQuads[i].i_right.v2           << ",\n"
              "    \"right_v3\": " << quads::AllQuads[i].i_right.v3           << ",\n"
              "    \"texture_id\": " << quads::AllQuads[i].texture_id         << ",\n"
              "    \"frame_id\": " << quads::AllQuads[i].frame_id             << ",\n"
              "    \"r\": " << quads::AllQuads[i].r                     << ",\n"
              "    \"g\": " << quads::AllQuads[i].g                     << ",\n"
              "    \"b\": " << quads::AllQuads[i].b                     << ",\n"
              "    \"a\": " << quads::AllQuads[i].a                     << ",\n"
              "    \"x\": " << quads::AllQuads[i].x                       << ",\n"
              "    \"y\": " << quads::AllQuads[i].y                       << ",\n"
              "    \"z\": " << quads::AllQuads[i].z                       << ",\n"
              "    \"w\": " << quads::AllQuads[i].w                      << ",\n"
              "    \"h\": " << quads::AllQuads[i].h                      << ",\n"
              "    \"camera_type\": " << quads::AllQuads[i].camera_type       << ",\n"
              "    \"window_x\": " << quads::AllQuads[i].window_x             << ",\n"
              "    \"window_y\": " << quads::AllQuads[i].window_y             << ",\n"
              "    \"window_w\": " << quads::AllQuads[i].window_w             << ",\n"
              "    \"window_h\": " << quads::AllQuads[i].window_h             << ",\n"
              "    \"norm_x_start\": " << quads::AllQuads[i].norm_x_start    << ",\n"
              "    \"norm_x_end\": " << quads::AllQuads[i].norm_x_end        << ",\n"
              "    \"norm_y_start\": " << quads::AllQuads[i].norm_y_start    << ",\n"
              "    \"norm_y_end\": " << quads::AllQuads[i].norm_y_end         << ",\n"
              "    \"is_clicked\": " << quads::AllQuads[i].is_clicked         << "\n"
              << std::endl;
      }
  }

}

