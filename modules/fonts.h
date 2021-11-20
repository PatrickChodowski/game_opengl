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

  // Stores in-game character data -> text, id, position, color. Used to make quads
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

  // Persistent across frames for both replaceable and nonreplaceble information.
  // Stores whole text, not single character
  struct LabelData 
  {
    int id;
    float r, g, b;
    float x_start;
    float y_start;
    float camera_type;
    float scale;
    std::string text;
  };


  // Vector of text quads to render 
  extern std::vector<quads2::QuadData> TextQuads;

  // Vector of used Text Ids
  extern std::vector<int> UsedTextIds;

  // Vector of used Text Ids
  extern std::vector<int> UsedLabelIds;

  // Map of character and character data in the texture
  extern std::map<char, CharacterData> chars;

  // Map of single character id, TextData (single characters stored)
  extern std::map<int, TextData> texts;

  // Map of Level's labels stored across frames
  extern std::map<int, LabelData> labels;

  // Font's texture data 
  extern textures2::TextureData FontTDD;

  // ID of the label for new game name input
  extern int NEW_GAME_LABEL_ID;

  // Initialize font texture and characters catalog
  void init(std::string font_name);

  // Add new label to labels. Returns label_id
  int add(std::string& text, float x_start, float y_start, float camera_type, float scale, float r, float g, float b);

  // Render characters of selected label data
  void render_chars(fonts2::LabelData ldd);

  // Render charcters from the texts map
  void render();

  // Clear all texts related data
  void clear();

  // Deletes label by label id
  void drop(int label_id);

}

#endif