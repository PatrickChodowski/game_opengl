#include "camera.h"
#include "maps.h"
#include "game.h"
#include "quads.h"

#include "../dependencies/parallel_hashmap/phmap.h"
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
  phmap::flat_hash_map<int, sig_ptr> CameraScale;

  void init()
  {
    camera::CameraScale[CAMERA_DYNAMIC] = _scale_quad_dynamic;
    camera::CameraScale[CAMERA_STATIC] = _scale_quad_static;
    camera::CameraScale[CAMERA_HIDDEN] = _scale_quad_hidden;
  }
 

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
    for(int q = 0; q < quads::AllQuads.size(); q++)
    {
      camera::CameraScale[quads::AllQuads[q].camera_type](&quads::AllQuads[q], camera_x, camera_y, scale_factor);
    }
  };

  void _scale_quad_dynamic(quads::QuadData *q, float camera_x, float camera_y, float scale_factor)
  {
    q->window_x = (q->x + camera_x)*scale_factor;
    q->window_y = (q->y + camera_y)*scale_factor;
    q->window_h = q->h*scale_factor;
    q->window_w = q->w*scale_factor;
  };

  void _scale_quad_static(quads::QuadData *q, float camera_x, float camera_y, float scale_factor)
  {
    // arguments just placeholders
    q->window_x = q->x;
    q->window_y = q->y;
    q->window_h = q->h;
    q->window_w = q->w;
  };

  void _scale_quad_hidden(quads::QuadData *q, float camera_x, float camera_y, float scale_factor)
  {

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
