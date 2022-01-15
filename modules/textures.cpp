#include "fonts.h"
#include "textures.h"
#include "utils.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
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

#include "../dictionary.h"
#include "../dependencies/parallel_hashmap/phmap.h"
#include "../dependencies/parallel_hashmap/btree.h"

namespace textures
{
  phmap::btree_map<int, textures::TextureData> textures = {};
  std::vector<unsigned int> BoundTextures = {};

  unsigned int _load_texture_to_opengl(unsigned int texture_id, int w, int h, int n_channels, std::string name)
  {
    std::string texture_path = "assets/"+name+".png";
    stbi_set_flip_vertically_on_load(false);
    unsigned char *image_data = stbi_load(texture_path.c_str(), &w, &h, &n_channels, 4); 

    if(image_data == NULL)
    {
      std::cout << "Error while loading texture from " << texture_path << std::endl;
    };

    // std::cout << "texture id before " << texture_id << std::endl;
    glGenTextures(1, &texture_id); 
    glBindTexture(GL_TEXTURE_2D, texture_id); 
    // std::cout << "texture id after " << texture_id << std::endl;

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
    textures::TextureData TD;
    std::string json_data = utils::read_text_file(data_path);
    JS::ParseContext context(json_data);
    context.parseTo(TD);

    // propagate TD.frames(map) from frames_list(vector)
    for(int f=0; f < TD.frames_list.size(); f++)
    {
      TD.frames.insert({TD.frames_list[f].frame_id, TD.frames_list[f]});
    }

    TD.frames_list.clear();


    // propagate TD.anims(map) from anims_list(vector)
    for(int a=0; a < TD.anim_list.size(); a++)
    {
      TD.anims.insert({TD.anim_list[a].id, TD.anim_list[a]});
    }
    TD.anim_list.clear();
    

    textures::textures.insert({TD.id, TD});
  };

  void load(int texture_id)
  {
    unsigned int opengl_texture_id = textures::_load_texture_to_opengl(textures::textures[texture_id].id, 
                                                                        textures::textures[texture_id].w, 
                                                                        textures::textures[texture_id].h, 
                                                                        4, 
                                                                        textures::textures[texture_id].name);
    textures::textures[texture_id].opengl_texture_id = opengl_texture_id;
    textures::BoundTextures.push_back(textures::textures[texture_id].opengl_texture_id);

  };

  void bind()
  {
    for(int t=0; t<textures::BoundTextures.size(); t++)
    {
      //std::cout << " Binding texture " <<  textures::BoundTextures[t] << std::endl;
      // which texture slot we are actually binding 
      // first slot -> GL_TEXTURE0
      // Max 32, but depends on platform
      glActiveTexture(GL_TEXTURE0 + textures::BoundTextures[t]);
      glBindTexture(GL_TEXTURE_2D, textures::BoundTextures[t]);
    } 
  };

  void init()
  {
    std::vector<std::string> texture_list = utils::list_json_files("data/assets/");
    // read texture data by name
    for(int t=0; t<texture_list.size(); t++)
    {
      textures::read_data(texture_list[t]);
    }

    // load to opengl by texture_id
    for (auto const &x : textures::textures)
    {
      if(x.first != FONT_TEXTURE_ID)
      {
        textures::load(x.first);
      }
    }
    std::cout << "Textures Initialized" << std::endl;
  };

  void drop()
  {
    for(int t=0; t<textures::BoundTextures.size(); t++)
    {
      glDeleteTextures(1, &textures::BoundTextures[t]);
    } 
    textures::BoundTextures.clear();
  };

  float _get_normalized_frame_x_start(int texture_id, int frame_id)
  {
    textures::TextureData tdd = textures::textures[texture_id];
    return tdd.frames[frame_id].x/tdd.w;
  };

  float _get_normalized_frame_x_end(int texture_id, int frame_id)
  {
    textures::TextureData tdd = textures::textures[texture_id];
    return (tdd.frames[frame_id].x + tdd.frames[frame_id].w)/tdd.w;
  };

  float _get_normalized_frame_y_start(int texture_id, int frame_id)
  {
    textures::TextureData tdd = textures::textures[texture_id];
    return tdd.frames[frame_id].y/tdd.h;
  };

  float _get_normalized_frame_y_end(int texture_id, int frame_id)
  {
    textures::TextureData tdd = textures::textures[texture_id];
    return (tdd.frames[frame_id].y + tdd.frames[frame_id].h)/tdd.h;
  };



  // not the clearest json from here
  void log()
  {
    const char* log_path = "logs/textures.json";
    std::ofstream log_file (log_path);
    std::string end_str = " }, \n";
    if (log_file.is_open())
    {
      log_file << "[ \n";
      for (auto const& [k, v] : textures::textures)
      {
        // if(i == (textures::textures.size() - 1))
        // {
        //   end_str = " } \n";
        // }

        log_file << " { \n" <<
                      "    \"key\": " << k                                  << ",\n"
                      "    \"id\": " << v.id                                << ",\n"
                      "    \"opengl_texture_id\": " << v.opengl_texture_id  << ",\n"
                      "    \"w\": " << v.w                                  << ",\n"
                      "    \"h\": " << v.h                                  << ",\n"
                      "    \"type\": " << v.type                            << ",\n"
                      "    \"name\": " << v.name                            << " \n"
                      << end_str;
      }
      log_file << "] \n";
      log_file.close();
    }
  }


}
