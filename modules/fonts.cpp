
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

#include <ft2build.h>
#include FT_FREETYPE_H  

#include "../dictionary.h"
#include "../dependencies/parallel_hashmap/phmap.h"


namespace fonts
{
  unsigned int FONT_MODEL_ID = 1;

  phmap::flat_hash_map<char, CharacterData> chars = {};
  phmap::flat_hash_map<int, TextData> texts = {};
  phmap::flat_hash_map<int, LabelData> labels;
  std::vector<int> TextIndex = {};
  std::vector<int> LabelIndex = {};
  models::ModelData FontTDD;
  int NEW_GAME_LABEL_ID = 0;

  void init(std::string font_name)
  {
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
    glGenTextures(1, &FONT_MODEL_ID);
    glBindTexture(GL_TEXTURE_2D, FONT_MODEL_ID);
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
      character.model_id = FONT_MODEL_ID;
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

    // Adjust for models
    // Font texture sort of another model

    models::ModelData MD;
    MD.id = FONT_MODEL_ID;
    MD.name = "font";
    MD.w = atlas_width;
    MD.h = atlas_height;
    MD.opengl_texture_id = FONT_MODEL_ID;

    models::models.insert(std::pair<int, models::ModelData>{FONT_MODEL_ID, MD});
    models::SceneModels.insert(std::pair<int, int>{FONT_MODEL_ID, FONT_MODEL_ID});

    std::cout << "Fonts Initialized" << std::endl;
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

      tdd.norm.x_start = chars[*p].offset;
      tdd.norm.x_end = chars[*p].offset + (chars[*p].bitmap_width/FontTDD.w);
 
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
  };


  void drop(int label_id)
  {
    fonts::labels.erase(label_id);
    utils::drop_id(fonts::LabelIndex, label_id);
  }

  void clear()
  {
    fonts::texts.clear();
    fonts::labels.clear();
    fonts::TextIndex.clear();
    fonts::LabelIndex.clear();
    fonts::NEW_GAME_LABEL_ID = -1;
  };


}

