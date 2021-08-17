#ifndef ENTITY_H
#define ENTITY_H


// rendering entities logic

// entity type ids:
// hero:0, item:1, mob: 2

namespace ent
{
  quads::Quad render_entity(
                     int entity_type_id,
                     bool alive,
                     int texture_id,
                     int frame_id,
                     float x, 
                     float y,
                     int h, 
                     int w, 
                     float is_static,
                     textures::TextureData FontTD,
                     bool solid,
                     bool coll
                     )
  {
    float norm_x_start = (float)textures::Catalog[texture_id].frames[frame_id].x/
    (float)textures::Catalog[texture_id].width;

    float norm_x_end =  (float)(textures::Catalog[texture_id].frames[frame_id].x + 
    textures::Catalog[texture_id].frames[frame_id].w)/
    (float)textures::Catalog[texture_id].width;


    // render single entity
    struct quads::Quad quad;
    quad.id = quads::gen_quad_id();
    quad.x = x;
    quad.y = y;
    quad.w = w;
    quad.h = h;
    quad.frame_id = frame_id;
    quad.texture_id = texture_id;
    quad.solid = 0.0f;
    quad.is_clicked = 0.0f;
    quad.r_col = 0.5f;
    quad.g_col = 0.5f;
    quad.b_col = 0.5f;
    quad.a_col = 1.0f;
    quad.type_id = QUAD_TYPE_ENTITY;
    quad.is_static = is_static;
    quad.entity_type_id = entity_type_id;
    quad.alive = alive;
    quad.solid = solid;
    quad.coll = coll;

    quad.a = quads::gen_vertex_id();
    quad.b = quads::gen_vertex_id();
    quad.c = quads::gen_vertex_id();
    quad.d = quads::gen_vertex_id();

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
    quad.v_a.tex_coord_x = norm_x_start;
    quad.v_a.tex_coord_y = 0.0f;
    quad.v_a.texture_id = quad.texture_id;
    quad.v_a.is_clicked = (float)quad.is_clicked;
    quad.v_a.type_id = (double)quad.type_id;
    quad.v_a.is_static = (float)quad.is_static;

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
    quad.v_b.tex_coord_x = norm_x_end;
    quad.v_b.tex_coord_y = 0.0f;
    quad.v_b.texture_id = quad.texture_id;
    quad.v_b.is_clicked = (float)quad.is_clicked;
    quad.v_b.type_id = (double)quad.type_id;
    quad.v_b.is_static = (float)quad.is_static;

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
    quad.v_c.tex_coord_x = norm_x_start;
    quad.v_c.tex_coord_y = 1.0f;
    quad.v_c.texture_id = quad.texture_id;
    quad.v_c.is_clicked = (float)quad.is_clicked;
    quad.v_c.type_id = (double)quad.type_id;
    quad.v_c.is_static = (float)quad.is_static;

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
    quad.v_d.tex_coord_x = norm_x_end;
    quad.v_d.tex_coord_y = 1.0f;
    quad.v_d.texture_id = quad.texture_id;
    quad.v_d.is_clicked = (float)quad.is_clicked;
    quad.v_d.type_id = (double)quad.type_id;
    quad.v_d.is_static = (float)quad.is_static;

    // create vindices 
    quad.i_left.a = quad.a;
    quad.i_left.b = quad.b;
    quad.i_left.c = quad.c;
    quad.i_right.a = quad.b;
    quad.i_right.b = quad.c;
    quad.i_right.c = quad.d;

    return quad;
  }



  void drop_entities()
  {
    for (int q = 0; q <ent::EntityQuads.size(); q++)
    {
      quads::delete_quad_id(ent::EntityQuads[q].id);
      quads::delete_vertex_id(ent::EntityQuads[q].a);
      quads::delete_vertex_id(ent::EntityQuads[q].b);
      quads::delete_vertex_id(ent::EntityQuads[q].c);
      quads::delete_vertex_id(ent::EntityQuads[q].d);
    }
    ent::EntityQuads.clear();
  }


}














#endif