
#ifndef QUADS_H
#define QUADS_H

// everything regarding rendering quads - levels, objects, entities etc.

namespace quads
{

  int COUNT_VERTEX_ATTRIBUTES = 12;
  int VERTEX_OFFSET = 1;

    void print_out_quads(std::vector<Quad> quads)
    {
      for(int t=0; t<quads.size(); t++)
    //for(int t=0; t<2; t++)
      {
        std::cout << "Quad ID: " << quads[t].id << std::endl; 
        std::cout << "Texture ID: " << quads[t].texture_id << std::endl;
        std::cout << "Frame ID: " << quads[t].frame_id << std::endl;
        std::cout << "X: " << quads[t].x << std::endl; 
        std::cout << "Y: " << quads[t].y << std::endl; 
        std::cout << "Width: " << quads[t].w << std::endl; 
        std::cout << "Height: " << quads[t].h << std::endl; 

        std::cout << "Vertices positions:"  << std::endl;
        std::cout << "Vertex ID: " << quads[t].a << " X:" << quads[t].v_a.x_pos << ", Y:" << quads[t].v_a.y_pos << ", tcX:" << quads[t].v_a.tex_coord_x << ", tcY:" << quads[t].v_a.tex_coord_y << std::endl;
        std::cout << "Vertex ID: " << quads[t].b << " X:" << quads[t].v_b.x_pos << ", Y:" << quads[t].v_b.y_pos << ", tcX:" << quads[t].v_b.tex_coord_x << ", tcY:" << quads[t].v_b.tex_coord_y << std::endl;
        std::cout << "Vertex ID: " << quads[t].c << " X:" << quads[t].v_c.x_pos << ", Y:" << quads[t].v_c.y_pos << ", tcX:" << quads[t].v_c.tex_coord_x << ", tcY:" << quads[t].v_c.tex_coord_y << std::endl;
        std::cout << "Vertex ID: " << quads[t].d << " X:" << quads[t].v_d.x_pos << ", Y:" << quads[t].v_d.y_pos << ", tcX:" << quads[t].v_d.tex_coord_x << ", tcY:" << quads[t].v_d.tex_coord_y << std::endl;

        std::cout << "Vindices:" << std::endl <<
        quads[t].i_left.a << " " << quads[t].i_left.b << " " << quads[t].i_left.c << std::endl <<
        quads[t].i_right.a << " " << quads[t].i_right.b << " " << quads[t].i_right.c << std::endl;
      }
    }

    void print_out_quad_map(std::map<int, quads::Quad> quad_map)
    {
        for (auto const& x : quad_map)
        {
            std::cout << "Quad id : " << x.first << x.second.id << " - " << x.second.texture_id <<  " - " << x.second.frame_id << std::endl << 
              x.second.x  << "," << x.second.y << std::endl <<
              x.second.w  << "," << x.second.h << std::endl;
        }
    }


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

      float norm_x_start = (float)textures::Catalog[quads[i].texture_id].frames[quads[i].frame_id].x/
      (float)textures::Catalog[quads[i].texture_id].width;

      float norm_x_end =  (float)(textures::Catalog[quads[i].texture_id].frames[quads[i].frame_id].x + 
      textures::Catalog[quads[i].texture_id].frames[quads[i].frame_id].w)/
      (float)textures::Catalog[quads[i].texture_id].width;

      // create vertex struct - A
      quads[i].v_a.vertex_id = quads[i].a;
      quads[i].v_a.tile_id = quads[i].id;
      quads[i].v_a.frame_id = quads[i].frame_id;
      quads[i].v_a.x_pos = (float)quads[i].x;
      quads[i].v_a.y_pos = (float)quads[i].y;
      quads[i].v_a.z_pos = 0.0f;
      quads[i].v_a.r_col = 0.0f;
      quads[i].v_a.g_col = 0.0f;
      quads[i].v_a.b_col = 0.0f;
      quads[i].v_a.a_col = 1.0f;
      quads[i].v_a.tex_coord_x = norm_x_start;
      quads[i].v_a.tex_coord_y = 0.0f;
      quads[i].v_a.texture_id = textures::Catalog[quads[i].texture_id].opengl_texture_id;
      quads[i].v_a.is_clicked = (float)quads[i].is_clicked;

      // create vertex struct - B
      quads[i].v_b.vertex_id = quads[i].b;
      quads[i].v_b.tile_id = quads[i].id;
      quads[i].v_b.frame_id = quads[i].frame_id;
      quads[i].v_b.x_pos = (float)quads[i].x + (float)TILE_DIM  - (float)VERTEX_OFFSET;
      quads[i].v_b.y_pos = (float)quads[i].y;
      quads[i].v_b.z_pos = 0.0f;
      quads[i].v_b.r_col = 0.0f;
      quads[i].v_b.g_col = 0.0f;
      quads[i].v_b.b_col = 0.0f;
      quads[i].v_b.a_col = 1.0f;
      quads[i].v_b.tex_coord_x = norm_x_end;
      quads[i].v_b.tex_coord_y = 0.0f;
      quads[i].v_b.texture_id = textures::Catalog[quads[i].texture_id].opengl_texture_id;
      quads[i].v_b.is_clicked = (float)quads[i].is_clicked;

      // create vertex struct - C
      quads[i].v_c.vertex_id = quads[i].c;
      quads[i].v_c.tile_id = quads[i].id;
      quads[i].v_c.frame_id = quads[i].frame_id;
      quads[i].v_c.x_pos = (float)quads[i].x;
      quads[i].v_c.y_pos = (float)quads[i].y + (float)TILE_DIM - (float)VERTEX_OFFSET;
      quads[i].v_c.z_pos = 0.0f;
      quads[i].v_c.r_col = 0.0f;
      quads[i].v_c.g_col = 0.0f;
      quads[i].v_c.b_col = 0.0f;
      quads[i].v_c.a_col = 1.0f;
      quads[i].v_c.tex_coord_x = norm_x_start;
      quads[i].v_c.tex_coord_y = 1.0f;
      quads[i].v_c.texture_id = textures::Catalog[quads[i].texture_id].opengl_texture_id;
      quads[i].v_c.is_clicked = (float)quads[i].is_clicked;


      // create vertex struct - D
      quads[i].v_d.vertex_id = quads[i].d;
      quads[i].v_d.tile_id = quads[i].id;
      quads[i].v_d.frame_id = quads[i].frame_id;
      quads[i].v_d.x_pos = (float)quads[i].x + (float)TILE_DIM  - (float)VERTEX_OFFSET;
      quads[i].v_d.y_pos = (float)quads[i].y + (float)TILE_DIM - (float)VERTEX_OFFSET;
      quads[i].v_d.z_pos = 0.0f;
      quads[i].v_d.r_col = 1.0f;
      quads[i].v_d.g_col = 1.0f;
      quads[i].v_d.b_col = 1.0f;
      quads[i].v_d.a_col = 1.0f;
      quads[i].v_d.tex_coord_x = norm_x_end;
      quads[i].v_d.tex_coord_y = 1.0f;
      quads[i].v_d.texture_id = textures::Catalog[quads[i].texture_id].opengl_texture_id;
      quads[i].v_d.is_clicked = (float)quads[i].is_clicked;

      // create vindices 
      quads[i].i_left.a = quads[i].a;
      quads[i].i_left.b = quads[i].b;
      quads[i].i_left.c = quads[i].c;

      quads[i].i_right.a = quads[i].b;
      quads[i].i_right.b = quads[i].c;
      quads[i].i_right.c = quads[i].d;
    }

    if(LOGGING==-1)
    {
      print_out_quads(quads);
    }

    return quads;
  }


  // szczerze to mam wrazenie ze to kiepski pomysl
  std::vector<Quad> assign_vertices_no_texture(std::vector<Quad> quads)
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
      quads[i].v_a.frame_id = 0;
      quads[i].v_a.x_pos = (float)quads[i].x;
      quads[i].v_a.y_pos = (float)quads[i].y;
      quads[i].v_a.z_pos = 0.0f;
      quads[i].v_a.r_col = quads[i].r_col;
      quads[i].v_a.g_col = quads[i].g_col;
      quads[i].v_a.b_col = quads[i].b_col;
      quads[i].v_a.a_col = quads[i].a_col;
      quads[i].v_a.tex_coord_x = 0.0f;
      quads[i].v_a.tex_coord_y = 0.0f;
      quads[i].v_a.texture_id = 0;
      quads[i].v_a.is_clicked = (float)quads[i].is_clicked;

      // create vertex struct - B
      quads[i].v_b.vertex_id = quads[i].b;
      quads[i].v_b.tile_id = quads[i].id;
      quads[i].v_b.frame_id = 0;
      quads[i].v_b.x_pos = (float)quads[i].x + (float)quads[i].w;
      quads[i].v_b.y_pos = (float)quads[i].y;
      quads[i].v_b.z_pos = 0.0f;
      quads[i].v_b.r_col = quads[i].r_col;
      quads[i].v_b.g_col = quads[i].g_col;
      quads[i].v_b.b_col = quads[i].b_col;
      quads[i].v_b.a_col = quads[i].a_col;
      quads[i].v_b.tex_coord_x = 0.0f;
      quads[i].v_b.tex_coord_y = 0.0f;
      quads[i].v_b.texture_id = 0;
      quads[i].v_b.is_clicked = (float)quads[i].is_clicked;

      // create vertex struct - C
      quads[i].v_c.vertex_id = quads[i].c;
      quads[i].v_c.tile_id = quads[i].id;
      quads[i].v_c.frame_id = 0;
      quads[i].v_c.x_pos = (float)quads[i].x;
      quads[i].v_c.y_pos = (float)quads[i].y  + (float)quads[i].h;
      quads[i].v_c.z_pos = 0.0f;
      quads[i].v_c.r_col = quads[i].r_col;
      quads[i].v_c.g_col = quads[i].g_col;
      quads[i].v_c.b_col = quads[i].b_col;
      quads[i].v_c.a_col = quads[i].a_col;
      quads[i].v_c.tex_coord_x = 0.0f;
      quads[i].v_c.tex_coord_y = 0.0f;
      quads[i].v_c.texture_id = 0;
      quads[i].v_c.is_clicked = (float)quads[i].is_clicked;


      // create vertex struct - D
      quads[i].v_d.vertex_id = quads[i].d;
      quads[i].v_d.tile_id = quads[i].id;
      quads[i].v_d.frame_id = 0;
      quads[i].v_d.x_pos = (float)quads[i].x + (float)quads[i].w;
      quads[i].v_d.y_pos = (float)quads[i].y + (float)quads[i].h;
      quads[i].v_d.z_pos = 0.0f;
      quads[i].v_d.r_col = quads[i].r_col;
      quads[i].v_d.g_col = quads[i].g_col;
      quads[i].v_d.b_col = quads[i].b_col;
      quads[i].v_d.a_col = quads[i].a_col;
      quads[i].v_d.tex_coord_x = 0.0f;
      quads[i].v_d.tex_coord_y = 0.0f;
      quads[i].v_d.texture_id = 0;
      quads[i].v_d.is_clicked = (float)quads[i].is_clicked;

      // create vindices 
      quads[i].i_left.a = quads[i].a;
      quads[i].i_left.b = quads[i].b;
      quads[i].i_left.c = quads[i].c;

      quads[i].i_right.a = quads[i].b;
      quads[i].i_right.b = quads[i].c;
      quads[i].i_right.c = quads[i].d;
    }

    if(LOGGING==-1)
    {
      print_out_quads(quads);
    }

    return quads;
  }


}


#endif

