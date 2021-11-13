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

  extern std::vector<quads2::QuadData> TextQuads;
  extern std::map<char, CharacterData> TextData;

  textures2::TextureData init(std::string font_name);
  void render(std::string text);











}

#endif