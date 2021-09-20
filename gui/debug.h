#ifndef DEBUG_H
#define DEBUG_H

// rendering debug quads
namespace debug
{
    // renders transparent colorful quad in given point
    void render_point(float x, float y)
    {
        // std::cout << "rendering point: " << x << "," << y << std::endl;
        struct quads::Quad quad;
        quad.id = quads::gen_quad_id();
        quad.x = x;
        quad.y = y;
        quad.w = 10;   
        quad.h = 10;
        quad.s_x = camera::scale_x(x);
        quad.s_y = camera::scale_y(y);
        quad.s_w = 10;   
        quad.s_h = 10;
        quad.r_col = 1.0;
        quad.g_col = 0.0;
        quad.b_col = 0.0;
        quad.a_col = 0.5;
        quad.is_clicked = 0.0f;
        quad.type_id = QUAD_TYPE_DEBUG;
        quad.is_static = 0.0f;

        quad.a = quads::gen_vertex_id();
        quad.b = quads::gen_vertex_id();
        quad.c = quads::gen_vertex_id();
        quad.d = quads::gen_vertex_id();

        // create vertex struct - A
        quad.v_a.vertex_id = quad.a;
        quad.v_a.tile_id = quad.id;
        quad.v_a.frame_id = 0;
        quad.v_a.x_pos = (float)quad.x;
        quad.v_a.y_pos = (float)quad.y;
        quad.v_a.z_pos = 0.0f;
        quad.v_a.r_col = quad.r_col;
        quad.v_a.g_col = quad.g_col;
        quad.v_a.b_col = quad.b_col;
        quad.v_a.a_col = quad.a_col;
        quad.v_a.tex_coord_x = 0.0f;
        quad.v_a.tex_coord_y = 0.0f;
        quad.v_a.texture_id = 0;
        quad.v_a.is_clicked = (float)quad.is_clicked;
        quad.v_a.type_id = (float)quad.type_id;
        quad.v_a.is_static = (float)quad.is_static;

        // quad struct - B
        quad.v_b.vertex_id = quad.b;
        quad.v_b.tile_id = quad.id;
        quad.v_b.frame_id = 0;
        quad.v_b.x_pos = (float)quad.x + (float)quad.w;
        quad.v_b.y_pos = (float)quad.y;
        quad.v_b.z_pos = 0.0f;
        quad.v_b.r_col = quad.r_col;
        quad.v_b.g_col = quad.g_col;
        quad.v_b.b_col = quad.b_col;
        quad.v_b.a_col = quad.a_col;
        quad.v_b.tex_coord_x = 0.0f;
        quad.v_b.tex_coord_y = 0.0f;
        quad.v_b.texture_id = 0;
        quad.v_b.is_clicked = (float)quad.is_clicked;
        quad.v_b.type_id = (float)quad.type_id;
        quad.v_b.is_static = (float)quad.is_static;

        // create vertex struct - C
        quad.v_c.vertex_id = quad.c;
        quad.v_c.tile_id = quad.id;
        quad.v_c.frame_id = 0;
        quad.v_c.x_pos = (float)quad.x;
        quad.v_c.y_pos = (float)quad.y  + (float)quad.h;
        quad.v_c.z_pos = 0.0f;
        quad.v_c.r_col = quad.r_col;
        quad.v_c.g_col = quad.g_col;
        quad.v_c.b_col = quad.b_col;
        quad.v_c.a_col = quad.a_col;
        quad.v_c.tex_coord_x = 0.0f;
        quad.v_c.tex_coord_y = 0.0f;
        quad.v_c.texture_id = 0;
        quad.v_c.is_clicked = (float)quad.is_clicked;
        quad.v_c.type_id = (float)quad.type_id;
        quad.v_c.is_static = (float)quad.is_static;


        // create vertex struct - D
        quad.v_d.vertex_id = quad.d;
        quad.v_d.tile_id = quad.id;
        quad.v_d.frame_id = 0;
        quad.v_d.x_pos = (float)quad.x + (float)quad.w;
        quad.v_d.y_pos = (float)quad.y + (float)quad.h;
        quad.v_d.z_pos = 0.0f;
        quad.v_d.r_col = quad.r_col;
        quad.v_d.g_col = quad.g_col;
        quad.v_d.b_col = quad.b_col;
        quad.v_d.a_col = quad.a_col;
        quad.v_d.tex_coord_x = 0.0f;
        quad.v_d.tex_coord_y = 0.0f;
        quad.v_d.texture_id = 0;
        quad.v_d.is_clicked = (float)quad.is_clicked;
        quad.v_d.type_id = (float)quad.type_id;
        quad.v_d.is_static = (float)quad.is_static;

        // create vindices 
        quad.i_left.a = quad.a;
        quad.i_left.b = quad.b;
        quad.i_left.c = quad.c;
        quad.i_right.a = quad.b;
        quad.i_right.b = quad.c;
        quad.i_right.c = quad.d;
        debug::DebugQuads.push_back(quad);

    }




    // renders very thin quad to show the line
    void render_line(float x1, float y1, float x2, float y2)
    {
        struct quads::Quad quad;
        quad.id = quads::gen_quad_id();

        quad.x = x1;
        quad.y = y1;
        quad.w = x1; //gui::Catalog[gui_id].w;    
        quad.h = y1; //gui::Catalog[gui_id].h;
        quad.s_x = x1;
        quad.s_y = y1;
        quad.s_w = x1;//gui::Catalog[gui_id].w;    
        quad.s_h = y1;//gui::Catalog[gui_id].h;

        quad.r_col = 1.0;
        quad.g_col = 0.0;
        quad.b_col = 0.0;
        quad.a_col = 0.5;
        quad.is_clicked = 0.0f;
        quad.type_id = QUAD_TYPE_DEBUG;
        quad.is_static = 1.0f;
        
        quad.a = quads::gen_vertex_id();
        quad.b = quads::gen_vertex_id();
        quad.c = quads::gen_vertex_id();
        quad.d = quads::gen_vertex_id();

        // create vertex struct - A
        quad.v_a.vertex_id = quad.a;
        quad.v_a.tile_id = quad.id;
        quad.v_a.frame_id = 0;
        quad.v_a.x_pos = (float)quad.x;
        quad.v_a.y_pos = (float)quad.y;
        quad.v_a.z_pos = 0.0f;
        quad.v_a.r_col = quad.r_col;
        quad.v_a.g_col = quad.g_col;
        quad.v_a.b_col = quad.b_col;
        quad.v_a.a_col = quad.a_col;
        quad.v_a.tex_coord_x = 0.0f;
        quad.v_a.tex_coord_y = 0.0f;
        quad.v_a.texture_id = 0;
        quad.v_a.is_clicked = (float)quad.is_clicked;
        quad.v_a.type_id = (float)quad.type_id;
        quad.v_a.is_static = (float)quad.is_static;

        // quad struct - B
        quad.v_b.vertex_id = quad.b;
        quad.v_b.tile_id = quad.id;
        quad.v_b.frame_id = 0;
        quad.v_b.x_pos = (float)quad.x + (float)quad.w;
        quad.v_b.y_pos = (float)quad.y;
        quad.v_b.z_pos = 0.0f;
        quad.v_b.r_col = quad.r_col;
        quad.v_b.g_col = quad.g_col;
        quad.v_b.b_col = quad.b_col;
        quad.v_b.a_col = quad.a_col;
        quad.v_b.tex_coord_x = 0.0f;
        quad.v_b.tex_coord_y = 0.0f;
        quad.v_b.texture_id = 0;
        quad.v_b.is_clicked = (float)quad.is_clicked;
        quad.v_b.type_id = (float)quad.type_id;
        quad.v_b.is_static = (float)quad.is_static;

        // create vertex struct - C
        quad.v_c.vertex_id = quad.c;
        quad.v_c.tile_id = quad.id;
        quad.v_c.frame_id = 0;
        quad.v_c.x_pos = (float)quad.x;
        quad.v_c.y_pos = (float)quad.y  + (float)quad.h;
        quad.v_c.z_pos = 0.0f;
        quad.v_c.r_col = quad.r_col;
        quad.v_c.g_col = quad.g_col;
        quad.v_c.b_col = quad.b_col;
        quad.v_c.a_col = quad.a_col;
        quad.v_c.tex_coord_x = 0.0f;
        quad.v_c.tex_coord_y = 0.0f;
        quad.v_c.texture_id = 0;
        quad.v_c.is_clicked = (float)quad.is_clicked;
        quad.v_c.type_id = (float)quad.type_id;
        quad.v_c.is_static = (float)quad.is_static;


        // create vertex struct - D
        quad.v_d.vertex_id = quad.d;
        quad.v_d.tile_id = quad.id;
        quad.v_d.frame_id = 0;
        quad.v_d.x_pos = (float)quad.x + (float)quad.w;
        quad.v_d.y_pos = (float)quad.y + (float)quad.h;
        quad.v_d.z_pos = 0.0f;
        quad.v_d.r_col = quad.r_col;
        quad.v_d.g_col = quad.g_col;
        quad.v_d.b_col = quad.b_col;
        quad.v_d.a_col = quad.a_col;
        quad.v_d.tex_coord_x = 0.0f;
        quad.v_d.tex_coord_y = 0.0f;
        quad.v_d.texture_id = 0;
        quad.v_d.is_clicked = (float)quad.is_clicked;
        quad.v_d.type_id = (float)quad.type_id;
        quad.v_d.is_static = (float)quad.is_static;

        // create vindices 
        quad.i_left.a = quad.a;
        quad.i_left.b = quad.b;
        quad.i_left.c = quad.c;
        quad.i_right.a = quad.b;
        quad.i_right.b = quad.c;
        quad.i_right.c = quad.d;
        debug::DebugQuads.push_back(quad);
    }


    
  void drop()
  {
    for (int q = 0; q <debug::DebugQuads.size(); q++)
    {
      quads::delete_quad_id(debug::DebugQuads[q].id);
      quads::delete_vertex_id(debug::DebugQuads[q].a);
      quads::delete_vertex_id(debug::DebugQuads[q].b);
      quads::delete_vertex_id(debug::DebugQuads[q].c);
      quads::delete_vertex_id(debug::DebugQuads[q].d);
    }
    debug::DebugQuads.clear();
  } 


}


#endif