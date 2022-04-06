#include "fonts.h"
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

  std::vector<int> TextIndex;
  std::vector<int> LabelIndex;
  phmap::flat_hash_map<int, fonts::TextCharacterData> texts;
  phmap::flat_hash_map<int, fonts::LabelData> labels;


  int NEW_GAME_LABEL_ID;
  fonts::FontsModelData FontTDD;

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

  int add(std::string& text, float x_start, float y_start, float camera_type, int size, float r, float g, float b)
  {
    fonts::LabelData ldd;
    ldd.id = utils::generate_id(fonts::LabelIndex);
    ldd.text = text;
    ldd.x_start = x_start;
    ldd.y_start = y_start;
    ldd.camera_type = camera_type;
    ldd.r = r;
    ldd.g = g;
    ldd.b = b;
    ldd.size = size;
    fonts::labels[ldd.id] = ldd;
    return ldd.id;
  };

  // THIS NEEDS CHANGES
  void render_chars(fonts::LabelData ldd)
  {
    float x = ldd.x_start;
    float y = ldd.y_start;

    for(const char *p = ldd.text.c_str(); *p; p++) 
    { 
      fonts::TextCharacterData tdd;
      tdd.id = utils::generate_id(fonts::TextIndex);
      tdd.model_id = chars[*p].model_id;
      tdd.frame_id = chars[*p].frame_id;

      tdd.pos.x = x + chars[*p].bitmap_left * ldd.scale;
      tdd.pos.y = y - ((chars[*p].bitmap_height - chars[*p].align) * ldd.scale);
      tdd.pos.z = 0.95f;
      tdd.dims.w = chars[*p].bitmap_width;
      tdd.dims.h = chars[*p].bitmap_height * ldd.scale;
      //tdd.dims.h = chars[*p].bitmap_height;
    
      tdd.color.r = ldd.r;
      tdd.color.g = ldd.g;
      tdd.color.b = ldd.b;
      tdd.color.a = 1.0;
      tdd.camera_type = ldd.camera_type;
      tdd.is_clicked = false;

      x += ((chars[*p].bitmap_width * ldd.scale)+5);
      fonts::texts[tdd.id] = tdd;
    }
  };

  void render()
  {
    fonts::TextIndex.clear();
    fonts::texts.clear();
    for (auto const& [k, v] : fonts::labels)
    { 
      fonts::render_chars(v);
    } 
    quads::add_quads(fonts::texts, OBJECT_TYPE_TEXT);
  }

  void clear()
  {
    fonts::texts.clear();
    fonts::labels.clear();
    fonts::TextIndex.clear();
    fonts::LabelIndex.clear();
    fonts::NEW_GAME_LABEL_ID = -1;
  };


  void drop(int label_id)
  {
    fonts::labels.erase(label_id);
    utils::drop_id(fonts::LabelIndex, label_id);
  };





}