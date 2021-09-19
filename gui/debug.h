#ifndef DEBUG_H
#define DEBUG_H

// rendering debug quads
namespace debug
{
    // renders very thin quad to show the line
    void render_line(float x1, float y1, float x2, float y2)
    {
        struct quads::Quad quad;
        quad.id = quads::gen_quad_id();

        quad.x = x1;
        quad.y = y1;
        quad.w = gui::Catalog[gui_id].w;    
        quad.h = gui::Catalog[gui_id].h;
        quad.s_x = gui::Catalog[gui_id].x;
        quad.s_y = gui::Catalog[gui_id].y;
        quad.s_w = gui::Catalog[gui_id].w;    
        quad.s_h = gui::Catalog[gui_id].h;

            quad.r_col = gui::Catalog[gui_id].r_col;
            quad.g_col = gui::Catalog[gui_id].g_col;
            quad.b_col = gui::Catalog[gui_id].b_col;
            quad.a_col = gui::Catalog[gui_id].a_col;
            quad.is_clicked = 0.0f;
            quad.type_id = QUAD_TYPE_GUI;
            quad.is_static = 1.0f;
            gui::GuiQuads.push_back(quad);

            // insert name and quad id
            CurrentGuiWindows.insert(std::pair<int, int>(gui_id, quad.id));
        }

        for(int i = 0; i < gui::GuiQuads.size(); i++)
        { 
            // add vertex ids
            gui::GuiQuads[i].a = quads::gen_vertex_id();
            gui::GuiQuads[i].b = quads::gen_vertex_id();
            gui::GuiQuads[i].c = quads::gen_vertex_id();
            gui::GuiQuads[i].d = quads::gen_vertex_id();
        
            // create vertex struct - A
            gui::GuiQuads[i].v_a.vertex_id = gui::GuiQuads[i].a;
            gui::GuiQuads[i].v_a.tile_id = gui::GuiQuads[i].id;
            gui::GuiQuads[i].v_a.frame_id = 0;
            gui::GuiQuads[i].v_a.x_pos = (float)gui::GuiQuads[i].x;
            gui::GuiQuads[i].v_a.y_pos = (float)gui::GuiQuads[i].y;
            gui::GuiQuads[i].v_a.z_pos = 0.0f;
            gui::GuiQuads[i].v_a.r_col = gui::GuiQuads[i].r_col;
            gui::GuiQuads[i].v_a.g_col = gui::GuiQuads[i].g_col;
            gui::GuiQuads[i].v_a.b_col = gui::GuiQuads[i].b_col;
            gui::GuiQuads[i].v_a.a_col = gui::GuiQuads[i].a_col;
            gui::GuiQuads[i].v_a.tex_coord_x = 0.0f;
            gui::GuiQuads[i].v_a.tex_coord_y = 0.0f;
            gui::GuiQuads[i].v_a.texture_id = 0;
            gui::GuiQuads[i].v_a.is_clicked = (float)gui::GuiQuads[i].is_clicked;
            gui::GuiQuads[i].v_a.type_id = (float)gui::GuiQuads[i].type_id;
            gui::GuiQuads[i].v_a.is_static = (float)gui::GuiQuads[i].is_static;

            // create vertex struct - B
            gui::GuiQuads[i].v_b.vertex_id = gui::GuiQuads[i].b;
            gui::GuiQuads[i].v_b.tile_id = gui::GuiQuads[i].id;
            gui::GuiQuads[i].v_b.frame_id = 0;
            gui::GuiQuads[i].v_b.x_pos = (float)gui::GuiQuads[i].x + (float)gui::GuiQuads[i].w;
            gui::GuiQuads[i].v_b.y_pos = (float)gui::GuiQuads[i].y;
            gui::GuiQuads[i].v_b.z_pos = 0.0f;
            gui::GuiQuads[i].v_b.r_col = gui::GuiQuads[i].r_col;
            gui::GuiQuads[i].v_b.g_col = gui::GuiQuads[i].g_col;
            gui::GuiQuads[i].v_b.b_col = gui::GuiQuads[i].b_col;
            gui::GuiQuads[i].v_b.a_col = gui::GuiQuads[i].a_col;
            gui::GuiQuads[i].v_b.tex_coord_x = 0.0f;
            gui::GuiQuads[i].v_b.tex_coord_y = 0.0f;
            gui::GuiQuads[i].v_b.texture_id = 0;
            gui::GuiQuads[i].v_b.is_clicked = (float)gui::GuiQuads[i].is_clicked;
            gui::GuiQuads[i].v_b.type_id = (float)gui::GuiQuads[i].type_id;
            gui::GuiQuads[i].v_b.is_static = (float)gui::GuiQuads[i].is_static;

            // create vertex struct - C
            gui::GuiQuads[i].v_c.vertex_id = gui::GuiQuads[i].c;
            gui::GuiQuads[i].v_c.tile_id = gui::GuiQuads[i].id;
            gui::GuiQuads[i].v_c.frame_id = 0;
            gui::GuiQuads[i].v_c.x_pos = (float)gui::GuiQuads[i].x;
            gui::GuiQuads[i].v_c.y_pos = (float)gui::GuiQuads[i].y  + (float)gui::GuiQuads[i].h;
            gui::GuiQuads[i].v_c.z_pos = 0.0f;
            gui::GuiQuads[i].v_c.r_col = gui::GuiQuads[i].r_col;
            gui::GuiQuads[i].v_c.g_col = gui::GuiQuads[i].g_col;
            gui::GuiQuads[i].v_c.b_col = gui::GuiQuads[i].b_col;
            gui::GuiQuads[i].v_c.a_col = gui::GuiQuads[i].a_col;
            gui::GuiQuads[i].v_c.tex_coord_x = 0.0f;
            gui::GuiQuads[i].v_c.tex_coord_y = 0.0f;
            gui::GuiQuads[i].v_c.texture_id = 0;
            gui::GuiQuads[i].v_c.is_clicked = (float)gui::GuiQuads[i].is_clicked;
            gui::GuiQuads[i].v_c.type_id = (float)gui::GuiQuads[i].type_id;
            gui::GuiQuads[i].v_c.is_static = (float)gui::GuiQuads[i].is_static;


            // create vertex struct - D
            gui::GuiQuads[i].v_d.vertex_id = gui::GuiQuads[i].d;
            gui::GuiQuads[i].v_d.tile_id = gui::GuiQuads[i].id;
            gui::GuiQuads[i].v_d.frame_id = 0;
            gui::GuiQuads[i].v_d.x_pos = (float)gui::GuiQuads[i].x + (float)gui::GuiQuads[i].w;
            gui::GuiQuads[i].v_d.y_pos = (float)gui::GuiQuads[i].y + (float)gui::GuiQuads[i].h;
            gui::GuiQuads[i].v_d.z_pos = 0.0f;
            gui::GuiQuads[i].v_d.r_col = gui::GuiQuads[i].r_col;
            gui::GuiQuads[i].v_d.g_col = gui::GuiQuads[i].g_col;
            gui::GuiQuads[i].v_d.b_col = gui::GuiQuads[i].b_col;
            gui::GuiQuads[i].v_d.a_col = gui::GuiQuads[i].a_col;
            gui::GuiQuads[i].v_d.tex_coord_x = 0.0f;
            gui::GuiQuads[i].v_d.tex_coord_y = 0.0f;
            gui::GuiQuads[i].v_d.texture_id = 0;
            gui::GuiQuads[i].v_d.is_clicked = (float)gui::GuiQuads[i].is_clicked;
            gui::GuiQuads[i].v_d.type_id = (float)gui::GuiQuads[i].type_id;
            gui::GuiQuads[i].v_d.is_static = (float)gui::GuiQuads[i].is_static;

            // create vindices 
            gui::GuiQuads[i].i_left.a = gui::GuiQuads[i].a;
            gui::GuiQuads[i].i_left.b = gui::GuiQuads[i].b;
            gui::GuiQuads[i].i_left.c = gui::GuiQuads[i].c;

            gui::GuiQuads[i].i_right.a = gui::GuiQuads[i].b;
            gui::GuiQuads[i].i_right.b = gui::GuiQuads[i].c;
            gui::GuiQuads[i].i_right.c = gui::GuiQuads[i].d;
        }

    }




}


#endif