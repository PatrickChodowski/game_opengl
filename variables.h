
#ifndef VARS_H
#define VARS_H

// file for defining variables that should be accessible by multiple files 

namespace quads
{

  #define QUAD_TYPE_MAP 0.0f
  #define QUAD_TYPE_MENU 1.0f
  #define QUAD_TYPE_TEXT 2.0f
  #define QUAD_TYPE_ENTITY 3.0f

  int COUNT_VERTEX_ATTRIBUTES = 14;
  int VERTEX_OFFSET = 1;



  
  // vertex index
  struct Vindex
  {  
    int a; 
    int b;
    int c;
  };


  // Vertex information
  struct Vertex
  {  
    // info send to GPU:
    double x_pos; 
    double y_pos;
    double z_pos;

    double r_col;
    double g_col;
    double b_col;
    double a_col;

    double frame_id;
    double tex_coord_x;
    double tex_coord_y;
    double texture_id;

    double is_clicked;
    double type_id;
    double is_static;

    /// not counted as VERTEX ATTRIBUTES
    int tile_id;
    int vertex_id;
  };

 


  // Quad will contain information about:
  // - original position (x,y of top left corner)
  // - type, solid, id
  // - vertex ids for a,b,c,d
  // - Vindices with vertex ids
  // - actual vertex information

  struct Quad
  {
    int id;
    int x;
    int y;
    int w;
    int h;
    bool solid;
    bool is_clicked;

    // vertex IDS:
    int a;
    int b;
    int c;
    int d;

    // a b
    // c d

    // Vertex information
    Vertex v_a;
    Vertex v_b;
    Vertex v_c;
    Vertex v_d;

    // vindices
    struct Vindex i_left;
    // a b
    // c
    struct Vindex i_right;
    //   b
    // c d

    // for assigning texture and frame in assign_vertices:
    int texture_id;
    int frame_id;

    // for passing colors
    float r_col;
    float g_col;
    float b_col;
    float a_col;
    
    // for different shader calls:
    float type_id; // 0 - level, 1 - menu, 2 - text, 3 - entity

    // to choose camera reaction:
    float is_static;

    // for entities logic:
    int entity_type_id;
    bool alive;
  };

  struct ScaledQuad
  {
    int id;
    float x;
    float y;
    float w;
    float h;
  };


  void print_out_quads(std::vector<Quad> quads)
  {
    for(int t=0; t<quads.size(); t++)
    {
      std::cout << "Quad ID: " << quads[t].id << std::endl; 
      std::cout << "Texture ID: " << quads[t].texture_id << std::endl;
      std::cout << "Frame ID: " << quads[t].frame_id << std::endl;
      std::cout << "X: " << quads[t].x << std::endl; 
      std::cout << "Y: " << quads[t].y << std::endl; 
      std::cout << "Width: " << quads[t].w << std::endl; 
      std::cout << "Height: " << quads[t].h << std::endl; 
      std::cout << "Type: " << quads[t].type_id << std::endl; 

      std::cout << "Vertices positions:"  << std::endl;
      std::cout << "Vertex ID: " << quads[t].a << " X:" << quads[t].v_a.x_pos << ", Y:" << quads[t].v_a.y_pos << ", tcX:" << quads[t].v_a.tex_coord_x << ", tcY:" << quads[t].v_a.tex_coord_y << std::endl;
      std::cout << "Vertex ID: " << quads[t].b << " X:" << quads[t].v_b.x_pos << ", Y:" << quads[t].v_b.y_pos << ", tcX:" << quads[t].v_b.tex_coord_x << ", tcY:" << quads[t].v_b.tex_coord_y << std::endl;
      std::cout << "Vertex ID: " << quads[t].c << " X:" << quads[t].v_c.x_pos << ", Y:" << quads[t].v_c.y_pos << ", tcX:" << quads[t].v_c.tex_coord_x << ", tcY:" << quads[t].v_c.tex_coord_y << std::endl;
      std::cout << "Vertex ID: " << quads[t].d << " X:" << quads[t].v_d.x_pos << ", Y:" << quads[t].v_d.y_pos << ", tcX:" << quads[t].v_d.tex_coord_x << ", tcY:" << quads[t].v_d.tex_coord_y << std::endl;

      std::cout << "Vindices:" << std::endl <<
      quads[t].i_left.a << " " << quads[t].i_left.b << " " << quads[t].i_left.c << std::endl <<
      quads[t].i_right.a << " " << quads[t].i_right.b << " " << quads[t].i_right.c << std::endl;
    }
  }

  /*
    AllQuads will be a vector containing all quads to be sent to buffer on each iteration.
    It gathers levels::LevelQuads, menu::MenuQuads, fonts::TextQuads into one chunk of information to be send on the update buffer

    Meta information about quads sent will be stored in map Quads Summary - currently only name of the type and size
  */
  std::vector<quads::Quad> AllQuads;
  std::map<std::string, int> QuadsSummary;
  std::vector<quads::ScaledQuad> ScaledAllQuads;
  std::vector<int> UsedQuadIds = {};
  std::vector<int> UsedVertexIds = {};

  int find_next_quad_id()
  {
    /*
      Algorithm to find next available quad id 
    */
    
    int n = quads::UsedQuadIds.size();
    bool found = false;

    // for whole vector, find value that would be bigger than (index + 1)
    for (int i = 0; i < n; i++)
    {
      if (quads::UsedQuadIds[i] > (i+1)){
        return i+1;
      }
    }
    return n+1;
  }

  int find_next_vertex_id()
  {
    /*
      Algorithm to find next available vertex id 
    */
    
    int n = quads::UsedVertexIds.size();
    bool found = false;

    // for whole vector, find value that would be bigger than (index + 1)
    for (int i = 0; i < n; i++)
    {
      if (quads::UsedVertexIds[i] > i){
        return i;
      }
    }
    return n;
  }

  int gen_quad_id()
  {
    int next_quad_id = quads::find_next_quad_id();
    // std::cout << "Next Quad id: " << next_quad_id << std::endl;
    quads::UsedQuadIds.push_back(next_quad_id);
    return next_quad_id;
  }

   int gen_vertex_id()
  {
    int next_vertex_id = quads::find_next_vertex_id();
    // std::cout << "Next vertex id: " << next_vertex_id << std::endl;
    quads::UsedVertexIds.push_back(next_vertex_id);
    return next_vertex_id;
  }
}

namespace fonts
{
  std::vector<quads::Quad> TextQuads;
}

namespace maps
{
  std::vector<quads::Quad> MapQuads;
  std::vector<quads::ScaledQuad> ScaledMapQuads;

  // used only to optimize collisions, created when map data is loaded, contains only is_solid=true
  std::vector<quads::Quad> SolidMapQuads;
}

namespace ent
{
  std::vector<quads::Quad> EntityQuads;
}

namespace menu
{
  std::string NewGameName = "";
  std::vector<quads::Quad> MenuQuads;
  // Current Menu buttons store name and quad id
  std::map<int, std::string> CurrentMenuButtons;
  std::map<int, std::string> LoadSaveButtons;


std::vector<std::string> list_saves()
{
  std::vector<std::string> saves = {};
  saves = utils::list_files("saves");
  return saves;
}

  bool validate_name()
  {
    bool good = false;
    // check if doesnt exist in saves
    // check if has more than 0 characters

    if(menu::NewGameName.size() > 0)
    {
      std::vector<std::string> saves = menu::list_saves();
      for(int s = 0; s<saves.size(); s++)
      {
        if(menu::NewGameName == saves[s]){
          return good;
        }
      }
      good = true;
    }
    return good;
  }
}

namespace textures 
{
    struct Frame
  {
    int frame_id;
    int x;
    int y;
    int w;
    int h;
    int is_solid;

    JS_OBJ(frame_id, x, y, w, h, is_solid);
  };

  // General texture information
  struct TextureData
  {
    int id;
    std::string type;
    std::string name;
    int width;
    int height;
    std::vector<Frame> frames_list;
    std::map<int, Frame> frames;

    unsigned int opengl_texture_id;

    JS_OBJ(id, type, name, width, height, frames_list);
  };

  // Creating catalog of all textures data 
  std::map<int, TextureData> Catalog = {};
  std::vector<unsigned int> BoundTextures = {};
  textures::TextureData FontTD;
}

namespace game 
{
  glm::mat4 STATIC_MVP;
  glm::mat4 DYNAMIC_MVP;
  glm::mat4 ZOOM_MVP;

  // starts with menu on:
  std::map<std::string, bool> GAME_STATE;
  bool CHANGE_STATE_TRIGGER = false;
  std::vector<std::string> GAME_STATE_LIST = {"GAME_ON",
                                              "MAIN_MENU",
                                              "NEW_GAME_MENU",
                                              "LOAD_GAME_MENU",
                                              "SETTINGS_MENU"};
  void init_game_states()
  {
    GAME_STATE.insert(std::pair<std::string, bool>("GAME_ON", false));
    GAME_STATE.insert(std::pair<std::string, bool>("MAIN_MENU", true));
    GAME_STATE.insert(std::pair<std::string, bool>("NEW_GAME_MENU", false));
    GAME_STATE.insert(std::pair<std::string, bool>("LOAD_GAME_MENU", false));
    GAME_STATE.insert(std::pair<std::string, bool>("SETTINGS_MENU", false));
  }

  std::string get_state()
  {
    std::string current_state;
    for(int s = 0; s < GAME_STATE_LIST.size(); s++)
    {
      if(GAME_STATE[GAME_STATE_LIST[s]])
      {
        current_state = GAME_STATE_LIST[s];
        break;
      }
    }
    std::cout << "Current state: " << current_state << std::endl;
    return current_state;
  }

  void set_state(std::string state_name)
  {
    std::string old_state = get_state();
    if(old_state != state_name){
      GAME_STATE[state_name] = true;
      for(int s = 0; s < GAME_STATE_LIST.size(); s++)
      {
        if(GAME_STATE_LIST[s] != state_name){
          GAME_STATE[GAME_STATE_LIST[s]] = false;
        }
      }
      std::cout << "Set the game state to " << state_name << std::endl;
      CHANGE_STATE_TRIGGER = true;
    }
  }


  void print_game_states()
  {
    for (auto const& x : GAME_STATE)
    {
      std::cout << x.first << ':' << x.second << std::endl;
    }
  }


}

#endif