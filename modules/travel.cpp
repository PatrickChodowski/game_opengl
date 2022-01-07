
#include <iostream>
#include <string>
#include <vector>

#include "anims.h"
#include "entity.h"
#include "maps.h"
#include "navmesh.h"
#include "pathfinder.h"
#include "travel.h"
#include "utils.h"

#include "../dictionary.h"
#include "../dependencies/parallel_hashmap/phmap.h"


namespace travel
{
  phmap::flat_hash_map<int, travel::TravelData> travels;
  std::vector<int> travels_to_cancel;

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

  bool _check_if_entity_already_moving(int entity_id)
  {
    return travel::travels.count(entity_id) > 0;
  }

  void init_travel(int entity_id)
  {
    // To trigger delayed travel, when we dont know the endpoint yet
    travel::TravelData tp;
    tp.state = TRAVEL_STATE_IDLE;
    int entity_node_id = paths::get_navnode_id(entity::entities.at(entity_id).pos.x, entity::entities.at(entity_id).pos.y);
    tp.entity_id = entity_id;
    tp.current_node = entity_node_id;
    tp.current_x = entity::entities.at(entity_id).pos.x;
    tp.current_y = entity::entities.at(entity_id).pos.y;

    travel::travels[entity_id] = tp;
    std::cout << "Initialized travel for " << entity_id << std::endl;
  }

  void init_travel_with_target(int entity_id, float target_x, float target_y)
  {
    int entity_node_id = paths::get_navnode_id(entity::entities.at(entity_id).pos.x, entity::entities.at(entity_id).pos.y);
    int target_node = paths::get_navnode_id(target_x, target_y);
    travel::TravelData tp = travel::make_basic_plan(entity_node_id, target_node);
  
    tp.entity_id = entity_id;
    tp.current_node = entity_node_id;
    tp.current_x = entity::entities.at(entity_id).pos.x;
    tp.current_y = entity::entities.at(entity_id).pos.y;

    tp.target_node = paths::get_navnode_id(target_x, target_y);
    tp.target_x = target_x;
    tp.target_y = target_y;
    tp.state = TRAVEL_STATE_ACTIVE;

    travel::travels[entity_id] = tp;
    std::cout << "Initialized travel to  (" << target_x << "," << target_y <<  ") for " << entity_id << std::endl;
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

  void go(travel::TravelData tp)
  {
    int checked_real_node = travel::_sanity_check_replace_tp(tp.current_x, tp.current_y, tp.current_node);
    // if not correct then adjust plan
    if(checked_real_node != tp.current_node & checked_real_node != -1)
    {
      travel::TravelData tp_adj = make_basic_plan(checked_real_node, tp.target_node);
      tp.current_node = tp_adj.current_node;
      tp.next_node = tp_adj.next_node;
      tp.full_path = tp_adj.full_path;
    }

    // if we are not at the target yet, we move
    float dist_to_target = utils::get_distance_between_points(tp.current_x, tp.current_y, tp.target_x, tp.target_y);
    if(dist_to_target > entity::entities.at(tp.entity_id).speed)
    {
      // if we are not at the target node
      if(tp.current_node != tp.target_node)
      {
        travel::TravelPoint c_point;
        nav::NavGateData gate = nav::navnodes[tp.current_node].edges[tp.next_node];
        tp.next_gate = gate.id;
        if(gate.orientation == NAVGATE_HORIZONTAL_ORIENTATION)
        {
          c_point = travel::_get_nearest_point_on_line(gate.gate_min_x, gate.gate_min_y, 
                                              gate.gate_max_x, gate.gate_min_y, 
                                              tp.current_x, tp.current_y);

        } else if (gate.orientation == NAVGATE_VERTICAL_ORIENTATION)
        {
          c_point = travel::_get_nearest_point_on_line(gate.gate_min_x, gate.gate_min_y, 
                                              gate.gate_min_x, gate.gate_max_y, 
                                              tp.current_x, tp.current_y);
        }
        tp.cpoint_x = c_point.x;
        tp.cpoint_y = c_point.y;
        float dist = utils::get_distance_between_points(tp.current_x, tp.current_y, c_point.x, c_point.y);
        float angle = travel::_get_angle_between_points(tp.current_x, tp.current_y, c_point.x, c_point.y);
        float x1 = entity::entities.at(tp.entity_id).pos.x + (cos(angle) * entity::entities.at(tp.entity_id).speed);
        float y1 = entity::entities.at(tp.entity_id).pos.y + (sin(angle) * entity::entities.at(tp.entity_id).speed);

        travel::_animate_by_direction(tp.entity_id, angle);
        tp.current_x = x1;
        tp.current_y = y1;
        entity::update_position(tp.entity_id, x1, y1);

        // update the current and next node information if we are almost at the gate
        if(dist <= entity::entities.at(tp.entity_id).speed)
        {
          tp.current_step_index += 1;
          tp.current_node = tp.full_path[tp.current_step_index];
          if(tp.current_node != tp.target_node)
          {
            tp.next_node = tp.full_path[(tp.current_step_index+1)];
          }
        }

      } else if (tp.current_node == tp.target_node)
      {
        float dist = utils::get_distance_between_points(tp.current_x, tp.current_y, tp.target_x, tp.target_y);
        float angle = travel::_get_angle_between_points(tp.current_x, tp.current_y, tp.target_x, tp.target_y);
        float x1 = entity::entities.at(tp.entity_id).pos.x + cos(angle) * entity::entities.at(tp.entity_id).speed;
        float y1 = entity::entities.at(tp.entity_id).pos.y + sin(angle) * entity::entities.at(tp.entity_id).speed;
        travel::_animate_by_direction(tp.entity_id, angle);
        tp.current_x = x1;
        tp.current_y = y1;

        entity::update_position(tp.entity_id, x1, y1);
      }
      travel::travels[tp.entity_id] = tp;
    } else 
    {
      travel::travels_to_cancel.push_back(tp.entity_id);
    }
  }

  void _animate_by_direction(int entity_id, float angle)
  {
    // Looks like very much temporary function
    if(cos(angle) > 0)
    {// going left
      anims::start(2,entity_id);
    } else if (cos(angle) < 0)
    {// going right
      anims::start(3,entity_id);
    }
    
    if(sin(angle) >0.5)
    {
      // going up
      anims::start(0,entity_id);
    } else if (sin(angle) < -0.5)
    {// going down
      anims::start(1,entity_id);
    }

  }

  void update()
  {
    travel::travels_to_cancel.clear();
    for (auto & [k, v] : travel::travels)
    {  
      if(v.state == TRAVEL_STATE_ACTIVE)
      {
        travel::go(v);
      }
    }

    for(int i=0; i < travel::travels_to_cancel.size(); i++)
    {
      travel::drop(travel::travels_to_cancel[i]);
    }

  }

  void drop(int travel_id)
  {
    travel::travels.erase(travel_id); 
  }

  void clear()
  {
    travel::travels.clear();
    travel::travels_to_cancel.clear();

  }
}
