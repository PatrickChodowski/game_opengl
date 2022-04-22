
#include "../dependencies/glm/mat4x4.hpp"
#include "../dependencies/glm/ext/matrix_transform.hpp"
#include "../dependencies/glm/gtc/matrix_transform.hpp"
#include "../dependencies/glm/gtc/type_ptr.hpp"
#include "../dependencies/glm/gtx/string_cast.hpp"
#include "../dependencies/parallel_hashmap/phmap.h"

#ifndef MODULES_CAMERA_H
#define MODULES_CAMERA_H

// Frame data. Table of distances between points for entity hero is per frame
namespace camera
{
  // Singleton struct (there is only one camera) with all camera parameters
  struct Camera
  {
    float previous_x = 0;
    float previous_y = 0;
    float x = 0;
    float y = 0;
    float move_x = 0;
    float move_y = 0;
    float base_zoom = 1.0f;
    float zoom = 1.0f;
    float zoom_speed = 0.1f;

    bool centric = true;
  };

  extern Camera cam;
  extern glm::mat4 STATIC_MVP;
  extern glm::mat4 DYNAMIC_MVP;

  typedef void (*sig_ptr)();
  extern phmap::flat_hash_map<int, sig_ptr> CameraHandler;

  // initialize camera
  void init();
  
  // Re-set camera struct "cam" to default values
  void reset();

  // Scale quadData objects
  void scale_quads(float camera_x, float camera_y, float camera_zoom);

  // Scale static camera quads
  void _scale_quad_static();

  // Scale dynamic camera quads
  void _scale_quad_dynamic();

  // generate dynamic MVP transformation matrix
  glm::mat4 gen_dynamic_mvp(float camera_move_x, float camera_move_y, float camera_zoom);

  // generate static MVP
  glm::mat4 gen_static_mvp();

  // Reverse X from window to world 
  float reverse_coord_x(float window_x, float camera_x, float camera_zoom);

  // Reverse Y from window to world 
  float reverse_coord_y(float window_y, float camera_y, float camera_zoom);

}


#endif