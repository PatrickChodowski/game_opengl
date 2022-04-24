
#include "../modules/camera.h"
#include "../modules/ecs.h" 
#include "../modules/quads.h"

#include "../dictionary.h"
#include "../dependencies/parallel_hashmap/phmap.h"


bool test_camera__scale_quad_static()
{
  bool passed = false;
  quads::QuadData q;
  q.x = 100;
  q.y = 100;
  q.w = 100;
  q.h = 100;
  q.camera_type = CAMERA_STATIC;
  camera::_scale_quad_static(&q);
  if(q.x == q.window_x & q.y == q.window_y & q.h == q.window_h & q.w == q.window_w){
    passed = true;
  }
  return passed;
}

bool test_camera__scale_quad_dynamic()
{
  bool passed = false;
  quads::QuadData q;
  q.x = 100;
  q.y = 100;
  q.w = 100;
  q.h = 100;

  float camera_x = 50;
  float camera_y = 20;
  float scale_factor = 0.8;

  // expected values
  float e_x = (q.x + camera_x)*scale_factor;
  float e_y = (q.y + camera_y)*scale_factor;
  float e_h = q.h*scale_factor;
  float e_w = q.h*scale_factor;

  q.camera_type = CAMERA_DYNAMIC;
  camera::_scale_quad_dynamic(&q, camera_x, camera_y, scale_factor);
  if(e_x == q.window_x & e_y == q.window_y & e_h == q.window_h & e_w == q.window_w){
    passed = true;
  }
  return passed;
}