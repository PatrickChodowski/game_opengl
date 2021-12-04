
#include <map>
#include <string>
#include <vector>

#ifndef MODULES_TRAVEL_H
#define MODULES_TRAVEL_H

namespace travel
{
  struct TravelData
  {
    int entity_id;
    int current_step_index = 0;
    int current_node;
    int target_node;
    int next_gate;
    int next_node;

    // #define TRAVEL_STATE_IDLE 0
    // #define TRAVEL_STATE_ACTIVE 1
    // #define TRAVEL_STATE_FINISHED 2
    int state;


    float current_x;
    float current_y;
    float target_x;
    float target_y;
    float cpoint_x;
    float cpoint_y;

    std::vector<int> full_path;

  };

  struct TravelPoint
  {
    float x;
    float y;
  };

  extern travel::TravelPoint last_click;
  extern std::map<int, travel::TravelData> travels;
  extern std::vector<int> travels_to_cancel;

  void reset_last_click();

  // Calculates angle between 2 points
  float _get_angle_between_points(float e_x, float e_y, float p_x, float p_y);

  // Finds nearest point on target NavGate
  travel::TravelPoint _get_nearest_point_on_line(float l_x1, float l_y1, float l_x2, float l_y2, float p_x, float p_y);

  // Checks if current entity position is on correct node (node in travel) updates if its not
  int _sanity_check_replace_tp(float x, float y, int tp_current_node_id);

  // Checks if entity has already a travelData assigned
  bool _check_if_entity_already_moving(int entity_id);

  // Make plan of the travel between start node and end node. 
  // Calls pathfinder::find_path
  travel::TravelData make_basic_plan(int current_node_id, int target_node_id);

  //Calls travel::make_basic_plan, then calculates distance 
  // and angle to the next point, finds next gate and updates the position of entity
  void go(travel::TravelData tp);

  // Manages the travels. Calls travel::go on each TravelData and cancels finished or broken travels
  void update();

  // Deletes travel from travels  by travel_id
  void drop(int travel_id);

  // Remove data from travels and travels_to_cancel
  void clear();

  // initialize travel object without knowing the end yet, keeping in idle state
  void init_travel(int entity_id);



}

#endif
