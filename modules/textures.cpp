#include "textures.h"
#include "utils.h"
#include <string>
#include <map>
#include <vector>

// #define STB_IMAGE_IMPLEMENTATION
#include "../dependencies/stb_image.h"

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
  std::map<int, textures2::TextureData> textures = {};
  std::vector<unsigned int> BoundTextures = {};
  textures2::TextureData FontTD;

  unsigned int _load_texture_to_opengl(unsigned int texture_id, int w, int h, int n_channels, std::string name)
  {
    std::string texture_path = "data/assets/"+name+".png";
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

  void read_data(std::string name)
  {
    std::string data_path = "./data/assets/"+name+".json";
    textures2::TextureData TD;
    std::string json_data = utils2::read_text_file(data_path);
    JS::ParseContext context(json_data);
    context.parseTo(TD);

    // list animations available for texture (event_id_list)
    for(int a = 0; a < TD.anims_list.size(); a++)
    {
      TD.anims.insert({TD.anims_list[a].event_id, TD.anims_list[a]});
    }
    TD.anims_list.clear();

    // propagate TD.frames(map) from frames_list(vector)
    for(int f=0; f < TD.frames_list.size(); f++)
    {
      TD.frames.insert({TD.frames_list[f].frame_id, TD.frames_list[f]});
    }

    TD.frames_list.clear();

    textures2::textures.insert({TD.id, TD});
  };

  void load(int texture_id)
  {
    unsigned int opengl_texture_id = textures2::_load_texture_to_opengl(textures2::textures[texture_id].id, 
                                                                        textures2::textures[texture_id].w, 
                                                                        textures2::textures[texture_id].h, 
                                                                        4, 
                                                                        textures2::textures[texture_id].name);
    textures2::textures[texture_id].opengl_texture_id = opengl_texture_id;
    textures2::BoundTextures.push_back(textures2::textures[texture_id].opengl_texture_id);
  };

  void bind()
  {
    for(int t=0; t<textures2::BoundTextures.size(); t++)
    {
      // which texture slot we are actually binding 
      // first slot -> GL_TEXTURE0
      // Max 32, but depends on platform
      glActiveTexture(GL_TEXTURE0 + textures2::BoundTextures[t]);
      glBindTexture(GL_TEXTURE_2D, textures2::BoundTextures[t]);
    } 
  };

  void init()
  {
    std::vector<std::string> texture_list = utils2::list_json_files("data/assets/");
    // read texture data by name
    for(int t=0; t<texture_list.size(); t++)
    {
      textures2::read_data(texture_list[t]);
    }

    // load to opengl by texture_id
    for (auto const &x : textures2::textures)
    {
      textures2::load(x.first);
    }
  }

  void drop()
  {
    for(int t=0; t<textures2::BoundTextures.size(); t++)
    {
      glDeleteTextures(1, &textures2::BoundTextures[t]);
    } 
    textures2::BoundTextures.clear();
  }

}
