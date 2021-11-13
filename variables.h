
#ifndef VARS_H
#define VARS_H

namespace debug
{
  std::vector<quads::Quad> DebugQuads;
}

namespace fonts
{
  std::vector<quads::Quad> TextQuads;
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
    int texture_id;
    int seq_index;
    int length;
    int delay;
    int idle_frame;

    float time_since_last_update;

    //auto frame_update_time = std::chrono::system_clock::now();
    // std::time_t frame_update_time;
    std::chrono::milliseconds frame_update_time = timer::get_current_ms_time();
    std::string animation_label;
  };


  // Gathers current playanimation objects. <entity_id, Play Animation object>
  //std::map<int, PlayAnimation> PlayAnimationControl;

  // Gathers current playanimation objects. vector::<Play Animation object>
  std::vector<PlayAnimation> PlayAnimationControl;  

  // Contains entity IDs of which animation should be removed
  std::vector<int> PAsToRemove = {};
}

  // // Takes index of quad in entities and new frame id, and updates values in the quad
  // void update_frame(int ent_quad_index_id, int frame_id)
  // {
  //   int texture_id = ent::EntityQuads[ent_quad_index_id].texture_id;
  //   float norm_x_start = (float)textures::Catalog[texture_id].frames[frame_id].x/
  //   (float)textures::Catalog[texture_id].width;

  //   float norm_x_end =  (float)(textures::Catalog[texture_id].frames[frame_id].x + 
  //   textures::Catalog[texture_id].frames[frame_id].w)/
  //   (float)textures::Catalog[texture_id].width;

  //   ent::EntityQuads[ent_quad_index_id].frame_id = frame_id;
  //   ent::EntityQuads[ent_quad_index_id].v_a.tex_coord_x = norm_x_start;
  //   ent::EntityQuads[ent_quad_index_id].v_c.tex_coord_x = norm_x_start;
  //   ent::EntityQuads[ent_quad_index_id].v_b.tex_coord_x = norm_x_end;
  //   ent::EntityQuads[ent_quad_index_id].v_d.tex_coord_x = norm_x_end;
  //   ent::EntityQuads[ent_quad_index_id].v_a.frame_id = frame_id;
  //   ent::EntityQuads[ent_quad_index_id].v_b.frame_id = frame_id;
  //   ent::EntityQuads[ent_quad_index_id].v_c.frame_id = frame_id;
  //   ent::EntityQuads[ent_quad_index_id].v_d.frame_id = frame_id;
  // }


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