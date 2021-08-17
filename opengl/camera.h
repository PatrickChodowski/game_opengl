#ifndef CAMERA_H
#define CAMERA_H

#include "../dictionary.h"

// Camera settings for now

namespace camera
{
  float base_zoom = 1.0f;
  int base_speed = 6;
  int speed = 6;
  int x = 0;
  int y = 0;
  int move_x = 0;
  int move_y = 0;
  float zoom = 1.0f;
  float zoom_speed = 0.1f;
  bool centric = true;
  int tile_dim = TILE_DIM;


  void scale_move_quads(int camera_move_x=0, int camera_move_y=0)
  // works on quads::AllQuads data, provides s_x, s_y, s_w, s_h, radius based on the camera move and tile type
  // scaled information is used for collisions, nav mesh, mouse events
  // scaling only quads from map and entity (probably will need to scale some texts later) but! later! not now
  {
    camera::tile_dim = (float)TILE_DIM*float(camera::zoom);
    float scale_factor = (1.0f/float(camera::zoom));

    for(int q=0; q<quads::AllQuads.size(); q++)
    {
      if (quads::AllQuads[q].type_id == QUAD_TYPE_MAP || quads::AllQuads[q].type_id == QUAD_TYPE_ENTITY){

        if(quads::AllQuads[q].entity_type_id != ENTITY_TYPE_ID_HERO)
        {
          quads::AllQuads[q].s_x = ((float)quads::AllQuads[q].x + (float)camera_move_x)*scale_factor;
          quads::AllQuads[q].s_y  = ((float)quads::AllQuads[q].y + (float)camera_move_y)*scale_factor;
          quads::AllQuads[q].s_h  = (float)quads::AllQuads[q].h*scale_factor;
          quads::AllQuads[q].s_w  = (float)quads::AllQuads[q].w*scale_factor;
          quads::AllQuads[q].s_diag = std::sqrt(std::pow((quads::AllQuads[q].s_w/2),2) + std::pow((quads::AllQuads[q].s_h/2),2));
        } else if (quads::AllQuads[q].entity_type_id == ENTITY_TYPE_ID_HERO) {
          quads::AllQuads[q].s_x = ((float)quads::AllQuads[q].x)*scale_factor;
          quads::AllQuads[q].s_y  = ((float)quads::AllQuads[q].y)*scale_factor;
          quads::AllQuads[q].s_h  = (float)quads::AllQuads[q].h*scale_factor;
          quads::AllQuads[q].s_w  = (float)quads::AllQuads[q].w*scale_factor;
          quads::AllQuads[q].s_diag = std::sqrt(std::pow((quads::AllQuads[q].s_w/2),2) + std::pow((quads::AllQuads[q].s_h/2),2));
        }
      }
    }
  };

  
  glm::mat4 generate_dynamic_mvp(float zoom, int camera_move_x=0, int camera_move_y=0)
  {
    float z_window_width = (float)WINDOW_WIDTH * (float)zoom;
    float z_window_height = (float)WINDOW_HEIGHT * (float)zoom;
    glm::mat4 proj = glm::ortho(0.0f, z_window_width, z_window_height, 0.0f, -1.0f, 1.0f);
    // glm::vec3(0, 10, 0) moves tiles down
    // glm::vec3(10, 0, 0) moves tiles right
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(camera_move_x, camera_move_y, 0));
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    glm::mat4 mvp = proj*view*model;
    return mvp;
  }

  glm::mat4 generate_static_mvp()
  {
    glm::mat4 proj = glm::ortho(0.0f, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, 0.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    glm::mat4 mvp = proj*view*model;
    return mvp;
  }

  glm::mat4 generate_zoom_only_mvp(float zoom)
  {
    float z_window_width = (float)WINDOW_WIDTH * (float)zoom;
    float z_window_height = (float)WINDOW_HEIGHT * (float)zoom;
    glm::mat4 proj = glm::ortho(0.0f, z_window_width, z_window_height, 0.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    glm::mat4 mvp = proj*view*model;
    return mvp;
  }


  void reset()
  {
    camera::zoom = 1;
    camera::x = 0;
    camera::y = 0;
    camera::move_x = 0;
    camera::move_y = 0;
    camera::tile_dim = TILE_DIM;    
  }

  

    

}


#endif