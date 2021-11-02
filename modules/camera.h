#include "../dependencies/glm/mat4x4.hpp"
#include "../dependencies/glm/ext/matrix_transform.hpp"
#include "../dependencies/glm/gtc/matrix_transform.hpp"
#include "../dependencies/glm/gtc/type_ptr.hpp"
#include "../dependencies/glm/gtx/string_cast.hpp"
#include <map>

#ifndef MODULES_CAMERA_H
#define MODULES_CAMERA_H

// Frame data. Table of distances between points for entity hero is per frame
namespace camera
{
  // Singleton struct (there is only one camera) with all camera parameters
  struct Camera
  {
    float base_speed = 6;
    float speed = 6;
    float previous_x = 0;
    float previous_y = 0;
    float x = 0;
    float y = 0;
    float move_x = 0;
    float move_y = 0;
    float tile_dim = 96;
    float base_zoom = 1.0f;
    float zoom = 1.0f;
    float zoom_speed = 0.1f;

    bool centric = true;
  };

  extern Camera cam;

  // Re-set camera struct "cam" to default values
  void reset2();

  // Inner method scaling particular data_table
  template <typename T>
  void _scale_table(std::map<int, T> data_table, float camera_x, float camera_y, float scale_factor);

  // Scale all objects (entities, tiles etc.) positions (x,y) and dimensions (w,h,diag) with camera parameters
  // Should scale: entities(different with static), map tiles, texts(not all)
  void scale(float camera_x, float camera_y, float camera_zoom);

  // Inverse the scaled items to original space
  void inverse_scale();

  // generate dynamic MVP transformation matrix
  glm::mat4 gen_dynamic_mvp(float camera_move_x, float camera_move_y, float camera_zoom);

}


#endif