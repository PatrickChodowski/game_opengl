#ifndef CAMERA_H
#define CAMERA_H

// Camera settings for now

namespace camera
{
  float base_zoom = 1.0f;
  int base_speed = 6;
  int speed = 6;
  int x = 0;
  int y = 0;
  int move_x = 0;
  int move_y = 0;
  float zoom = 1.0f;
  float zoom_speed = 0.1f;
  bool centric = true;
  int tile_dim = TILE_DIM;

  // for mouse events usage
  std::vector<quads::ScaledQuad> scale_move_quads(std::vector<quads::Quad> quads, int camera_move_x=0, int camera_move_y=0)
  {
    camera::tile_dim = (float)TILE_DIM*float(camera::zoom);
    std::vector<quads::ScaledQuad> scaled_quads = {};
    float scale_factor = (1.0f/float(camera::zoom));

    for(int q=0; q<quads.size(); q++)
    {
      quads::ScaledQuad sq;
      if (quads[q].type_id != 1){
        sq.x = ((float)quads[q].x + (float)camera_move_x)*scale_factor;
        sq.y = ((float)quads[q].y + (float)camera_move_y)*scale_factor;
        sq.h = (float)quads[q].h*scale_factor;
        sq.w = (float)quads[q].w*scale_factor;
      } else {
        // menu:
        sq.x = (float)quads[q].x;
        sq.y = (float)quads[q].y;
        sq.h = (float)quads[q].h;
        sq.w = (float)quads[q].w;
      }
      sq.id = quads[q].id;
      scaled_quads.push_back(sq);
    }
    return scaled_quads;
  };

  
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


    
  }

  

    

}


#endif