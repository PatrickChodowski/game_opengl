
#ifndef VARS_H
#define VARS_H

// file for defining variables that should be accessible by multiple files 
#include "dictionary.h"

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
  int find_quad_id(int quad_id, std::vector<quads::Quad> quads)
  {
    int quad_index = -1;
    /*Will return the index of quad id*/
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

    JS_OBJ(frame_id, x, y, w, h);
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
  };

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
  std::map<int, NavNode> NavMesh;
  std::vector<std::vector<int>> NavMeshGraph;
}

namespace paths
{
  int get_navnode_id(float x, float y)
  {
    int nav_node_id = -1;
    for (auto const& nn : nav::NavMesh)
    { 
      if((x >= nn.second.s_min_x) & (x <= nn.second.s_max_x) & (y >= nn.second.s_min_y) & (y <= nn.second.s_max_y))
      { 
        nav_node_id = nn.first;
        break;
      }
     }
    std::cout << " Point: " << x << "," << y << " Node id: " << nav_node_id << std::endl;
    return nav_node_id;
  }

int min_distance(std::vector<float> distances, 
                std::vector<bool> polygon_included,
                int polygon_count
                )
{
    // Initialize min value
    int min = INT_MAX, min_index;
 
    for (int v = 0; v < polygon_count; v++)
        if (polygon_included[v] == false && distances[v] <= min)
            min = distances[v], min_index = v;
 
    return min_index;
}

  void find_path_djikstra(int start_node_id, int end_node_id)
  {
    int polygon_count = nav::NavMesh.size();
    std::vector<bool> polygon_included = {};
    std::vector<float> distances = {};

    for(int v = 0; v < polygon_count; v++)
    {
      polygon_included.push_back(false);
      distances.push_back(INT_MAX);
    }

    // distance of end_node_id to itself will be always 0:
    distances[(end_node_id-1)] = 0;

    for (int count = 0; count < polygon_count - 1; count++) 
    {
      int u = min_distance(distances, polygon_included, polygon_count);
      polygon_included[u] = true;

      for (int v = 0; v < polygon_count; v++)
      {
        // if (!polygon_included[v] && nav::NavMeshGraph[u][v] && distances[u] != INT_MAX
        //     && distances[u] + nav::NavMeshGraph[u][v] < distances[v]){
        //       distances[v] = distances[u] + graph[u][v];
        // }
      }
    }
  }
  


  void find_path(int start_node_id, int end_node_id)
  {
    if((start_node_id > -1) & (end_node_id > -1))
    {
      std::cout << "Looking for path between " << start_node_id << " and " << end_node_id << std::endl;
      std::vector<int> nodes_visited = {};
      int limit = 10;
      int count = 0;
      int current_agent_node_id = start_node_id;
      nodes_visited.push_back(current_agent_node_id);

      while((current_agent_node_id != end_node_id) & (count <= limit))
      {
          for (auto const& e : nav::NavMesh[current_agent_node_id].edges)
          {
              if(!(std::find(nodes_visited.begin(), nodes_visited.end(), e.first) != nodes_visited.end()))
              {
                  // not visited yet
                  current_agent_node_id = e.first;
                  nodes_visited.push_back(current_agent_node_id);
                  break;
              }
          }
        count += 1;
      }
      for(int n = 0; n < nodes_visited.size(); n++)
      {
          std::cout << nodes_visited[n] << " -> ";
      }
      std::cout << std::endl;
    }
  }
}




namespace mobs
{
  void move_to_point(float x, float y)
  {
    // get mob quad id (temporary):
    int quad_index; 
    for(int q = 0; q < quads::AllQuads.size(); q++)
    {
      if(quads::AllQuads[q].entity_type_id == ENTITY_TYPE_ID_MOB)
      {
        quad_index = q;
        break;
      }
    }
    std::cout << "Mob Quad Index: " << quad_index << std::endl;
    std::cout << "Mob Position: " << quads::AllQuads[quad_index].s_x << "," << quads::AllQuads[quad_index].s_y << std::endl;

    // int quad_index = quads::find_quad_id(quad_id, quads::AllQuads);
    int quad_node_id = paths::get_navnode_id(quads::AllQuads[quad_index].s_x, quads::AllQuads[quad_index].s_y);
    // std::cout << "Mob Polygon: " << quad_node_id << std::endl;


    int target_node_id = paths::get_navnode_id(x, y);

    if(quad_node_id != target_node_id)
    {
      paths::find_path(quad_node_id, target_node_id);
    }
  }


}
#endif