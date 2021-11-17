#include <map>
#include <string>

#include "quads.h"
#include "textures.h"

#ifndef MODULES_FONTS_H
#define MODULES_FONTS_H

namespace fonts2
{  
  
  struct CharacterData 
  {
    int texture_id;
    int frame_id;

    float advance_x;
    float advance_y;
    float bitmap_width;
    float bitmap_height; // or bitmap_rows
    float bitmap_left;
    float bitmap_top;
    float offset;  // offset of glyph
    float align;
  };

  // Stores in-game character data -> text, id, position, color, 
  struct TextData
  {
    int id;
    int texture_id;
    int frame_id;

    float x, y;
    float w, h;

    float r, g, b, a;
    float camera_type;

    float norm_x_start, norm_x_end;

    float char_width;
    float char_height;
    float atlas_width;
    float atlas_height;
    float offset;

    bool is_clicked;
  };

  // Vector of text quads to render 
  extern std::vector<quads2::QuadData> TextQuads;

  // Vector of used Text Ids
  extern std::vector<int> UsedTextIds;

  // Map of character and character data in the texture
  extern std::map<char, CharacterData> chars;

  // Map of text_id, TextData
  extern std::map<int, TextData> texts;

  // Map of temporary text_id, TextData
  extern std::map<int, TextData> temptexts;

  // Font's texture data 
  extern textures2::TextureData FontTDD;

  // Initialize font texture and characters catalog
  void init(std::string font_name);

  // Add the text to render
  void add(const char *text, float x, float y, float camera_type, float scale, bool temp);

  // Render charcters from the texts map
  void render();

  // Clear all texts related data
  void clear();

}

#endif