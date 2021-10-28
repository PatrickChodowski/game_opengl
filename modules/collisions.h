#include <vector>
#include <map>

#ifndef MODULES_COLLISIONS_H
#define MODULES_COLLISIONS_H

// Frame data. Table of distances between points for entity hero is per frame
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

  // [Old] Quad based distance
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

  // Data. stores information about distance between 2 entities. Used for collision detection
  struct DistanceBetweenEntities
  {
    int a_entity_id;
    int b_entity_id;
    float distance;
    float limit; // sum of entity diagonals
    bool is_near;
  };


  // Calculates distance between two points
  float get_distance_between_points(float x1, float y1, float x2, float y2);

  // Gets vector of entity distances to other entities
  std::vector<collisions::DistanceBetweenEntities> get_entity_distances(int entity_id);

}



#endif
