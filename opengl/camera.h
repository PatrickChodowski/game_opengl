#ifndef CAMERA_H
#define CAMERA_H

// Camera settings for now

namespace camera
{

  int speed = 20;
  int x = 0;
  int y = 0;
  int move_x = 0;
  int move_y = 0;
  int zoom = 0;
  int zoom_speed = 100;
  bool centric = true;


  glm::mat4 generate_mvp(int zoom, int camera_move_x=0, int camera_move_y=0)
  {
    // glm::mat4 proj = glm::ortho(0.0f, (float)WINDOW_WIDTH, 0.0f, (float)WINDOW_HEIGHT, -1.0f, 1.0f);


    float z_window_width = (float)WINDOW_WIDTH + (float)zoom;
    float z_window_height = (float)WINDOW_HEIGHT + (float)zoom;

    glm::mat4 proj = glm::ortho(0.0f, z_window_width, z_window_height, 0.0f, -1.0f, 1.0f);

    // glm::vec3(0, 10, 0) moves tiles down
    // glm::vec3(10, 0, 0) moves tiles right
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(camera_move_x, camera_move_y, 0));
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

    glm::mat4 mvp = proj*view*model;
    return mvp;
  }
    

}


#endif