#ifndef FONTS_H
#define FONTS_H


namespace fonts 
{
  struct Character 
  {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2 Size; // Size of glyph
    glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
    unsigned int Advance; // Offset to advance to next glyph
  };

  std::map<char, Character> Characters;
  FT_Face face;
  FT_Library ft;

  void clear_text()
  {
    fonts::TextQuads.clear();
  }

  void init_font()
  {
    if(FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    };

    if(FT_New_Face(ft, "fonts/OpenSans.ttf", 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;  
    };

    FT_Set_Pixel_Sizes(face, 0, 48); 
    if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
    {
      std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;  
    }
  }

  void no_idea()
  {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    for (unsigned char c = 0; c < 128; c++)
    {
      // load character glyph 
      if (FT_Load_Char(face, c, FT_LOAD_RENDER))
      {
          std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
          continue;
      }
      // generate texture
      unsigned int texture;
      glGenTextures(1, &texture);
      glBindTexture(GL_TEXTURE_2D, texture);
      glTexImage2D(
          GL_TEXTURE_2D,
          0,
          GL_RED,
          face->glyph->bitmap.width,
          face->glyph->bitmap.rows,
          0,
          GL_RED,
          GL_UNSIGNED_BYTE,
          face->glyph->bitmap.buffer
      );
      // set texture options
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      // now store character for later use
      Character character = {
          texture, 
          glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
          glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
          face->glyph->advance.x
      };
      Characters.insert(std::pair<char, Character>(c, character));
    }
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
  }


  void load_text_quads(std::string text, float x, float y, float scale, float r = 0.5, float g = 0.5, float b = 0.5)
  {
    // iterate through all characters
    std::vector<quads::Quad> text_quads;

    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];
        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        struct quads::Quad quad;
        quad.id = qm::gen_quad_id();
        quad.x = x + ch.Bearing.x * scale;
        quad.y = y - (ch.Size.y - ch.Bearing.y) * scale;
        quad.w = ch.Size.x * scale;
        quad.h = ch.Size.y * scale;

        quad.r_col = r;
        quad.g_col = g;
        quad.b_col = b;
        quad.a_col = 1.0f;

        quad.is_clicked = 0.0f;
        text_quads.push_back(quad);
        std::cout << "text quad id:" << std::endl;
        std::cout << quad.id << std::endl;
    }
    text_quads = quads::assign_vertices_no_texture(text_quads);

    // add new text to all texts
    for(int l=0; l < text_quads.size(); l++)
    {
      fonts::TextQuads.push_back(text_quads[l]);
    }
  }

}

#endif
