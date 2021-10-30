#include "camera.h"

namespace camera
{

  Camera cam;
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
    cam.tile_dim = 96;
    cam.base_zoom = 1.0f;
    cam.zoom = 1.0f;
    cam.zoom_speed = 0.1f;
    cam.centric = true;
  };

  void scale()
  {

  };

  void inverse_scale()
  {

  };

}