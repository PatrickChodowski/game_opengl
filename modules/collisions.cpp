#include <vector>
#include "collisions.h"
#include "entity.h"
#include <math.h>

namespace collisions
{
  int SENSOR_COUNT = 9;
  int SENSOR_OFFSET = 1;
  int ABS_COUNT = 1;

  float get_distance_between_points(float x1, float y1, float x2, float y2)
  {
    float distance = std::sqrt(std::pow((x2 - x1), 2) + std::pow((y2-y1), 2));
    return distance;
  }


  // REDO-> isntead of using quads, use entity and map tables
  std::vector<collisions::DistanceBetweenEntities> get_entity_distances(int entity_id)
  {
    // calculate distances between alive entity and quads of certain type
    // initialize instance of distances table
    std::vector<collisions::DistanceBetweenEntities> distances = {};
    float e_x = entity::entities[entity_id].x;
    float e_y = entity::entities[entity_id].y;
    float e_diag = entity::entities[entity_id].diag;

    if (entity::entities.size() > 1)
    {
      for (auto const& [k, v]: entity::entities)
      {
        // for any entity thats not argument entity id
        if((k != entity_id))
        {
          float dist = get_distance_between_points(v.x, v.y, e_x, e_y);
          DistanceBetweenEntities dbe;
          dbe.a_entity_id = entity_id;
          dbe.b_entity_id = k;
          dbe.distance = dist;
          dbe.limit = e_diag + v.diag;
          dbe.is_near = false;

          if(dbe.distance <= dbe.limit)
          {
            dbe.is_near = true;
          }
          distances.push_back(dbe);
        }
      }
    }
    return distances;
  }

}