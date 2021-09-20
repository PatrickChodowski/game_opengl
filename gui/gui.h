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

        //std::cout << "GUI SIZE : " << gui::GuiQuads.size() << std::endl;
    }

    void drop()
    {
        logger::log(LOG_INFO, LOG_EVENT_DROP_DATA, "gui::drop", __FILE__, __LINE__, "Dropping GUI");
        CurrentGuiWindows.clear();
        for (int q = 0; q < gui::GuiQuads.size(); q++)
        {
            quads::delete_quad_id(gui::GuiQuads[q].id);
            quads::delete_vertex_id(gui::GuiQuads[q].a);
            quads::delete_vertex_id(gui::GuiQuads[q].b);
            quads::delete_vertex_id(gui::GuiQuads[q].c);
            quads::delete_vertex_id(gui::GuiQuads[q].d);
        }
        gui::GuiQuads.clear();
    }

    // add label to gui by gui_id, x, y, text, scale,  is_static
    void add_gui_label(int gui_id, float x, float y, std::string text, float scale,  bool is_static)
    {
        // get quad id from current gui windows
        for (auto const& a : gui::CurrentGuiWindows)
        {
            if(a.first == gui_id)
            {
                quads::add_label(a.second, x, y, text, scale, is_static);
                break;
            }
        }
    }

    // Rendering all information in debug window
    void render_debug_window()
    {
        float _scale = 0.4;
        // gui::add_gui_label(GUI_DEBUG, 610, 25, "FPS:_" + std::to_string(FPS), _scale, 1.0);
        // gui::add_gui_label(GUI_DEBUG, 610, 50, "quad_count:_" + std::to_string(quads::COUNT_QUADS), _scale, 1.0);
        // gui::add_gui_label(GUI_DEBUG, 610, 75, "_VBO_usage:_" + std::to_string(buffer::VBO_buffer_usage).substr(0, 5), _scale, 1.0);
        // gui::add_gui_label(GUI_DEBUG, 610, 100, "_EBO_usage:_" + std::to_string(buffer::EBO_buffer_usage).substr(0, 5), _scale, 1.0);
        // gui::add_gui_label(GUI_DEBUG, 610, 125, "camera_zoom:_" + std::to_string(camera::zoom).substr(0, 3), _scale, 1.0);
        // gui::add_gui_label(GUI_DEBUG, 610, 150, "camera_x:_" + std::to_string(camera::x), _scale,  1.0);
        // gui::add_gui_label(GUI_DEBUG, 610, 175, "camera_y:_" + std::to_string(camera::y), _scale, 1.0);
        // gui::add_gui_label(GUI_DEBUG, 610, 200, "TC_size:_" + std::to_string(travel::TravelControl.size()), _scale, 1.0);
        if(travel::TravelControl.size() > 0)
        {
            int count = 0;
            for (auto const& t : travel::TravelControl)
            {   
                
                std::string tp_info0 = "TP_ID:_" + std::to_string(t.first); 
                // std::string tp_info1 = "___Current_pos:_" + std::to_string(t.second.current_x).substr(0, 4) + 
                // "," + std::to_string(t.second.current_y).substr(0, 4);

                std::string tp_info2 = "___Target_pos:_" + std::to_string(t.second.target_x).substr(0, 4) + 
                "," + std::to_string(t.second.target_y).substr(0, 4);

                std::string tp_info3 = "___Current_node:_" + std::to_string(t.second.current_node);
                std::string tp_info4 = "___Next_node:_" + std::to_string(t.second.next_node);
                std::string tp_info5 = "___Target_node:_" + std::to_string(t.second.target_node);
                std::string tp_info6 = "___Next_gate:_" + std::to_string(t.second.next_gate);

                std::string tp_info7 = "___Full_path:_";
                for(int f=0; f<t.second.full_path.size(); f++)
                {
                    tp_info7 += std::to_string(t.second.full_path[f]);
                    tp_info7 += ",";
                }

                //t.second.next_gate
                //std::string tp_info8 = "___cpoint:_" + std::to_string(t.second.cpoint_x).substr(0, 4) + "," + std::to_string(t.second.cpoint_y).substr(0, 4);


                count++;
                gui::add_gui_label(GUI_DEBUG, 610, 200+count*25, tp_info0, _scale, 1.0);
                count++;
                //gui::add_gui_label(GUI_DEBUG, 610, 200+count*25, tp_info1, _scale, 1.0);
                count++;
                gui::add_gui_label(GUI_DEBUG, 610, 200+count*25, tp_info2, _scale, 1.0);
                count++;
                gui::add_gui_label(GUI_DEBUG, 610, 200+count*25, tp_info3, _scale, 1.0);
                count++;
                gui::add_gui_label(GUI_DEBUG, 610, 200+count*25, tp_info4, _scale, 1.0);
                count++;
                gui::add_gui_label(GUI_DEBUG, 610, 200+count*25, tp_info5, _scale, 1.0);
                count++;
                gui::add_gui_label(GUI_DEBUG, 610, 200+count*25, tp_info6, _scale, 1.0);
                count++;
                gui::add_gui_label(GUI_DEBUG, 610, 200+count*25, tp_info7, _scale, 1.0);
                //count++;
                //gui::add_gui_label(GUI_DEBUG, 610, 200+count*25, tp_info8, _scale, 1.0);
          
            }
        }
    }


}



#endif


