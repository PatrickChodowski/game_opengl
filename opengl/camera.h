#ifndef CAMERA_H
#define CAMERA_H


// Camera settings for now

namespace camera
{
  float base_zoom = 1.0f;
  int base_speed = 6;
  int speed = 6;
  int previous_x = 0;
  int previous_y = 0;
  int x = 0;
  int y = 0;
  int move_x = 0;
  int move_y = 0;
  float zoom = 1.0f;
  float zoom_speed = 0.1f;
  bool centric = true;
  int tile_dim = TILE_DIM;
  
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
    std::cout << " resetting camera " << std::endl;
    camera::zoom = 1;
    camera::x = 0;
    camera::y = 0;
    camera::move_x = 0;
    camera::move_y = 0;
    camera::tile_dim = TILE_DIM;    
  }

  // function to scale only X position value
  float scale_x(float x, int camera_x=0, float camera_zoom=1.0)
  {
    float scale_factor = (1.0f/float(camera_zoom));
    camera_x = (-1)*camera_x;
    float s_x = (x + (float)camera_x)*scale_factor;
    return s_x;
  }

  // function to scale only Y position value
  float scale_y(float y, int camera_y=0, float camera_zoom=1.0)
  {
    float scale_factor = (1.0f/float(camera_zoom));
    float s_y = (y + (float)camera_y)*scale_factor;
    return s_y;
  }

  float zoom_position(float pos, float camera_zoom=1.0)
  {
    float scale_factor = (1.0f/float(camera_zoom));
    float s_pos = pos*scale_factor;
    return s_pos;
  }

  // reverses click X with camera
  float reverse_scale_click_x(float x, int camera_x=0, float camera_zoom=1.0)
  {
    //float scale_factor = (1.0f/float(camera_zoom));
    float s_x = (x + (float)camera_x)*camera_zoom;
    return s_x;
  }

  // reverses click Y with camera
  float reverse_scale_click_y(float y, int camera_y=0, float camera_zoom=1.0)
  {
    //float scale_factor = (1.0f/float(camera_zoom));
    camera_y = (-1)*camera_y;
    float s_y = (y + (float)camera_y)*camera_zoom;
    return s_y;
  }

}


#endif