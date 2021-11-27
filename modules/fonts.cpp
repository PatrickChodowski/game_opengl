
#include "fonts.h"
#include "textures.h"
#include "utils.h"

#include <iostream>
#include <map>

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

#include <ft2build.h>
#include FT_FREETYPE_H  

#include "../dictionary.h"


namespace fonts
{
  std::map<char, CharacterData> chars = {};
  std::vector<quads::QuadData> TextQuads = {};
  std::map<int, TextData> texts = {};
  std::map<int, LabelData> labels;
  std::vector<int> TextIndex = {};
  std::vector<int> LabelIndex = {};
  textures::TextureData FontTDD;
  int NEW_GAME_LABEL_ID = 0;

  void init(std::string font_name)
  {
    /*
      Init opens font file .ttf and reads-in all character glyphs into character map and texture that will be later sent to OpenGL 

      :params:
      font_name: Font name (filename from ./fonts)

      :returns:
      textures::TextureData  Font Texture Data containing atlas parameters and texture id
    */

    // initialize library
    FT_Face face;
    FT_Library ft;
    if(FT_Init_FreeType(&ft)) 
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    };

    std::string font_path = "fonts/"+font_name+".ttf";
    if(FT_New_Face(ft, font_path.c_str(), 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;  
    };

    FT_Set_Pixel_Sizes(face, 0, 48); 
    if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
    {
      std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;  
    }

    FT_GlyphSlot g = face->glyph;
    int w = 0;
    unsigned int h = 0;

    // get combined width of glyphs and maximum height 
    for (GLubyte c = 32; c < 128; c++)
    {

      // Load character glyph 
      if (FT_Load_Char(face, c, FT_LOAD_RENDER))
      {
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
        continue;
      }
      w += g->bitmap.width;
      h = std::max(h, g->bitmap.rows);
    }
    int atlas_width = w;
    int atlas_height = h;

    //std::cout << "ATLAS WIDTH: " << atlas_width << std::endl;
    //std::cout << "ATLAS HEIGHT: " << atlas_height << std::endl;


    // Generate texture - adds new texture id, based on what was generated before
    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      
    //std::cout << "Font texture ID: " << texture_id << std::endl;  

    // Set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, //target
                 0, //level 
                 GL_RED, //internal format
                 atlas_width,  // width
                 atlas_height,  // height
                 0,  // border
                 GL_RED,  // format
                 GL_UNSIGNED_BYTE,  // type
                 0); // data

    // fill out empty texture id
    int x = 0;
    int c_id = 0;
    for(int i = 32; i < 128; i++) 
    {
      if(FT_Load_Char(face, i, FT_LOAD_RENDER))
        continue;
        //https://learnopengl.com/In-Practice/Text-Rendering

      fonts::CharacterData character;
      character.advance_x = g->advance.x;
      character.advance_y = g->advance.y;
      character.bitmap_width = g->bitmap.width;
      character.bitmap_height = g->bitmap.rows;
      character.bitmap_left = g->bitmap_left;
      character.bitmap_top = g->bitmap_top;
      character.align = g->bitmap.rows - g->bitmap_top;
      character.offset = ((float)x/ (float)atlas_width);
      character.frame_id = c_id;
      character.texture_id = (int)texture_id;
      chars.insert(std::pair<GLchar, fonts::CharacterData>(i, character));

      //GLchar test_i = i;
      //std::cout << "font " << test_i << " " << character.bitmap_height << " " << character.bitmap_top << " " << character.align << std::endl;
      glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);
      x += g->bitmap.width;
      c_id += 1;
    }

    glBindTexture(GL_TEXTURE_2D, 0); 
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    textures::TextureData TD;
    TD.id = texture_id;
    TD.type = "font";
    TD.name = "font";
    TD.w = atlas_width;
    TD.h = atlas_height;
    TD.opengl_texture_id = texture_id;

    fonts::FontTDD = TD;
    textures::textures[texture_id] = TD;
    textures::BoundTextures.push_back(TD.opengl_texture_id);
  };

  int add(std::string& text, float x_start, float y_start, float camera_type, float scale, float r, float g, float b)
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
    ldd.scale = scale;
    labels[ldd.id] = ldd;
    return ldd.id;
  };

  void render_chars(fonts::LabelData ldd)
  { 
    float x = ldd.x_start;
    float y = ldd.y_start;

    for(const char *p = ldd.text.c_str(); *p; p++) 
    { 
      fonts::TextData tdd;
      tdd.id = utils::generate_id(fonts::TextIndex);
      tdd.texture_id = chars[*p].texture_id;
      tdd.frame_id = chars[*p].frame_id;

      tdd.x = x + chars[*p].bitmap_left * ldd.scale;
      tdd.y = y - ((chars[*p].bitmap_height - chars[*p].align) * ldd.scale);
      tdd.w = chars[*p].bitmap_width * ldd.scale;
      tdd.h = chars[*p].bitmap_height * ldd.scale;

      tdd.r = ldd.r;
      tdd.g = ldd.g;
      tdd.b = ldd.b;
      // tdd.r = 1.0;
      // tdd.g = 1.0;
      // tdd.b = 1.0;
      tdd.a = 1.0;
      tdd.camera_type = ldd.camera_type;
      tdd.is_clicked = false;

      tdd.norm_x_start = chars[*p].offset;
      tdd.norm_x_end = chars[*p].offset + (chars[*p].bitmap_width/FontTDD.w);

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
    fonts::TextQuads.clear();
    fonts::TextQuads = quads::make_quads(fonts::texts, OBJECT_TYPE_TEXT);
  };


  void drop(int label_id)
  {
    fonts::labels.erase(label_id);
    utils::drop_id(fonts::LabelIndex, label_id);
  }

  void clear()
  {
    fonts::TextQuads.clear();
    fonts::texts.clear();
    fonts::labels.clear();
    fonts::TextIndex.clear();
    fonts::LabelIndex.clear();
  };


}

