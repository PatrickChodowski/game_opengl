#include "fonts2.h"
#include "models.h"
#include "utils.h"

#include <iostream>

#include <string>
#include <vector>

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

#include "../dictionary.h"
#include "../dependencies/parallel_hashmap/phmap.h"


namespace fonts
{
  unsigned int FONT_MODEL_ID = 1;
  std::string FONT_NAME = 'norse';

  void init()
  {
    std::vector<std::string> fonts_list = utils::list_json_files("fonts");
    for(int f=0; f < fonts_list.size(); f++)
    {
      fonts::read_data(fonts_list[m]);
    };
    std::cout << "Fonts Initialized" << std::endl;
  }


  void read_data(std::string& file_name)
  {
    fonts::FontsModelData FMD;
    std::string file_path = "./fonts/"+file_name+".json";
    std::string json_data = utils::read_text_file(file_path);
    JS::ParseContext context(json_data);
    context.allow_missing_members = true;
    context.parseTo(FMD);

    // propagate FMD.frames(map) from frames_list(vector)
    for(int f=0; f < FMD.frames_list.size(); f++)
    {
      FMD.frames.insert({FMD.frames_list[f].frame_id, FMD.frames_list[f]});
    }

    unsigned int opengl_texture_id = fonts::_load_font_texture_to_opengl(FMD.w, FMD.h, 4);

  }


  unsigned int _load_font_texture_to_opengl(int w, int h, int n_channels)
  {
    std::string texture_path = std::string("fonts/") + std::to_string(FONT_MODEL_ID) + "_" + FONT_NAME + "___spritesheet.png";
    stbi_set_flip_vertically_on_load(false);
    unsigned char *image_data = stbi_load(texture_path.c_str(), &w, &h, &n_channels, 4); 

    if(image_data == NULL)
    {
      std::cout << "Error while loading texture from " << texture_path << std::endl;
    };

    // We generate texture and assign it to texture/model_id. It doesnt have to be the same, but high chance it will
    unsigned int texture_id;
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