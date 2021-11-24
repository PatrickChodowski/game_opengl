
#include <map>
#include <string>
#include <vector>

#include "travel.h"
#include "utils.h"

namespace travel
{
  std::map<int, travel::TravelData> travels;

  float _get_angle_between_points(float e_x, float e_y, float p_x, float p_y)
  {
    float angle = (atan2((p_y - e_y), (p_x - e_x)));
    return angle;
  }

  travel::TravelPoint _get_nearest_point_on_line(float l_x1, float l_y1, float l_x2, float l_y2, float p_x, float p_y)
  {
    float dx = l_x2 - l_x1;
    float dy = l_y2 - l_y1;
    float det = (dx*dx) + (dy*dy);
    float a = abs(((dy*(p_y-l_y1))+(dx*(p_x-l_x1)))/det);
    travel::TravelPoint tpp;
    tpp.x = l_x1+(a*dx);
    tpp.y = l_y1+(a*dy);
    return tpp;
  }

  int _sanity_check_replace_tp(float x, float y, int tp_current_node_id)
  {
    int correct_node = tp_current_node_id;
    int real_node_id = paths::get_navnode_id(x, y);
    if(tp_current_node_id != real_node_id)
    {
      correct_node = real_node_id;
    }
    return correct_node;
  }

  travel::TravelData make_basic_plan(int current_node_id, int target_node_id)
  {
    travel::TravelData tp;
    if(current_node_id != target_node_id)
    {
      std::vector<int> path = paths::find_path(current_node_id, target_node_id);
      tp.full_path = path;
      tp.next_node = path[1];
    } else if (current_node_id == target_node_id){
      tp.full_path = {};
      tp.next_node = target_node_id;
    }
    tp.current_node = current_node_id;
    tp.target_node = target_node_id;
    return tp;

  }

  void go(travel::TravelData)
  {
    int checked_real_node = travel::sanity_check_replace_tp(tp.current_x, tp.current_y, tp.current_node);



  }

}
