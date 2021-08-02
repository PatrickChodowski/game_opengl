#ifndef FONTS_H
#define FONTS_H


namespace fonts
{
  FT_Face face;
  FT_Library ft;
  int COUNT_VERTEX_ATTRIBUTES = 12;
  int VERTEX_OFFSET = 1;

  struct Character 
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
  };

  std::map<char, Character> character_map;

  textures::TextureData init()
  {
    // initialize library
    FT_Library ft;
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

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlas_width, atlas_height, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

    // fill out empty texture id
    int x = 0;
    int c_id = 0;
    for(int i = 32; i < 128; i++) 
    {
      if(FT_Load_Char(face, i, FT_LOAD_RENDER))
        continue;

      Character character;
      character.advance_x = g->advance.x;
      character.advance_y = g->advance.y;
      character.bitmap_width = g->bitmap.width;
      character.bitmap_height = g->bitmap.rows;
      character.bitmap_left = g->bitmap_left;
      character.bitmap_top = g->bitmap_top;
      character.offset = ((float)x/ (float)atlas_width);
      character.frame_id = c_id;
      character.texture_id = (int)texture_id;
      character_map.insert(std::pair<GLchar, Character>(i, character));

      std::cout << "Character: " << i << std::endl;
      std::cout << "Advance: X: " << character.advance_x << ", Y: " << character.advance_y  << std::endl;
      std::cout << "Bitmap: Width: " << character.bitmap_width << ", Height: " << character.bitmap_height << std::endl;
      std::cout << "Bitmap: Left: " << character.bitmap_left << ", Top: " << character.bitmap_top << std::endl;
      std::cout << "Offset: " << character.offset << std::endl;
      std::cout << "Texture id: " << character.texture_id << std::endl;

      glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);
      x += g->bitmap.width;
      c_id += 1;
    }

    GlCall(glBindTexture(GL_TEXTURE_2D, 0)); 
    logger::print("Fonts texture id: " + std::to_string(texture_id));
    FT_Done_Face(face);
    FT_Done_FreeType(ft);


    textures::TextureData TD;
    TD.id = texture_id;
    TD.type = "font";
    TD.name = "font";
    TD.width = atlas_width;
    TD.height = atlas_height;
    TD.opengl_texture_id = texture_id;


    return TD;
  }


  std::vector<quads::Quad> render_text(const char *text, 
                                       int x, 
                                       int y, 
                                       textures::TextureData FontTD,
                                       float scale = 1.0,
                                       float r_col = 0.5, 
                                       float g_col = 0.5, 
                                       float b_col = 0.5,
                                       float a_col = 1.0)
  {
    std::vector<quads::Quad> text_quads;

   // render given text at position x, y
    for(const char *p = text; *p; p++) 
    { 
      /* Skip glyphs that have no pixels */
      if(!character_map[*p].bitmap_width * scale || !character_map[*p].bitmap_height * scale)
        continue;

      float offset = character_map[*p].offset;
      float bitmap_width = character_map[*p].bitmap_width;
      float bitmap_height = character_map[*p].bitmap_height;
      float atlas_width = FontTD.width;
      float atlas_height = FontTD.height;


      // create quad here
      struct quads::Quad quad;
      quad.id = qm::gen_quad_id();
      quad.x = x + character_map[*p].bitmap_left * scale;
      quad.y = y - character_map[*p].bitmap_top * scale;
      quad.w = character_map[*p].bitmap_width * scale;
      quad.h = character_map[*p].bitmap_height * scale;
      quad.frame_id = character_map[*p].frame_id;
      quad.texture_id = character_map[*p].texture_id;
      //quad.texture_id = 4;
      quad.solid = 0.0f;
      quad.is_clicked = 0.0f;
      quad.r_col = r_col;
      quad.g_col = g_col;
      quad.b_col = b_col;
      quad.a_col = a_col;


      std::cout << "Rendering character: " << *p << std::endl;
      std::cout << "X: " << quad.x << std::endl;
      std::cout << "Y: " << quad.y << std::endl;


      // assign vertices
      quad.a = stoi(std::to_string(quad.id) + "001");
      quad.b = stoi(std::to_string(quad.id) + "002");
      quad.c = stoi(std::to_string(quad.id) + "003");
      quad.d = stoi(std::to_string(quad.id) + "004");

      quad.v_a.vertex_id = quad.a;
      quad.v_a.tile_id = quad.id;
      quad.v_a.frame_id = quad.frame_id;
      quad.v_a.x_pos = (float)quad.x;
      quad.v_a.y_pos = (float)quad.y;
      quad.v_a.z_pos = 0.0f;
      quad.v_a.r_col = quad.r_col;
      quad.v_a.g_col = quad.g_col;
      quad.v_a.b_col = quad.b_col;
      quad.v_a.a_col = quad.a_col;
      quad.v_a.tex_coord_x = offset;
      quad.v_a.tex_coord_y = 0.0f;

      //quad.v_a.tex_coord_x = 0.0f;
      //quad.v_a.tex_coord_y = 0.0f;
      quad.v_a.texture_id = quad.texture_id;
      quad.v_a.is_clicked = (float)quad.is_clicked;

      quad.v_b.vertex_id = quad.b;
      quad.v_b.tile_id = quad.id;
      quad.v_b.frame_id = quad.frame_id;
      quad.v_b.x_pos = (float)quad.x + (float)quad.w;
      quad.v_b.y_pos = (float)quad.y;
      quad.v_b.z_pos = 0.0f;
      quad.v_b.r_col = quad.r_col;
      quad.v_b.g_col = quad.g_col;
      quad.v_b.b_col = quad.b_col;
      quad.v_b.a_col = quad.a_col;
      quad.v_b.tex_coord_x = offset+((float)bitmap_width/(float)atlas_width);
      quad.v_b.tex_coord_y = 0.0f;

      // quad.v_b.tex_coord_x = 1.0f;
      // quad.v_b.tex_coord_y = 0.0f;
      quad.v_b.texture_id = quad.texture_id;
      quad.v_b.is_clicked = (float)quad.is_clicked;

      quad.v_c.vertex_id = quad.c;
      quad.v_c.tile_id = quad.id;
      quad.v_c.frame_id = quad.frame_id;
      quad.v_c.x_pos = (float)quad.x;
      quad.v_c.y_pos = (float)quad.y + (float)quad.h;
      quad.v_c.z_pos = 0.0f;
      quad.v_c.r_col = quad.r_col;
      quad.v_c.g_col = quad.g_col;
      quad.v_c.b_col = quad.b_col;
      quad.v_c.a_col = quad.a_col;
      quad.v_c.tex_coord_x = offset;
      quad.v_c.tex_coord_y = 1.0f;
      //quad.v_c.tex_coord_x = 0.0f;
      //quad.v_c.tex_coord_y = 1.0f;

      quad.v_c.texture_id = quad.texture_id;
      quad.v_c.is_clicked = (float)quad.is_clicked;

      quad.v_d.vertex_id = quad.d;
      quad.v_d.tile_id = quad.id;
      quad.v_d.frame_id = quad.frame_id;
      quad.v_d.x_pos = (float)quad.x + (float)quad.w;
      quad.v_d.y_pos = (float)quad.y + (float)quad.h;
      quad.v_d.z_pos = 0.0f;
      quad.v_d.r_col = quad.r_col;
      quad.v_d.g_col = quad.g_col;
      quad.v_d.b_col = quad.b_col;
      quad.v_d.a_col = quad.a_col;
      quad.v_d.tex_coord_x = offset + ((float)bitmap_width/(float)atlas_width);
      quad.v_d.tex_coord_y = 1.0f;

      //quad.v_d.tex_coord_x = 1.0f;
      //quad.v_d.tex_coord_y = 1.0f;
      quad.v_d.texture_id = quad.texture_id;
      quad.v_d.is_clicked = (float)quad.is_clicked;


      // create vindices 
      quad.i_left.a = quad.a;
      quad.i_left.b = quad.b;
      quad.i_left.c = quad.c;
      quad.i_right.a = quad.b;
      quad.i_right.b = quad.c;
      quad.i_right.c = quad.d;


      // push new x for next character
      x += character_map[*p].bitmap_width * scale;
      text_quads.push_back(quad);
    }
    quads::print_out_quads(text_quads);
    return text_quads;
  }








}




#endif
