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

  // AABB data for the entity
  struct AABB
  {
    int min_x;
    int min_y;
    int max_x;
    int max_y;
    int id;
  };

  // Sensors assigned to entity
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

  // Data. stores information about distance between entity and some other object (can be entity or map etc.). Used for collision detection
  struct DistanceToObject
  {
    int a_entity_id;
    int object_id; // can be map object, can be entity
    int object_type; //map or entity

    float distance;
    float limit; // sum of entity diagonals
    bool is_near;
  };


  // Calculates distance between two points
  float get_distance_between_points(float x1, float y1, float x2, float y2);

  // Gets vector of entity distances to other entities
  std::vector<collisions::DistanceToObject> get_entity_to_entity_distances(int entity_id);

  // Gets vector of entity distances to map objects
  std::vector<collisions::DistanceToObject> get_entity_to_map_distances(int entity_id);

  // Find broad collisions for entity (by default its the hero, but lets leave the option)
  std::vector<collisions::DistanceToObject> find_entity_broad_collisions(int entity_id);

  // Analyze solid collisions from near distances from find_entity_broad_collisions
  void resolve_solid_collisions(std::vector<collisions::DistanceToObject near_distances);

}



#endif
