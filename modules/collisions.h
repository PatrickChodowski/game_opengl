
#include <vector>
#include "../dependencies/parallel_hashmap/phmap.h"


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


  // Data. stores information about distance between entity and some other object (can be entity or map etc.). Used for collision detection
  struct DistanceToObject
  {
    int entity_id;
    int target_entity_id;
    int target_entity_type_id; // map, live, item, door

    float distance;
    float limit; // sum of entity diagonals
    bool is_solid;
  };


  typedef void (*sig_ptr)(collisions::DistanceToObject &dto);
  // Resolve collision functor
  extern phmap::flat_hash_map<int, sig_ptr> resolve;


  extern std::vector<collisions::DistanceToObject> distances;

  // Update collisions each frame
  void update();

  // Gets distance from entity to single entity object
  void _get_entity_to_entity_distance(int entity_id, int target_entity_id);

  // Find broad collisions for entity (by default its the hero, but lets leave the option)
  void get_distances(int entity_id);

  // Analyze solid collisions from near distances from find_entity_broad_collisions
  void _resolve_solid_collisions();

  // Sets sensors on selected entity
  void _set_sensors(int entity_id);

  // Set abs on entities
  void _set_abs_entity(int entity_id);
 
  // Sets AABBs boxes on objects flagged by near distances
  void _set_abs();

  // Fill resolve functor
  void init();

  // Clear distances data
  void clear();

  // Resolve doors -> go through doors if there are doors
  void _resolve_doors(collisions::DistanceToObject &dto);

  // Resolve items collision
  void _resolve_items(collisions::DistanceToObject &dto);

  // Resolve solid collision
  void _resolve_solid(collisions::DistanceToObject &dto);

}



#endif
