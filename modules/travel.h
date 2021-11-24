
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













}

#endif
