#ifndef MODULES_CAMERA_H
#define MODULES_CAMERA_H

// Frame data. Table of distances between points for entity hero is per frame
namespace camera
{
  // Singleton struct (there is only one camera) with all camera parameters
  struct Camera
  {
    int base_speed = 6;
    int speed = 6;
    int previous_x = 0;
    int previous_y = 0;
    int x = 0;
    int y = 0;
    int move_x = 0;
    int move_y = 0;
    int tile_dim = 96;

    float base_zoom = 1.0f;
    float zoom = 1.0f;
    float zoom_speed = 0.1f;

    bool centric = true;
  };

  extern Camera cam;

  // Re-set camera struct "cam" to default values
  void reset2();

  // Scale anything with camera
  void scale();

  // Inverse the scaled items to original space
  void inverse_scale();

}


#endif