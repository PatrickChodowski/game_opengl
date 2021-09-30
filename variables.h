
#ifndef VARS_H
#define VARS_H


namespace colls
{
  int SENSOR_COUNT = 9;
  int SENSOR_OFFSET = 1;

  int ABS_COUNT = 1;

  struct AABB
  {
    int min_x;
    int min_y;
    int max_x;
    int max_y;
    int id;
  };

  struct Sensor
  {
    float x;
    float y;
    int id;
  };

  struct SolidLimits
  {
    std::vector<float> left_borders = {};
    std::vector<float> right_borders = {};
    std::vector<float> top_borders = {};
    std::vector<float> bottom_borders = {};
  };

  struct DistanceBetweenPoints
  {
    int a_quad_id;
    int b_quad_id;
    int a_quad_type;
    int b_quad_type;
    float distance;
    float limit; // sum of quants diagonals
    bool is_near;
  };

}

namespace quads
{
  int COUNT_VERTEX_ATTRIBUTES = 14;
  int VERTEX_OFFSET = 1;
  int COUNT_QUADS = 0;
  int REQ_SIZE_BUFFER = 0;

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

  struct QuadLabel
  {
    std::string text;
    int x;
    int y;
    bool is_static;
    float scale;
  };


  // Quad will contain information about:
  // - original position (x,y of top left corner)
  // - type, solid, id
  // - vertex ids for a,b,c,d
  // - Vindices with vertex ids
  // - actual vertex information

// Contains all information about what is rendered
  struct Quad
  {
    int id;
    int x;
    int y;
    int w;
    int h;

    bool solid; // if true, will get collision with hero entity
    bool coll; // if true, will get distance to hero calculated
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
    // only for entities -> will store entity_id defined on mob::spawn and others
    int entity_id;
    bool alive;

    // Scaled metrics For collisions and mouse events:
    float s_x;
    float s_y;
    float s_w;
    float s_h;

    // 'diagonal' is the distance from the middle of the quad to the corner
    // would call it a radius but we are in a quad not a circle, but its like a radius to me
    float s_diag; // 'diagonal' is the distance from the center of the quad to the corner

    // used if given entity has collision sensors
    std::map<int, colls::Sensor> sensors;

    // its not abs, its AABBs, but this is what I do to entertain myself
    std::map<int, colls::AABB> abs; 

    // label collection
    std::vector<QuadLabel> labels;
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
  std::vector<int> UsedQuadIds = {};
  std::vector<int> UsedVertexIds = {};

  // Algorithm to find next available quad id 
  int find_next_quad_id()
  {
    int n = quads::UsedQuadIds.size();
    // for whole vector, find value that would be bigger than (index + 1)
    for (int i = 0; i < n; i++)
    {
      if (quads::UsedQuadIds[i] > (i+1)){
        return i+1;
      }
    }
    return n+1;
  }

  // Algorithm to find next available vertex id 
  int find_next_vertex_id()
  {
    int n = quads::UsedVertexIds.size();
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
    quads::UsedQuadIds.push_back(next_quad_id);
    return next_quad_id;
  }

  int gen_vertex_id()
  {
    int next_vertex_id = quads::find_next_vertex_id();
    quads::UsedVertexIds.push_back(next_vertex_id);
    return next_vertex_id;
  }

  // Will return the index of quad id
  int find_quad_id(int quad_id, std::vector<quads::Quad> quads)
  {
    int quad_index = -1;
    for(int q = 0; q < quads.size(); q++)
    {
      if(quad_id == quads[q].id)
      {
        quad_index = q;
        break;
      }
    }
    return quad_index;
  }
}

namespace debug
{
  std::vector<quads::Quad> DebugQuads;
}


namespace fonts
{
  std::vector<quads::Quad> TextQuads;
}

namespace maps
{
  std::vector<quads::Quad> MapQuads;
    struct Nest // nest for spawning mobs
  {
    float x;
    float y;
    int n;

    JS_OBJ(x, y, n);
  };

  struct Door
  {
    int door_id;
    int x;
    int y;
    int dest_map_id;
    int player_enter_x;
    int player_enter_y;

    JS_OBJ(door_id, x, y, dest_map_id, player_enter_x, player_enter_y);
  };

  struct MapData
  {
    int id;
    std::string name;
    int vertex_width;
    int vertex_height;
    int texture_id;
    int default_player_x;
    int default_player_y;
    std::vector<Door> doors;
    std::vector<Nest> nests;

    JS_OBJ(id, name, vertex_width, vertex_height, texture_id, default_player_x, default_player_y, doors, nests);
  };
  std::map<int, MapData> Catalog = {};

}

namespace mobs
{
  // Store all information about Alive Mob Entity - position, quad_id, mob type, state, stats etc.
  struct AliveMobData
  {
    float x;
    float y;
    float s_x;
    float s_y;
    int quad_id;
    int entity_id;
    int mob_id;
    int state = ENTITY_STATE_CALM;
    int hp;
    int speed;
  };

  // Table of Alive Mobs (id of Alive Mob and AliveMobData)
  std::vector<mobs::AliveMobData> AliveMobs = {};
}


namespace travel
{
  struct TravelPlan
  {
    int quad_id;
    int entity_id;
    std::vector<int> full_path;
    int current_step_index = 0;
    int current_node;
    float current_x;
    float current_y;

    int target_node;
    float target_x;
    float target_y;
    int next_node;

    int next_gate;
    float cpoint_x;
    float cpoint_y;
  };

  // entity_id, TravelPlan object
  std::map<int, TravelPlan> TravelControl;
  std::vector<int> TPsToRemove = {};
}

namespace anims
{
  // Play Animation data - event_id, quad_id, entity_id, current_frame, next_frame, delay, time_since_last_update, frame_update_time
  struct PlayAnimation
  {
    bool breakable;

    int event_id;
    int quad_id;
    int entity_id;
    int current_frame;
    int next_frame;
    int texture_id;
    int seq_index;

    float delay;
    float time_since_last_update;

    //auto frame_update_time = std::chrono::system_clock::now();
    std::time_t frame_update_time;

    std::string animation_label;
  };

  // Gathers current playanimation objects. <entity_id, Play Animation object>
  std::map<int, PlayAnimation> PlayAnimationControl;

  // Contains entity IDs of which animation should be removed
  std::vector<int> PAsToRemove = {};
}

namespace ent
{
  // Entity ID has to last longer than one frame and be assigned to entity since load till the end of its activity
  std::vector<int> UsedEntityIds = {};
  std::vector<quads::Quad> EntityQuads;

  // <entity_id, quad_id>
  std::map<int, int> EntityQuadList;

  // Algorithm to find next available entity id
  int find_next_entity_id()
  {
    int n = ent::UsedEntityIds.size();
    // for whole vector, find value that would be bigger than (index + 1)
    for (int i = 0; i < n; i++)
    {
      if (ent::UsedEntityIds[i] > (i+1)){
        return i+1;
      }
    }
    return n+1;
  }

  // Generate next entity id
  int gen_entity_id()
  {
    int next_entity_id = ent::find_next_entity_id();
    // std::cout << "next entity id: " << next_entity_id << std::endl;
    ent::UsedEntityIds.push_back(next_entity_id);
    return next_entity_id;
  }

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
  // Frame information - id, x, y, w, h, label. Read in with texture data
  struct Frame
  {
    int frame_id;
    int x;
    int y;
    int w;
    int h;
    std::string label;

    JS_OBJ(frame_id, x, y, w, h, label);
  };

  // Animation key data
  struct AnimationKey
  {
    int id;
    int next;
    float time;

    JS_OBJ(id, next, time);
  };

  // Animation information - id, x, y, w, h, label. Read in with texture data
  struct Animation
  {
    std::string label;
    int event_id;
    int length;
    std::vector<AnimationKey> sequence;

    std::vector<int> key_list;
    JS_OBJ(label, event_id, length, sequence);
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
    std::vector<Animation> anims_list;

    std::map<int, Frame> frames;

    // Catalog of <EventID-Animation Object>
    std::map<int, Animation> anims;
    unsigned int opengl_texture_id;

    JS_OBJ(id, type, name, width, height, frames_list, anims_list);
  };

  // Creating catalog of all textures data. <Texture_id - Texture Data object>
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

namespace nav
{
 // almost navigate, but no, its Nav Gate (gate between 2 polygons). Why I am not a comedian
  struct NavGate 
  {
    int id;
    int a_id;
    int b_id;
    float gate_min_x;
    float gate_max_x;
    float gate_min_y;
    float gate_max_y;
    float gate_s_min_x;
    float gate_s_max_x;
    float gate_s_min_y;
    float gate_s_max_y;
    int orientation;
  };

  // NavNode store information about AABB box, scaled AABB box, edges(map) and tile count
  struct NavNode
  {
    int id;
    float min_y;
    float min_x;
    float max_y;
    float max_x;
    float s_min_y;
    float s_min_x;
    float s_max_y;
    float s_max_x;
    std::map<int, NavGate> edges;
    int count_tiles;
  };

  // Map of NavNodeID (int) and NavNode
  std::map<int, NavNode> NavMesh;
  std::vector<std::vector<int>> NavMeshGraph;
}

namespace gui
{
  std::map<int, int> CurrentGuiWindows;
  std::vector<quads::Quad> GuiQuads;
}

#endif