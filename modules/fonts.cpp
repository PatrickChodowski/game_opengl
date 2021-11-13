
#include "fonts.h"
#include "textures.h"

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



namespace fonts2
{
  std::map<char, Character> character_map = {};


  textures2::TextureData init(std::string font_name)
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

    std::cout << "ATLAS WIDTH: " << atlas_width << std::endl;
    std::cout << "ATLAS HEIGHT: " << atlas_height << std::endl;


    // Generate texture - adds new texture id, based on what was generated before
    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      
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

      Character character;
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
      character_map.insert(std::pair<GLchar, Character>(i, character));

      //GLchar test_i = i;
      //std::cout << "font " << test_i << " " << character.bitmap_height << " " << character.bitmap_top << " " << character.align << std::endl;
      glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);
      x += g->bitmap.width;
      c_id += 1;
    }

    glBindTexture(GL_TEXTURE_2D, 0); 
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    textures2::TextureData TD;
    TD.id = texture_id;
    TD.type = "font";
    TD.name = "font";
    TD.w = atlas_width;
    TD.h = atlas_height;
    TD.opengl_texture_id = texture_id;

    return TD;
  }





}
