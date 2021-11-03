#include "textures.h"
#include <string>
#include <map>
#include <vector>
#include "../dependencies/stb_image.h"

// Opengl packages
#include <GL/glew.h> 
#ifdef TARGET_OS_MAC
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#endif

#ifdef __linux__
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif

namespace textures2
{
  std::map<int, textures2::TextureData> Catalog = {};
  std::vector<unsigned int> BoundTextures = {};
  textures2::TextureData FontTD;

  unsigned int _load_texture_to_opengl(unsigned int texture_id, int w, int h, int n_channels, std::string name)
  {
    std::string texture_path = "assets/"+name+".png";
    stbi_set_flip_vertically_on_load(false);
    unsigned char *image_data = stbi_load(texture_path.c_str(), &w, &h, &n_channels, 4); 
    glGenTextures(1, &texture_id); 
    glBindTexture(GL_TEXTURE_2D, texture_id); 

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Send Image data to GPU here
    glTexImage2D(GL_TEXTURE_2D, //target 
                 0, //level, 0 is base image
                 GL_RGBA, //internalformat
                 w,
                 h,  
                 0,  // border
                 GL_RGBA,  // format
                 GL_UNSIGNED_BYTE,  // type
                 image_data); // data

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(image_data);
    return texture_id;
  };

















}
