
#ifndef TRAVEL_H
#define TRAVEL_H

namespace travel
{
    float get_distance_between_points(float a_x, float a_y, float b_x, float b_y)
    {
      float distance = std::sqrt(std::pow((a_x - b_x), 2) + std::pow((a_y-b_y), 2));
      return distance;
    }

    float get_angle_between_points(float a_x, float a_y, float b_x, float b_y)
    {
        float angle;
        bool lowering;
        float dx = a_x - b_x;
        float dy = a_y - b_y;

        if(dx == 0){
            if(dy == 0){
                angle = 0;
            } else {
                if(a_y > b_y){
                    angle = 0;
                } else {
                    angle = 180;
                }
            }
        } else if (dy == 0){
            if(a_x < b_x){
                angle = 90;
            } else {
                angle = 270;
            }
        } else {
            angle = atan(dy / dx) / 3.14 * 180;
            if(b_y>a_y){
                lowering = true;
            } else {
                lowering = false;
            }
            if(((lowering) & (angle < 0)) | ((lowering == false) & (angle > 0))){
                angle += 270;
            } else {
                angle += 90;
            }
        }
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


    void go(travel::TravelPlan tp)
    {
        // if we are not at the target yet, we move
        if((tp.current_x != tp.target_x) & (tp.current_y != tp.target_y))
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

  

            }
       }
    }

    void manage()
    {
       for (auto const& tp : travel::TravelControl)
       {  
           travel::go(tp.second);
       }
    }




}

#endif