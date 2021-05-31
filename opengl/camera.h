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
  float zoom = 1.0f;
  float zoom_speed = 0.1f;
  bool centric = true;
  int tile_dim = TILE_DIM;

  struct ScaledQuad
  {
    int id;
    float x;
    float y;
    float w;
    float h;
  };


  // for mouse events usage
  //void scale(float zoom, int camera_move_x, int camera_move_y, std::vector<quads::Quad> level_quads)
  std::vector<ScaledQuad> scale_move_quads(std::vector<quads::Quad> level_quads, int camera_move_x=0, int camera_move_y=0)
  {
    camera::tile_dim = (float)TILE_DIM*float(camera::zoom);
    std::vector<ScaledQuad> scaled_level_quads = {};
    for(int q=0; q<level_quads.size(); q++)
    {
      ScaledQuad sq;
      sq.x = ((float)level_quads[q].x*(1.0f/float(camera::zoom))) + (float)camera_move_x;
      sq.y = ((float)level_quads[q].y*(1.0f/float(camera::zoom))) + (float)camera_move_y;
      sq.h = (float)level_quads[q].h*(1.0f/float(camera::zoom));
      sq.w = (float)level_quads[q].w*(1.0f/float(camera::zoom));
      sq.id = level_quads[q].id;
      scaled_level_quads.push_back(sq);

      // if(level_quads[q].id == 2){
      //   std::cout << "camera zoom:" << camera::zoom <<std::endl;
      //   std::cout << "scaled x: " << sq.x << " normal x: " <<  level_quads[q].x << std::endl;
      //   std::cout << "scaled y: " << sq.y << " normal y: " <<  level_quads[q].y << std::endl;
      //   std::cout << "scaled w: " << sq.w << " normal w: " <<  level_quads[q].w << std::endl;
      //   std::cout << "scaled h: " << sq.h << " normal h: " <<  level_quads[q].h << std::endl;
      // }
    }
    return scaled_level_quads;
  };

  
  glm::mat4 generate_mvp(float zoom, int camera_move_x=0, int camera_move_y=0)
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


    

}


#endif