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


  std::vector<collisions::DistanceToObject> get_entity_to_entity_distances(int entity_id)
  {
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


  std::vector<collisions::DistanceToObject> find_entity_broad_collisions(int entity_id = 0)
  {
    std::vector<collisions::DistanceToObject> near_distances;
    std::vector<collisions::DistanceToObject> map_distances = get_entity_to_map_distances(entity_id);
    std::vector<collisions::DistanceToObject> entity_distances = get_entity_to_entity_distances(entity_id);

    for(int d=0; d < map_distances.size(); d++)
    {
      if(map_distances[d].is_near)
      {
        near_distances.push_back(map_distances[d]);
      }
    }

    for(int d=0; d < entity_distances.size(); d++)
    {
      if(entity_distances[d].is_near)
      {
        near_distances.push_back(entity_distances[d]);
      }
    }
    return near_distances;
  }

  // TODO -> reimplement this method for entity
  void resolve_solid_collisions(std::vector<collisions::DistanceToObject> near_distances)
  {
    struct SolidLimits limits;


    for(int i=0; i<near_distances.size(); i++)
    {
      int qid = quads::find_quad_id(near_distances[i].b_quad_id, quads::AllQuads);
      if(quads::AllQuads[qid].solid)
      { 
        // loop through sensors
        for (auto const& s : quads::AllQuads[hid].sensors)
        {
          // hero entity on the left
          if((s.first == SENSOR_TOP_RIGHT || s.first == SENSOR_RIGHT || s.first == SENSOR_BOTTOM_RIGHT) && 
            (s.second.x >=  quads::AllQuads[qid].abs[AABB_FULL].min_x) && 
            (s.second.y >=  quads::AllQuads[qid].abs[AABB_FULL].min_y && s.second.y <= quads::AllQuads[qid].abs[AABB_FULL].max_y) &&
            (quads::AllQuads[hid].sensors[SENSOR_CENTER].x < quads::AllQuads[qid].abs[AABB_FULL].min_x)
          )
          {
             limits.right_borders.push_back(quads::AllQuads[qid].abs[AABB_FULL].min_x);
          }

          // hero entity on the right
          if((s.first == SENSOR_TOP_LEFT || s.first == SENSOR_LEFT || s.first == SENSOR_BOTTOM_LEFT) && 
            (s.second.x <=  quads::AllQuads[qid].abs[AABB_FULL].max_x) && 
            (s.second.y >=  quads::AllQuads[qid].abs[AABB_FULL].min_y && s.second.y <= quads::AllQuads[qid].abs[AABB_FULL].max_y) &&
            (quads::AllQuads[hid].sensors[SENSOR_CENTER].x > quads::AllQuads[qid].abs[AABB_FULL].max_x)
          )
          {
             limits.left_borders.push_back(quads::AllQuads[qid].abs[AABB_FULL].max_x);
          }

          // hero entity on the top
          if((s.first == SENSOR_BOTTOM_RIGHT || s.first == SENSOR_BOTTOM || s.first == SENSOR_BOTTOM_LEFT) && 
            (s.second.y >=  quads::AllQuads[qid].abs[AABB_FULL].min_y) && 
            (s.second.x >=  quads::AllQuads[qid].abs[AABB_FULL].min_x && s.second.x <= quads::AllQuads[qid].abs[AABB_FULL].max_x) &&
            (quads::AllQuads[hid].sensors[SENSOR_CENTER].y < quads::AllQuads[qid].abs[AABB_FULL].max_y)
          )
          {
             limits.bottom_borders.push_back(quads::AllQuads[qid].abs[AABB_FULL].min_y);
          }

          // hero entity on the bottom
          if((s.first == SENSOR_TOP_LEFT || s.first == SENSOR_TOP || s.first == SENSOR_TOP_RIGHT) && 
            (s.second.y <=  quads::AllQuads[qid].abs[AABB_FULL].max_y) && 
            (s.second.x >=  quads::AllQuads[qid].abs[AABB_FULL].min_x && s.second.x <= quads::AllQuads[qid].abs[AABB_FULL].max_x) &&
            (quads::AllQuads[hid].sensors[SENSOR_CENTER].y > quads::AllQuads[qid].abs[AABB_FULL].min_y)
          )
          {
             limits.top_borders.push_back(quads::AllQuads[qid].abs[AABB_FULL].max_y);
          }
        }
      }
    }

    // hero on the left     x |_|
    if (limits.right_borders.size() > 0)
    { 
      float min_x_border = *std::min_element(limits.right_borders.begin(), limits.right_borders.end());
      camera::x = camera::previous_x;
    } 
    // hero on the right   |_| x 
    if (limits.left_borders.size() > 0)
    { 
      float max_x_border = *std::max_element(limits.left_borders.begin(), limits.left_borders.end());
      camera::x = camera::previous_x;
    } 
    // hero on the top 
    if (limits.bottom_borders.size() > 0)
    { 
      float min_y_border = *std::min_element(limits.bottom_borders.begin(), limits.bottom_borders.end());
      camera::y = camera::previous_y;
    } 
    // hero on the bottom 
    if (limits.top_borders.size() > 0)
    { 
      float max_y_border = *std::min_element(limits.top_borders.begin(), limits.top_borders.end());
      camera::y = camera::previous_y;
    } 
    // need a bit smarter camera update than this, but it works, leaving for now
    quads::scale_move_quads(camera::x, camera::y, camera::zoom);
  }

}