
#ifndef TRAVEL_H
#define TRAVEL_H

namespace travel
{
    float get_distance_between_points(float a_x, float a_y, float b_x, float b_y)
    {
      float distance = std::sqrt(std::pow((a_x - b_x), 2) + std::pow((a_y-b_y), 2));
      return distance;
    }

    // Gets angle between entity loc and point loc. Creates invisble line through entity X and gets 360 
    float get_angle_between_points(float e_x, float e_y, float p_x, float p_y)
    {
      float angle = (atan2((p_y - e_y), (p_x - e_x)))*(180/M_PI);
      return angle;
    }

    std::pair<float, float> get_nearest_point_on_line(float l_x1, float l_y1, float l_x2, float l_y2, float p_x, float p_y)
    {
        float dx = l_x2 - l_x1;
        float dy = l_y2 - l_y1;
        float det = dx*dx + dy*dy;
        float a = (dy*(p_y-l_y1)+dx*(p_x-l_x1))/det;
        float c_x = l_x1+a*dx;
        float c_y = l_y1+a*dy;
        std::pair<float, float> point = {c_x, c_y};
        return point;
    }

    // Basically a decision tree handling for TravelPlan
    void go(travel::TravelPlan tp)
    {
        // if we are not at the target yet, we move
        float dist_to_target = get_distance_between_points(tp.current_x, tp.current_y, tp.target_x, tp.target_y);
        // std::cout << "dist to target" << dist_to_target <<  std::endl;
        if(dist_to_target > 100)
        {
          // if we are not at the target node
          if(tp.current_node != tp.target_node)
          {   
            // define the gate to the next node:
            std::pair<float, float> c_point;
            nav::NavGate gate = nav::NavMesh[tp.current_node].edges[tp.next_node];

            // find the closest point to the gate and calculate distance
            if(gate.orientation == NAVGATE_HORIZONTAL_ORIENTATION)
            {
                 c_point = get_nearest_point_on_line(gate.gate_min_x, gate.gate_min_y, 
                                                     gate.gate_max_x, gate.gate_min_y, 
                                                     tp.current_x, tp.current_y);

            } else if (gate.orientation == NAVGATE_VERTICAL_ORIENTATION)
            {
                 c_point = get_nearest_point_on_line(gate.gate_min_x, gate.gate_min_y, 
                                                     gate.gate_min_x, gate.gate_max_y, 
                                                     tp.current_x, tp.current_y);
            }

            float dist = get_distance_between_points(tp.current_x, tp.current_y, c_point.first, c_point.second);
            float angle = get_angle_between_points(tp.current_x, tp.current_y, c_point.first, c_point.second);
            float x1 = mobs::AliveMobs[0].x + cos(angle) * mobs::AliveMobs[0].speed;
            float y1 = mobs::AliveMobs[0].y + sin(angle) * mobs::AliveMobs[0].speed;
            
            //std::cout << "distance: " << dist <<std::endl;
            //std::cout << "angle: " << angle <<std::endl;
            //std::cout << "target: " << c_point.first << "," << c_point.second << std::endl;
            // std::cout << "x1: " << x1 <<std::endl;
            // std::cout << "y1: " << y1 <<std::endl;

            tp.current_x = x1;
            tp.current_y = y1;
            mobs::AliveMobs[0].x = x1;
            mobs::AliveMobs[0].y = y1;

            // update the current and next node information if we are almost at the gate
            if(dist <= mobs::AliveMobs[0].speed)
            {
              tp.current_step_index += 1;
              tp.current_node = tp.full_path[tp.current_step_index];
              if(tp.current_node != tp.target_node)
              {
                tp.next_node = tp.full_path[(tp.current_step_index+1)];
              }
 
            }

          } else if (tp.current_node == tp.target_node){

            float dist = get_distance_between_points(tp.current_x, tp.current_y, tp.target_x, tp.target_y);
            float angle = get_angle_between_points(tp.current_x, tp.current_y, tp.target_x, tp.target_y);
            float x1 = mobs::AliveMobs[0].x + cos(angle) * mobs::AliveMobs[0].speed;
            float y1 = mobs::AliveMobs[0].y + sin(angle) * mobs::AliveMobs[0].speed;

            //std::cout << "distance: " << dist <<std::endl;
            //std::cout << "angle: " << angle <<std::endl;
            //std::cout << "target: " << tp.target_x << "," << tp.target_y << std::endl;

            tp.current_x = x1;
            tp.current_y = y1;
            mobs::AliveMobs[0].x = x1;
            mobs::AliveMobs[0].y = y1;
          }

          travel::TravelControl[tp.quad_id] = tp;
        }
        else {
          // we are at the destination. Remove tp from TravelControl
        
          if(travel::TravelControl.count(tp.quad_id) > 0)
          {
            std::cout << "removing " <<  tp.quad_id << " from Travel Control" << std::endl;  
            travel::TravelControl.erase(tp.quad_id);  
            std::cout << "removed" << std::endl;
          }
        }
    }

    void manage()
    {
       //std::cout << "travelling entities: " << travel::TravelControl.size() << std::endl;
       for (auto const& tp : travel::TravelControl)
       {  
           travel::go(tp.second);
       }
    }




}

#endif