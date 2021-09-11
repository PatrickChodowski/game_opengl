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

    void render()
    {


    }

    void drop()
    {

    }

}



#endif


