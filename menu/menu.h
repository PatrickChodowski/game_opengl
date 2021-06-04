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

  JS_OBJ(id, x, y, w, h, label, name);
};

std::map<int, ButtonData> Catalog = {};
std::vector<quads::Quad> MenuQuads; 

std::vector<std::string> list_saves()
{
  std::vector<std::string> saves = {};
  saves = utils::list_files("saves");
  logger::print_vector(saves);
  return saves;
}



std::vector<quads::Quad> load_menu_quads(std::vector<int> button_list)
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
    quad.id = menu::Catalog[button_id].id;
    quad.x = menu::Catalog[button_id].x;
    quad.y = menu::Catalog[button_id].y;
    quad.w = menu::Catalog[button_id].w;    
    quad.h = menu::Catalog[button_id].h;

    button_quads.push_back(quad);
  } 
  return button_quads;
}



void load_main_menu()
{
  // button ids
  std::vector<int> main_menu_buttons = {0,1,2,3};
  MenuQuads = load_menu_quads(main_menu_buttons);

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
    << " width: " << BD.w << " height: " << BD.h << std::endl;
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




// pause menu
  






}

#endif
