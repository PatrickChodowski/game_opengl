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

std::vector<std::string> list_saves()
{
  std::vector<std::string> saves = {};
  saves = utils::list_files("saves");
  logger::print_vector(saves);
  return saves;
}


// function for generating quads for buttons
std::vector<quads::Quad> load_button_quads(std::vector<int> button_list)
{
  // use color to draw background and buttons for now
  // no clue how to add text yet lol

  // vertex data:
  // use - x,y,z  r,g,b,a   , is_clicked, 

  std::vector<quads::Quad> button_quads;
  for(int b = 0; b < button_list.size(); b++)
  {
    struct quads::Quad quad;
    int button_id = button_list[b];
    // quad.id = menu::Catalog[button_id].id;
    quad.id = quads::gen_quad_id();
    quad.x = menu::Catalog[button_id].x;
    quad.y = menu::Catalog[button_id].y;
    quad.w = menu::Catalog[button_id].w;    
    quad.h = menu::Catalog[button_id].h;
    quad.r_col = menu::Catalog[button_id].r_col;
    quad.g_col = menu::Catalog[button_id].g_col;
    quad.b_col = menu::Catalog[button_id].b_col;
    quad.a_col = menu::Catalog[button_id].a_col;
    quad.is_clicked = 0.0f;
    quad.type_id = QUAD_TYPE_MENU;
    button_quads.push_back(quad);
    // std::cout << "menu quad id:" << std::endl;
    // std::cout << quad.id << std::endl;
    fonts::render_text(menu::Catalog[button_id].label.c_str(), 
                      (menu::Catalog[button_id].x + 20), 
                      (menu::Catalog[button_id].y + 50), 
                      textures::FontTD, 
                      0.7, 
                      0.5, 
                      0.5, 
                      0.5); 

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
  MenuQuads = load_button_quads(menu_buttons);
}

// Reads data in jsons to catalog
void read_button_data(std::string name)
{
  ButtonData BD;
  std::string data_path = "./menu/data/"+name+".json";
  std::string json_data = utils::read_text_file(data_path);
  JS::ParseContext context(json_data);
  context.parseTo(BD);

  // add to catalog
  menu::Catalog.insert({BD.id, BD});
  if(LOGGING == 0)
  { 
    std::cout << "Read-in button id: " << BD.id << ", name: " << BD.name << std::endl
    << " label: " << BD.label << std::endl << " position: " << BD.x << "," << BD.y << std::endl
    << " width: " << BD.w << " height: " << BD.h << std::endl
    << " colors: "<< BD.r_col << "," << BD.b_col << "," << BD.g_col << "," << BD.a_col << std::endl;
  }
}

void init()
{
  // for now just debugging different options on runtime 
  logger::print("READING BUTTONS");
  std::vector<std::string> button_list = utils::list_files("menu/data/");
  for(int b=0; b < button_list.size(); b++)
  {
    read_button_data(button_list[b]);
  }
  std::vector<std::string> saves = menu::list_saves();
  hero::Hero hero = hero::load_from_save(saves[0]);  
}

// closes the menu
void drop()
{
  CurrentMenuButtons.clear();
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





// pause menu
  






}

#endif
