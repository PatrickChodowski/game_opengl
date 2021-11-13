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
  glm::mat4 ZOOM_MVP;

  void reset2()
  {
    cam.base_speed = 6;
    cam.speed = 6;
    cam.previous_x = 0;
    cam.previous_y = 0;
    cam.x = 0;
    cam.y = 0;
    cam.move_x = 0;
    cam.move_y = 0;
    cam.tile_dim = game2::TILE_DIM;
    cam.base_zoom = 1.0f;
    cam.zoom = 1.0f;
    cam.zoom_speed = 0.1f;
    cam.centric = true;
  };

  glm::mat4 gen_dynamic_mvp(float camera_move_x=0, float camera_move_y=0, float camera_zoom=1)
  {
    float z_window_width = game2::WINDOW_WIDTH * camera_zoom;
    float z_window_height = game2::WINDOW_HEIGHT * camera_zoom;
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
    glm::mat4 proj = glm::ortho(0.0f, game2::WINDOW_WIDTH, game2::WINDOW_HEIGHT, 0.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    glm::mat4 mvp = proj*view*model;
    return mvp;
  }


  glm::mat4 gen_zoom_only_mvp(float camera_zoom)
  {
    float z_window_width = game2::WINDOW_WIDTH * camera_zoom;
    float z_window_height = game2::WINDOW_HEIGHT * camera_zoom;
    glm::mat4 proj = glm::ortho(0.0f, z_window_width, z_window_height, 0.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    glm::mat4 mvp = proj*view*model;
    return mvp;
  }

  void scale_quads(float camera_x, float camera_y, float camera_zoom)
  {
    float scale_factor = (1.0f/camera_zoom);
    camera_x = (-1.0)*camera_x;
    float final_camera_x; 
    float final_camera_y;
    //std::cout << " Scale quads quads::AllQuads.size: " << quads2::AllQuads.size() << std::endl;

    for(auto q : quads2::AllQuads)
    {
      if(q.camera_type == CAMERA_STATIC)
      {
        final_camera_x = 0.0; 
        final_camera_y = 0.0;
      } else 
      {
        final_camera_x = camera_x; 
        final_camera_y = camera_y;
      }
      q.window_x = (q.x + final_camera_x)*scale_factor;
      q.window_y = (q.y + final_camera_y)*scale_factor;
      q.window_h = q.h*scale_factor;
      q.window_w = q.w*scale_factor;

      if(q.id == 0)
      {
        std::cout << q.window_x <<"," << q.window_y << std::endl;
      }
    }


  }


  void inverse_scale()
  {

  };

}