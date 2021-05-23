
#ifndef QUADS_H
#define QUADS_H

// everything regarding rendering quads - levels, objects, entities etc.

namespace quads
{
  // vertex index
  struct Vindex
  {  
    int a; 
    int b;
    int c;
  };


  // Vertex information
  struct Vertex
  {  
    // info send to GPU:
    double x_pos; 
    double y_pos;
    double z_pos;

    double r_col;
    double g_col;
    double b_col;
    double a_col;

    double tile_type;
    double tex_coord_x;
    double tex_coord_y;
    double texture_id;

    /// not counted as VERTEX ATTRIBUTES - yet
    int tile_id;
    int vertex_id;
  };

  int COUNT_VERTEX_ATTRIBUTES = 11;
  int VERTEX_OFFSET = 1;


  // Quad will contain information about:
  // - original position (x,y of top left corner)
  // - type, solid, id
  // - vertex ids for a,b,c,d
  // - Vindices with vertex ids
  // - actual vertex information

  struct Quad
  {
    int id;
    int x;
    int y;
    int type;
    bool solid;

    // vertex IDS:
    int a;
    int b;
    int c;
    int d;

    // a b
    // c d

    // Vertex information
    Vertex v_a;
    Vertex v_b;
    Vertex v_c;
    Vertex v_d;

    // vindices
    struct Vindex i_left;
    // a b
    // c
    struct Vindex i_right;
    //   b
    // c d

    std::string sheet_name;

  };


  std::vector<Quad> assign_vertices(std::vector<Quad> quads)
  {
    for(int i = 0; i < quads.size(); i++)
    { 
      // add vertex ids
      int j = i * 4;
      quads[i].a = j;
      quads[i].b = j+1;
      quads[i].c = j+2;
      quads[i].d = j+3;

      // create vertex struct - A
      quads[i].v_a.vertex_id = quads[i].a;
      quads[i].v_a.tile_id = quads[i].id;
      quads[i].v_a.tile_type = quads[i].type;
      quads[i].v_a.x_pos = (float)quads[i].x;
      quads[i].v_a.y_pos = (float)quads[i].y;
      quads[i].v_a.z_pos = 0.0f;
      quads[i].v_a.r_col = 0.0f;
      quads[i].v_a.g_col = 0.0f;
      quads[i].v_a.b_col = 0.0f;
      quads[i].v_a.a_col = 1.0f;
      quads[i].v_a.tex_coord_x = textures::tile_frames_catalog[quads[i].sheet_name][quads[i].type].norm_x_start;
      quads[i].v_a.tex_coord_y = 0.0f;
      quads[i].v_a.texture_id = textures::tile_frames_catalog[quads[i].sheet_name][quads[i].type].texture_id;

      // create vertex struct - B
      quads[i].v_b.vertex_id = quads[i].b;
      quads[i].v_b.tile_id = quads[i].id;
      quads[i].v_b.tile_type = quads[i].type;
      quads[i].v_b.x_pos = (float)quads[i].x + (float)TILE_DIM  - (float)VERTEX_OFFSET;
      quads[i].v_b.y_pos = (float)quads[i].y;
      quads[i].v_b.z_pos = 0.0f;
      quads[i].v_b.r_col = 0.0f;
      quads[i].v_b.g_col = 0.0f;
      quads[i].v_b.b_col = 0.0f;
      quads[i].v_b.a_col = 1.0f;
      quads[i].v_b.tex_coord_x = textures::tile_frames_catalog[quads[i].sheet_name][quads[i].type].norm_x_end;
      quads[i].v_b.tex_coord_y = 0.0f;
      quads[i].v_b.texture_id = textures::tile_frames_catalog[quads[i].sheet_name][quads[i].type].texture_id;


      // create vertex struct - C
      quads[i].v_c.vertex_id = quads[i].c;
      quads[i].v_c.tile_id = quads[i].id;
      quads[i].v_c.tile_type = quads[i].type;
      quads[i].v_c.x_pos = (float)quads[i].x;
      quads[i].v_c.y_pos = (float)quads[i].y + (float)TILE_DIM - (float)VERTEX_OFFSET;
      quads[i].v_c.z_pos = 0.0f;
      quads[i].v_c.r_col = 0.0f;
      quads[i].v_c.g_col = 0.0f;
      quads[i].v_c.b_col = 0.0f;
      quads[i].v_c.a_col = 1.0f;
      quads[i].v_c.tex_coord_x = textures::tile_frames_catalog[quads[i].sheet_name][quads[i].type].norm_x_start;
      quads[i].v_c.tex_coord_y = 1.0f;
      quads[i].v_c.texture_id = textures::tile_frames_catalog[quads[i].sheet_name][quads[i].type].texture_id;


      // create vertex struct - D
      quads[i].v_d.vertex_id = quads[i].d;
      quads[i].v_d.tile_id = quads[i].id;
      quads[i].v_d.tile_type = quads[i].type;
      quads[i].v_d.x_pos = (float)quads[i].x + (float)TILE_DIM  - (float)VERTEX_OFFSET;
      quads[i].v_d.y_pos = (float)quads[i].y + (float)TILE_DIM - (float)VERTEX_OFFSET;
      quads[i].v_d.z_pos = 0.0f;
      quads[i].v_d.r_col = 0.0f;
      quads[i].v_d.g_col = 0.0f;
      quads[i].v_d.b_col = 0.0f;
      quads[i].v_d.a_col = 1.0f;
      quads[i].v_d.tex_coord_x = textures::tile_frames_catalog[quads[i].sheet_name][quads[i].type].norm_x_end;
      quads[i].v_d.tex_coord_y = 1.0f;
      quads[i].v_d.texture_id = textures::tile_frames_catalog[quads[i].sheet_name][quads[i].type].texture_id;


      // create vindices 
      quads[i].i_left.a = quads[i].a;
      quads[i].i_left.b = quads[i].b;
      quads[i].i_left.c = quads[i].c;

      quads[i].i_right.a = quads[i].b;
      quads[i].i_right.b = quads[i].c;
      quads[i].i_right.c = quads[i].d;
    }
    return quads;
  }


}


#endif

