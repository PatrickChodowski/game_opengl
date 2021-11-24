
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

  extern std::map<int, travel::TravelData> travels;
  extern std::vector<int> travels_to_cancel;

  float _get_angle_between_points(float e_x, float e_y, float p_x, float p_y);

  travel::TravelPoint _get_nearest_point_on_line(float l_x1, float l_y1, float l_x2, float l_y2, float p_x, float p_y);

  int _sanity_check_replace_tp(float x, float y, int tp_current_node_id);

  bool _check_if_entity_already_moving(int entity_id);

  travel::TravelData make_basic_plan(int current_node_id, int target_node_id);

  void go(travel::TravelData tp);

  void update();

  void drop(int travel_id);

  void clear();



}

#endif
