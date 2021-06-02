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



std::vector<quads::Quad> load_menu_quads(std::vector<std::string> button_list)
{
  // use color to draw background and buttons for now
  // no clue how to add text yet lol

  // vertex data:
  // use - x,y,z  r,g,b,a   , is_clicked, 

  std::vector<quads::Quad> button_quads;
  for(int b = 0; b < button_list.size(); b++)
  {
    struct quads::Quad quad;
    button_quads.push_back(quad);
  } 
  return button_quads;
}



void load_main_menu()
{
  std::vector<std::string> main_menu_buttons = {"New", "Load", "Settings", "Exit"};
  MenuQuads = load_menu_quads(main_menu_buttons);

}



void init()
{
  // for now just debugging different options on runtime 
  std::vector<std::string> saves = menu::list_saves();
  hero::Hero hero = hero::load_from_save(saves[0]);
  
}




// pause menu
  






}

#endif
