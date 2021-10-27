#include <vector>
#include <map>

#ifndef MODULES_COLLISIONS_H
#define MODULES_COLLISIONS_H

namespace collisions
{

  extern int SENSOR_COUNT;
  extern int SENSOR_OFFSET;
  extern int ABS_COUNT;

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



#endif
