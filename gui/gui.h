#ifndef GUI_H
#define GUI_H

namespace gui
{
    // struct with gui information from ./gui/data
    struct GuiWindow
    {
      int id;
      int x;
      int y;
      int w;
      int h;
      float r_col;
      float g_col;
      float b_col;
      float a_col;
      std::string name;
      JS_OBJ(id, x, y, w, h, r_col, g_col, b_col, a_col, name);
    };

    std::map<int, GuiWindow> Catalog = {};

    // read data for selected gui file
    void read_gui_data(std::string name)
    {
        GuiWindow gw;
        std::string data_path = "./gui/data/"+name+".json";
        logger::log(LOG_INFO, LOG_EVENT_READ_DATA, "gui::read_gui_data", __FILE__, __LINE__, "Reading GUI Window data from " + data_path);
        std::string json_data = utils::read_text_file(data_path);
        JS::ParseContext context(json_data);
        context.parseTo(gw);
        gui::Catalog.insert({gw.id, gw});
    }

    // Read data for all files in ./gui
    void init()
    {
        logger::log(LOG_INFO, LOG_EVENT_INIT_MODULE, "gui::init", __FILE__, __LINE__, "Initializing gui");
        std::vector<std::string> gui_list = utils::list_files("gui/data/");

        for(int g=0; g < gui_list.size(); g++)
        {
            read_gui_data(gui_list[g]);
        }
        logger::log(LOG_INFO, LOG_EVENT_INIT_MODULE, "gui::init", __FILE__, __LINE__, "GUI initialized");
    }

    // render selected GUIs (defined by int)
    void render(std::vector<int> guis)
    {
        std::vector<quads::Quad> gui_quads;
        for(int g = 0; g < guis.size(); g++)
        {
            struct quads::Quad quad;
            int gui_id = guis[g];
            quad.id = quads::gen_quad_id();

            quad.x = gui::Catalog[gui_id].x;
            quad.y = gui::Catalog[gui_id].y;
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
            gui_quads.push_back(quad);

            // insert name and quad id
            CurrentGuiWindows.insert(std::pair<int, std::string>(quad.id, gui::Catalog[gui_id].name));
        }

        for(int i = 0; i < gui_quads.size(); i++)
        { 
            // add vertex ids
            gui_quads[i].a = quads::gen_vertex_id();
            gui_quads[i].b = quads::gen_vertex_id();
            gui_quads[i].c = quads::gen_vertex_id();
            gui_quads[i].d = quads::gen_vertex_id();
        
            // create vertex struct - A
            gui_quads[i].v_a.vertex_id = gui_quads[i].a;
            gui_quads[i].v_a.tile_id = gui_quads[i].id;
            gui_quads[i].v_a.frame_id = 0;
            gui_quads[i].v_a.x_pos = (float)gui_quads[i].x;
            gui_quads[i].v_a.y_pos = (float)gui_quads[i].y;
            gui_quads[i].v_a.z_pos = 0.0f;
            gui_quads[i].v_a.r_col = gui_quads[i].r_col;
            gui_quads[i].v_a.g_col = gui_quads[i].g_col;
            gui_quads[i].v_a.b_col = gui_quads[i].b_col;
            gui_quads[i].v_a.a_col = gui_quads[i].a_col;
            gui_quads[i].v_a.tex_coord_x = 0.0f;
            gui_quads[i].v_a.tex_coord_y = 0.0f;
            gui_quads[i].v_a.texture_id = 0;
            gui_quads[i].v_a.is_clicked = (float)gui_quads[i].is_clicked;
            gui_quads[i].v_a.type_id = (float)gui_quads[i].type_id;
            gui_quads[i].v_a.is_static = (float)gui_quads[i].is_static;

            // create vertex struct - B
            gui_quads[i].v_b.vertex_id = gui_quads[i].b;
            gui_quads[i].v_b.tile_id = gui_quads[i].id;
            gui_quads[i].v_b.frame_id = 0;
            gui_quads[i].v_b.x_pos = (float)gui_quads[i].x + (float)gui_quads[i].w;
            gui_quads[i].v_b.y_pos = (float)gui_quads[i].y;
            gui_quads[i].v_b.z_pos = 0.0f;
            gui_quads[i].v_b.r_col = gui_quads[i].r_col;
            gui_quads[i].v_b.g_col = gui_quads[i].g_col;
            gui_quads[i].v_b.b_col = gui_quads[i].b_col;
            gui_quads[i].v_b.a_col = gui_quads[i].a_col;
            gui_quads[i].v_b.tex_coord_x = 0.0f;
            gui_quads[i].v_b.tex_coord_y = 0.0f;
            gui_quads[i].v_b.texture_id = 0;
            gui_quads[i].v_b.is_clicked = (float)gui_quads[i].is_clicked;
            gui_quads[i].v_b.type_id = (float)gui_quads[i].type_id;
            gui_quads[i].v_b.is_static = (float)gui_quads[i].is_static;

            // create vertex struct - C
            gui_quads[i].v_c.vertex_id = gui_quads[i].c;
            gui_quads[i].v_c.tile_id = gui_quads[i].id;
            gui_quads[i].v_c.frame_id = 0;
            gui_quads[i].v_c.x_pos = (float)gui_quads[i].x;
            gui_quads[i].v_c.y_pos = (float)gui_quads[i].y  + (float)gui_quads[i].h;
            gui_quads[i].v_c.z_pos = 0.0f;
            gui_quads[i].v_c.r_col = gui_quads[i].r_col;
            gui_quads[i].v_c.g_col = gui_quads[i].g_col;
            gui_quads[i].v_c.b_col = gui_quads[i].b_col;
            gui_quads[i].v_c.a_col = gui_quads[i].a_col;
            gui_quads[i].v_c.tex_coord_x = 0.0f;
            gui_quads[i].v_c.tex_coord_y = 0.0f;
            gui_quads[i].v_c.texture_id = 0;
            gui_quads[i].v_c.is_clicked = (float)gui_quads[i].is_clicked;
            gui_quads[i].v_c.type_id = (float)gui_quads[i].type_id;
            gui_quads[i].v_c.is_static = (float)gui_quads[i].is_static;


            // create vertex struct - D
            gui_quads[i].v_d.vertex_id = gui_quads[i].d;
            gui_quads[i].v_d.tile_id = gui_quads[i].id;
            gui_quads[i].v_d.frame_id = 0;
            gui_quads[i].v_d.x_pos = (float)gui_quads[i].x + (float)gui_quads[i].w;
            gui_quads[i].v_d.y_pos = (float)gui_quads[i].y + (float)gui_quads[i].h;
            gui_quads[i].v_d.z_pos = 0.0f;
            gui_quads[i].v_d.r_col = gui_quads[i].r_col;
            gui_quads[i].v_d.g_col = gui_quads[i].g_col;
            gui_quads[i].v_d.b_col = gui_quads[i].b_col;
            gui_quads[i].v_d.a_col = gui_quads[i].a_col;
            gui_quads[i].v_d.tex_coord_x = 0.0f;
            gui_quads[i].v_d.tex_coord_y = 0.0f;
            gui_quads[i].v_d.texture_id = 0;
            gui_quads[i].v_d.is_clicked = (float)gui_quads[i].is_clicked;
            gui_quads[i].v_d.type_id = (float)gui_quads[i].type_id;
            gui_quads[i].v_d.is_static = (float)gui_quads[i].is_static;

            // create vindices 
            gui_quads[i].i_left.a = gui_quads[i].a;
            gui_quads[i].i_left.b = gui_quads[i].b;
            gui_quads[i].i_left.c = gui_quads[i].c;

            gui_quads[i].i_right.a = gui_quads[i].b;
            gui_quads[i].i_right.b = gui_quads[i].c;
            gui_quads[i].i_right.c = gui_quads[i].d;
        }

        gui::GuiQuads = gui_quads;
    }

    void drop()
    {
        logger::log(LOG_INFO, LOG_EVENT_DROP_DATA, "gui::drop", __FILE__, __LINE__, "Dropping GUI");
        CurrentGuiWindows.clear();
    }

}



#endif


