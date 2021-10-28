#include <vector>
#include "collisions.h"
#include "entity.h"
#include "maps.h"
#include <math.h>
#include "../dictionary.h"

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
  std::vector<collisions::DistanceToObject> get_entity_to_entity_distances(int entity_id)
  {
    // calculate distances between alive entity and quads of certain type
    // initialize instance of distances table
    std::vector<collisions::DistanceToObject> distances = {};
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
          DistanceToObject dto;
          dto.a_entity_id = entity_id;
          dto.object_id = k;
          dto.distance = dist;
          dto.limit = e_diag + v.diag;
          dto.is_near = false;
          dto.object_type = OBJECT_TYPE_ENTITY;

          if(dto.distance <= dto.limit)
          {
            dto.is_near = true;
          }
          distances.push_back(dto);
        }
      }
    }
    return distances;
  }


  std::vector<collisions::DistanceToObject> get_entity_to_map_distances(int entity_id)
  {
    // calculate distances between alive entity and quads of certain type
    // initialize instance of distances table
    std::vector<collisions::DistanceToObject> distances = {};
    float e_mid_x = entity::entities[entity_id].mid_x;
    float e_mid_y = entity::entities[entity_id].mid_y;
    float e_diag = entity::entities[entity_id].diag;

    if (maps::tiles.size() > 0)
    {
      for (auto const& [k, v]: maps::tiles)
      {
        // for any entity thats not argument entity id
        float dist = get_distance_between_points(v.mid_x, v.mid_y, e_mid_x, e_mid_y);
        DistanceToObject dto;
        dto.a_entity_id = entity_id;
        dto.object_id = k;
        dto.distance = dist;
        dto.limit = e_diag + v.diag;
        dto.is_near = false;
        dto.object_type = OBJECT_TYPE_MAP;

        if(dto.distance <= dto.limit)
        {
          dto.is_near = true;
        }
        distances.push_back(dto);
      }
    }
    return distances;
  }



    std::vector<colls::DistanceToObject> find_entity_broad_collisions(int entity_id = 0);
    {
      // std::cout << entity_quad_id << std::endl;
      std::vector<colls::DistanceBetweenPoints> map_distances;
      std::vector<colls::DistanceBetweenPoints> entity_distances;
      std::vector<colls::DistanceBetweenPoints> near_distances;

      if (entity_quad_id != -1)
      {

        map_distances = get_entity_to_map_distances(entity_id);
        entity_distances = get_entity_to_entity_distances(entity_id);
        if(menu::MenuQuads.size() > 0)
        {
          quad::AllQuads.insert(quad::AllQuads.end(), menu::MenuQuads.begin(), menu::MenuQuads.end());
        }

        distances = get_entity_to_entity_distances(entity_id);



        for(int d=0; d<distances.size(); d++)
        {
          if(distances[d].is_near)
          {
            near_distances.push_back(distances[d]);
          }
        }
      }
      return near_distances;
    }




}