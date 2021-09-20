#ifndef MENU_H
#define MENU_H

// define menu setup and its behaviour
// rendering buttons?

// slighlty different approach when its pause menu and main menu
namespace menu
{

// main menu
// should render one massive quad as background photo and then additional quads as buttons

// main menu
struct ButtonData
{
  int id;
  int x;
  int y;
  int w;
  int h;
  std::string label;
  std::string name;

  float r_col;
  float g_col;
  float b_col;
  float a_col;

  JS_OBJ(id, x, y, w, h, label, name, r_col, g_col, b_col, a_col);
};

std::map<int, ButtonData> Catalog = {};

// function for generating quads for buttons
std::vector<quads::Quad> load_button_quads(std::vector<int> button_list)
{
  std::vector<quads::Quad> button_quads;
  for(int b = 0; b < button_list.size(); b++)
  {
    struct quads::Quad quad;
    int button_id = button_list[b];
    quad.id = quads::gen_quad_id();
    quad.x = menu::Catalog[button_id].x;
    quad.y = menu::Catalog[button_id].y;
    quad.w = menu::Catalog[button_id].w;    
    quad.h = menu::Catalog[button_id].h;

    quad.s_x = menu::Catalog[button_id].x;
    quad.s_y = menu::Catalog[button_id].y;
    quad.s_w = menu::Catalog[button_id].w;    
    quad.s_h = menu::Catalog[button_id].h;

    quad.r_col = menu::Catalog[button_id].r_col;
    quad.g_col = menu::Catalog[button_id].g_col;
    quad.b_col = menu::Catalog[button_id].b_col;
    quad.a_col = menu::Catalog[button_id].a_col;
    quad.is_clicked = 0.0f;
    quad.type_id = QUAD_TYPE_MENU;
    quad.is_static = 1.0f;
    button_quads.push_back(quad);

    if(menu::Catalog[button_id].name != "new_game_name"){
      fonts::render_text(menu::Catalog[button_id].label.c_str(), 
                        (menu::Catalog[button_id].x + 20), 
                        (menu::Catalog[button_id].y + 50), 
                        textures::FontTD, 
                        0.7, 
                        0.5, 
                        0.5, 
                        0.5,
                        1.0); 
    } else {
      fonts::render_text(menu::NewGameName.c_str(), 
                        (menu::Catalog[button_id].x + 20), 
                        (menu::Catalog[button_id].y + 50), 
                        textures::FontTD, 
                        0.7, 
                        0.5, 
                        0.5, 
                        0.5,
                        1.0); 
    }
    // insert name and quad id
    CurrentMenuButtons.insert(std::pair<int, std::string>(quad.id, menu::Catalog[button_id].name));
  }

  for(int i = 0; i < button_quads.size(); i++)
  { 
    // add vertex ids
    button_quads[i].a = quads::gen_vertex_id();
    button_quads[i].b = quads::gen_vertex_id();
    button_quads[i].c = quads::gen_vertex_id();
    button_quads[i].d = quads::gen_vertex_id();

    // create vertex struct - A
    button_quads[i].v_a.vertex_id = button_quads[i].a;
    button_quads[i].v_a.tile_id = button_quads[i].id;
    button_quads[i].v_a.frame_id = 0;
    button_quads[i].v_a.x_pos = (float)button_quads[i].x;
    button_quads[i].v_a.y_pos = (float)button_quads[i].y;
    button_quads[i].v_a.z_pos = 0.0f;
    button_quads[i].v_a.r_col = button_quads[i].r_col;
    button_quads[i].v_a.g_col = button_quads[i].g_col;
    button_quads[i].v_a.b_col = button_quads[i].b_col;
    button_quads[i].v_a.a_col = button_quads[i].a_col;
    button_quads[i].v_a.tex_coord_x = 0.0f;
    button_quads[i].v_a.tex_coord_y = 0.0f;
    button_quads[i].v_a.texture_id = 0;
    button_quads[i].v_a.is_clicked = (float)button_quads[i].is_clicked;
    button_quads[i].v_a.type_id = (float)button_quads[i].type_id;
    button_quads[i].v_a.is_static = (float)button_quads[i].is_static;

    // create vertex struct - B
    button_quads[i].v_b.vertex_id = button_quads[i].b;
    button_quads[i].v_b.tile_id = button_quads[i].id;
    button_quads[i].v_b.frame_id = 0;
    button_quads[i].v_b.x_pos = (float)button_quads[i].x + (float)button_quads[i].w;
    button_quads[i].v_b.y_pos = (float)button_quads[i].y;
    button_quads[i].v_b.z_pos = 0.0f;
    button_quads[i].v_b.r_col = button_quads[i].r_col;
    button_quads[i].v_b.g_col = button_quads[i].g_col;
    button_quads[i].v_b.b_col = button_quads[i].b_col;
    button_quads[i].v_b.a_col = button_quads[i].a_col;
    button_quads[i].v_b.tex_coord_x = 0.0f;
    button_quads[i].v_b.tex_coord_y = 0.0f;
    button_quads[i].v_b.texture_id = 0;
    button_quads[i].v_b.is_clicked = (float)button_quads[i].is_clicked;
    button_quads[i].v_b.type_id = (float)button_quads[i].type_id;
    button_quads[i].v_b.is_static = (float)button_quads[i].is_static;

    // create vertex struct - C
    button_quads[i].v_c.vertex_id = button_quads[i].c;
    button_quads[i].v_c.tile_id = button_quads[i].id;
    button_quads[i].v_c.frame_id = 0;
    button_quads[i].v_c.x_pos = (float)button_quads[i].x;
    button_quads[i].v_c.y_pos = (float)button_quads[i].y  + (float)button_quads[i].h;
    button_quads[i].v_c.z_pos = 0.0f;
    button_quads[i].v_c.r_col = button_quads[i].r_col;
    button_quads[i].v_c.g_col = button_quads[i].g_col;
    button_quads[i].v_c.b_col = button_quads[i].b_col;
    button_quads[i].v_c.a_col = button_quads[i].a_col;
    button_quads[i].v_c.tex_coord_x = 0.0f;
    button_quads[i].v_c.tex_coord_y = 0.0f;
    button_quads[i].v_c.texture_id = 0;
    button_quads[i].v_c.is_clicked = (float)button_quads[i].is_clicked;
    button_quads[i].v_c.type_id = (float)button_quads[i].type_id;
    button_quads[i].v_c.is_static = (float)button_quads[i].is_static;


    // create vertex struct - D
    button_quads[i].v_d.vertex_id = button_quads[i].d;
    button_quads[i].v_d.tile_id = button_quads[i].id;
    button_quads[i].v_d.frame_id = 0;
    button_quads[i].v_d.x_pos = (float)button_quads[i].x + (float)button_quads[i].w;
    button_quads[i].v_d.y_pos = (float)button_quads[i].y + (float)button_quads[i].h;
    button_quads[i].v_d.z_pos = 0.0f;
    button_quads[i].v_d.r_col = button_quads[i].r_col;
    button_quads[i].v_d.g_col = button_quads[i].g_col;
    button_quads[i].v_d.b_col = button_quads[i].b_col;
    button_quads[i].v_d.a_col = button_quads[i].a_col;
    button_quads[i].v_d.tex_coord_x = 0.0f;
    button_quads[i].v_d.tex_coord_y = 0.0f;
    button_quads[i].v_d.texture_id = 0;
    button_quads[i].v_d.is_clicked = (float)button_quads[i].is_clicked;
    button_quads[i].v_d.type_id = (float)button_quads[i].type_id;
    button_quads[i].v_d.is_static = (float)button_quads[i].is_static;

    // create vindices 
    button_quads[i].i_left.a = button_quads[i].a;
    button_quads[i].i_left.b = button_quads[i].b;
    button_quads[i].i_left.c = button_quads[i].c;

    button_quads[i].i_right.a = button_quads[i].b;
    button_quads[i].i_right.b = button_quads[i].c;
    button_quads[i].i_right.c = button_quads[i].d;
  }

  return button_quads;
}


std::vector<quads::Quad> load_saves_buttons()
{
  std::vector<quads::Quad> button_quads;
  std::vector<std::string> saves = menu::list_saves();
  int button_id = 6;

  for(int b = 0; b < saves.size(); b++)
  {
    struct quads::Quad quad;
    quad.id = quads::gen_quad_id();
    quad.x = menu::Catalog[button_id].x;
    quad.y = menu::Catalog[button_id].y + (b*(menu::Catalog[button_id].h+5));
    quad.w = menu::Catalog[button_id].w;    
    quad.h = menu::Catalog[button_id].h;

    quad.s_x = menu::Catalog[button_id].x;
    quad.s_y = menu::Catalog[button_id].y  + (b*(menu::Catalog[button_id].h+5));
    quad.s_w = menu::Catalog[button_id].w;    
    quad.s_h = menu::Catalog[button_id].h;

    quad.r_col = menu::Catalog[button_id].r_col;
    quad.g_col = menu::Catalog[button_id].g_col;
    quad.b_col = menu::Catalog[button_id].b_col;
    quad.a_col = menu::Catalog[button_id].a_col;
    quad.is_clicked = 0.0f;
    quad.type_id = QUAD_TYPE_MENU;
    quad.is_static = 1.0f;
    quad.alive = false;
    quad.entity_type_id = ENTITY_TYPE_ID_NA;
    quad.solid = false;
    quad.coll = false;

    button_quads.push_back(quad);

    fonts::render_text(saves[b].c_str(), 
                      (menu::Catalog[button_id].x + 20), 
                      (menu::Catalog[button_id].y  + (b*(menu::Catalog[button_id].h+5)) + 50), 
                      textures::FontTD, 
                      0.7, 
                      0.5, 
                      0.5, 
                      0.5,
                      1.0); 
    
    // insert name and quad id
    // CurrentMenuButtons.insert(std::pair<int, std::string>(quad.id, saves[b]));
    LoadSaveButtons.insert(std::pair<int, std::string>(quad.id, saves[b]));
    CurrentMenuButtons.insert(std::pair<int, std::string>(quad.id, menu::Catalog[button_id].name));
  }

  for(int i = 0; i < button_quads.size(); i++)
  { 
    // add vertex ids
    button_quads[i].a = quads::gen_vertex_id();
    button_quads[i].b = quads::gen_vertex_id();
    button_quads[i].c = quads::gen_vertex_id();
    button_quads[i].d = quads::gen_vertex_id();

    // create vertex struct - A
    button_quads[i].v_a.vertex_id = button_quads[i].a;
    button_quads[i].v_a.tile_id = button_quads[i].id;
    button_quads[i].v_a.frame_id = 0;
    button_quads[i].v_a.x_pos = (float)button_quads[i].x;
    button_quads[i].v_a.y_pos = (float)button_quads[i].y;
    button_quads[i].v_a.z_pos = 0.0f;
    button_quads[i].v_a.r_col = button_quads[i].r_col;
    button_quads[i].v_a.g_col = button_quads[i].g_col;
    button_quads[i].v_a.b_col = button_quads[i].b_col;
    button_quads[i].v_a.a_col = button_quads[i].a_col;
    button_quads[i].v_a.tex_coord_x = 0.0f;
    button_quads[i].v_a.tex_coord_y = 0.0f;
    button_quads[i].v_a.texture_id = 0;
    button_quads[i].v_a.is_clicked = (float)button_quads[i].is_clicked;
    button_quads[i].v_a.type_id = (float)button_quads[i].type_id;
    button_quads[i].v_a.is_static = (float)button_quads[i].is_static;

    // create vertex struct - B
    button_quads[i].v_b.vertex_id = button_quads[i].b;
    button_quads[i].v_b.tile_id = button_quads[i].id;
    button_quads[i].v_b.frame_id = 0;
    button_quads[i].v_b.x_pos = (float)button_quads[i].x + (float)button_quads[i].w;
    button_quads[i].v_b.y_pos = (float)button_quads[i].y;
    button_quads[i].v_b.z_pos = 0.0f;
    button_quads[i].v_b.r_col = button_quads[i].r_col;
    button_quads[i].v_b.g_col = button_quads[i].g_col;
    button_quads[i].v_b.b_col = button_quads[i].b_col;
    button_quads[i].v_b.a_col = button_quads[i].a_col;
    button_quads[i].v_b.tex_coord_x = 0.0f;
    button_quads[i].v_b.tex_coord_y = 0.0f;
    button_quads[i].v_b.texture_id = 0;
    button_quads[i].v_b.is_clicked = (float)button_quads[i].is_clicked;
    button_quads[i].v_b.type_id = (float)button_quads[i].type_id;
    button_quads[i].v_b.is_static = (float)button_quads[i].is_static;

    // create vertex struct - C
    button_quads[i].v_c.vertex_id = button_quads[i].c;
    button_quads[i].v_c.tile_id = button_quads[i].id;
    button_quads[i].v_c.frame_id = 0;
    button_quads[i].v_c.x_pos = (float)button_quads[i].x;
    button_quads[i].v_c.y_pos = (float)button_quads[i].y  + (float)button_quads[i].h;
    button_quads[i].v_c.z_pos = 0.0f;
    button_quads[i].v_c.r_col = button_quads[i].r_col;
    button_quads[i].v_c.g_col = button_quads[i].g_col;
    button_quads[i].v_c.b_col = button_quads[i].b_col;
    button_quads[i].v_c.a_col = button_quads[i].a_col;
    button_quads[i].v_c.tex_coord_x = 0.0f;
    button_quads[i].v_c.tex_coord_y = 0.0f;
    button_quads[i].v_c.texture_id = 0;
    button_quads[i].v_c.is_clicked = (float)button_quads[i].is_clicked;
    button_quads[i].v_c.type_id = (float)button_quads[i].type_id;
    button_quads[i].v_c.is_static = (float)button_quads[i].is_static;


    // create vertex struct - D
    button_quads[i].v_d.vertex_id = button_quads[i].d;
    button_quads[i].v_d.tile_id = button_quads[i].id;
    button_quads[i].v_d.frame_id = 0;
    button_quads[i].v_d.x_pos = (float)button_quads[i].x + (float)button_quads[i].w;
    button_quads[i].v_d.y_pos = (float)button_quads[i].y + (float)button_quads[i].h;
    button_quads[i].v_d.z_pos = 0.0f;
    button_quads[i].v_d.r_col = button_quads[i].r_col;
    button_quads[i].v_d.g_col = button_quads[i].g_col;
    button_quads[i].v_d.b_col = button_quads[i].b_col;
    button_quads[i].v_d.a_col = button_quads[i].a_col;
    button_quads[i].v_d.tex_coord_x = 0.0f;
    button_quads[i].v_d.tex_coord_y = 0.0f;
    button_quads[i].v_d.texture_id = 0;
    button_quads[i].v_d.is_clicked = (float)button_quads[i].is_clicked;
    button_quads[i].v_d.type_id = (float)button_quads[i].type_id;
    button_quads[i].v_d.is_static = (float)button_quads[i].is_static;

    // create vindices 
    button_quads[i].i_left.a = button_quads[i].a;
    button_quads[i].i_left.b = button_quads[i].b;
    button_quads[i].i_left.c = button_quads[i].c;

    button_quads[i].i_right.a = button_quads[i].b;
    button_quads[i].i_right.b = button_quads[i].c;
    button_quads[i].i_right.c = button_quads[i].d;
  }

  return button_quads;
}


  // function for loading main menu
  void load_menu(std::vector<int> menu_buttons)
  {
    if(menu_buttons[0] == 6)
    {// load saves logic

      MenuQuads = load_saves_buttons();

      // go back to menu button
      std::vector<quads::Quad> go_back_button = load_button_quads({5});
      if(menu::MenuQuads.size() > 0){
        menu::MenuQuads.insert(menu::MenuQuads.end(), go_back_button.begin(), go_back_button.end());
      }

    } else {
      MenuQuads = load_button_quads(menu_buttons);
    }
  }

  // Reads data in jsons to catalog
  void read_button_data(std::string name)
  {
    ButtonData BD;
    std::string data_path = "./menu/data/"+name+".json";
    logger::log(LOG_INFO, LOG_EVENT_READ_DATA, "menu::read_button_data", __FILE__, __LINE__, "Reading button data from " + data_path);
    std::string json_data = utils::read_text_file(data_path);
    JS::ParseContext context(json_data);
    context.parseTo(BD);
    menu::Catalog.insert({BD.id, BD});
  }

  void init()
  {
    // for now just debugging different options on runtime 
    logger::log(LOG_INFO, LOG_EVENT_INIT_MODULE, "menu::init", __FILE__, __LINE__, "Initializing menu");
    std::vector<std::string> button_list = utils::list_files("menu/data/");
    for(int b=0; b < button_list.size(); b++)
    {
      read_button_data(button_list[b]);
    }
    std::vector<std::string> saves = menu::list_saves();
    hero::Hero hero = hero::load_from_save(saves[0]);  
    logger::log(LOG_INFO, LOG_EVENT_INIT_MODULE, "menu::init", __FILE__, __LINE__, "Menu initialized");
  }

  // closes the menu
  void drop()
  {
    logger::log(LOG_INFO, LOG_EVENT_DROP_DATA, "menu::drop", __FILE__, __LINE__, "Dropping menu");
    CurrentMenuButtons.clear();
    LoadSaveButtons.clear();
    for (int q = 0; q < menu::MenuQuads.size(); q++)
    {
      quads::delete_quad_id(menu::MenuQuads[q].id);
      quads::delete_vertex_id(menu::MenuQuads[q].a);
      quads::delete_vertex_id(menu::MenuQuads[q].b);
      quads::delete_vertex_id(menu::MenuQuads[q].c);
      quads::delete_vertex_id(menu::MenuQuads[q].d);
    }
    menu::MenuQuads.clear();
  }

  std::string allowed_input = "abcdefghijklmnoprstuvwxyzABCDEFGHIJKLMNOPRSTUVWXYZ";
  bool validate_input(std::string input)
  {
    bool good = false;
    if (menu::allowed_input.find(input) != std::string::npos) {
      good = true;
    }
    return good;
  }
}

#endif
