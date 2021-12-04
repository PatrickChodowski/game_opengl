#include "camera.h"
#include "entity.h"
#include "maps.h"
#include "game.h"
#include "../dependencies/glm/mat4x4.hpp"
#include "../dependencies/glm/ext/matrix_transform.hpp"
#include "../dependencies/glm/gtc/matrix_transform.hpp"
#include "../dependencies/glm/gtc/type_ptr.hpp"
#include "../dependencies/glm/gtx/string_cast.hpp"
#include "../dictionary.h"
#include <iostream>

namespace camera
{
  Camera cam;
  glm::mat4 STATIC_MVP;
  glm::mat4 DYNAMIC_MVP;

  void reset()
  {
    cam.previous_x = 0;
    cam.previous_y = 0;
    cam.x = 0;
    cam.y = 0;
    cam.move_x = 0;
    cam.move_y = 0;
    cam.base_zoom = 1.0f;
    cam.zoom = 1.0f;
    cam.zoom_speed = 0.1f;
    cam.centric = true;
  };

  glm::mat4 gen_dynamic_mvp(float camera_move_x=0, float camera_move_y=0, float camera_zoom=1)
  {
    float z_window_width = game::WINDOW_WIDTH * camera_zoom;
    float z_window_height = game::WINDOW_HEIGHT * camera_zoom;
    glm::mat4 proj = glm::ortho(0.0f, z_window_width, z_window_height, 0.0f, -1.0f, 1.0f);
    // glm::vec3(0, 10, 0) moves tiles down
    // glm::vec3(10, 0, 0) moves tiles right
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(camera_move_x, camera_move_y, 0));
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    glm::mat4 mvp = proj*view*model;
    return mvp;
  }

  glm::mat4 gen_static_mvp()
  {
    glm::mat4 proj = glm::ortho(0.0f, game::WINDOW_WIDTH, game::WINDOW_HEIGHT, 0.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    glm::mat4 mvp = proj*view*model;
    return mvp;
  }

  void scale_quads(float camera_x, float camera_y, float camera_zoom)
  {
    float scale_factor = (1.0/camera_zoom);
    camera_x = (-1.0)*camera_x;
    //std::cout << " Scale quads quads::AllQuads.size: " << quads::AllQuads.size() << std::endl;

    for(int q = 0; q < quads::AllQuads.size(); q++)
    {
      float final_camera_x = 0.0f; 
      float final_camera_y = 0.0f;
      float final_scale_factor = 1.0f;
      
      if(quads::AllQuads[q].camera_type == CAMERA_DYNAMIC)
      {
        final_camera_x = camera_x; 
        final_camera_y = camera_y;
        final_scale_factor = scale_factor;
      } 
      quads::AllQuads[q].window_x = (quads::AllQuads[q].x + final_camera_x)*final_scale_factor;
      quads::AllQuads[q].window_y = (quads::AllQuads[q].y + final_camera_y)*final_scale_factor;
      quads::AllQuads[q].window_h = quads::AllQuads[q].h*final_scale_factor;
      quads::AllQuads[q].window_w = quads::AllQuads[q].w*final_scale_factor;

      // if(quads::AllQuads[q].object_type_id == OBJECT_TYPE_MENU)
      // {
      //   std::cout << " camera: " << final_camera_x << "," << final_camera_y << std::endl;
      //   std::cout << " scale factor: " << final_scale_factor << std::endl;
      //   std::cout << " world: " << quads::AllQuads[q].x << "," << quads::AllQuads[q].y << "," << quads::AllQuads[q].h << std::endl;
      //   std::cout << " window: " << quads::AllQuads[q].window_x << "," << quads::AllQuads[q].window_y << "," << quads::AllQuads[q].window_h << std::endl;
      // }
    }
  }

  float reverse_coord_x(float window_x, float camera_x, float camera_zoom)
  {
    camera_x = (-1.0)*camera_x;
    float world_x = (window_x * camera_zoom) - camera_x;
    return world_x;
  }

  float reverse_coord_y(float window_y, float camera_y, float camera_zoom)
  {
    float world_y = (window_y * camera_zoom) - camera_y;
    return world_y;
  }
}
