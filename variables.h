
#ifndef VARS_H
#define VARS_H

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


#endif