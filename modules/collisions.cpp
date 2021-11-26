
#include <algorithm>
#include <math.h>
#include <vector>

#include "camera.h"
#include "collisions.h"
#include "entity.h"
#include "maps.h"
#include "utils.h"
#include "../dictionary.h"

// Note: I dont really like this implementation.
// 2 methods: one for entities and one for maps that just replace the table inside and also the if else in set_abs somehow bug me
// Need to refactor this later when I have better idea on how to use it


namespace collisions
{
  int SENSOR_COUNT = 9;
  int SENSOR_OFFSET = 1;
  int ABS_COUNT = 1;

  std::vector<collisions::DistanceToObject> _get_entity_to_entity_distances(int entity_id)
  {
    std::vector<collisions::DistanceToObject> distances = {};
    for (auto const& [k, v]: entity::entities)
    {
      if(k != entity_id)
      {
        float dist = utils2::get_distance_between_points(v.x, 
                                                 v.y, 
                                                 entity::entities[entity_id].x, 
                                                 entity::entities[entity_id].y);
        float dist_limit = entity::entities[entity_id].diag + v.diag;
        if(dist <= dist_limit)
        {
          DistanceToObject dto;
          dto.entity_id = entity_id;
          dto.object_id = k;
          dto.object_type = OBJECT_TYPE_ENTITY;
          dto.distance = dist;
          dto.limit = dist_limit;
          dto.is_solid = v.is_solid;
          dto.is_near = true;
          distances.push_back(dto);
        }
      }
    }
    return distances;
  }

  std::vector<collisions::DistanceToObject> _get_entity_to_map_distances(int entity_id)
  {
    std::vector<collisions::DistanceToObject> distances = {};
    for (auto const& [k, v]: maps2::tiles)
    {
      float dist = utils2::get_distance_between_points(v.x, 
                                                       v.y, 
                                                       entity::entities[entity_id].x, 
                                                       entity::entities[entity_id].y);
      float dist_limit = entity::entities[entity_id].diag + v.diag;
      if(dist <= dist_limit)
      {
        DistanceToObject dto;
        dto.entity_id = entity_id;
        dto.object_id = k;
        dto.object_type = OBJECT_TYPE_MAP;
        dto.distance = dist;
        dto.limit = dist_limit;
        dto.is_solid = v.is_solid;
        dto.is_near = true;
        distances.push_back(dto);
      }
    }
    return distances;
  }

  std::vector<collisions::DistanceToObject> _find_entity_broad_collisions(int entity_id = 0)
  {
    std::vector<collisions::DistanceToObject> near_distances = {};
    std::vector<collisions::DistanceToObject> map_near_distances = _get_entity_to_map_distances(entity_id);
    std::vector<collisions::DistanceToObject> entity_near_distances = _get_entity_to_entity_distances(entity_id);

    if(map_near_distances.size() > 0)
    {
      near_distances.insert(near_distances.end(), map_near_distances.begin(), map_near_distances.end());
    }
    if(entity_near_distances.size() > 0)
    {
      near_distances.insert(near_distances.end(), entity_near_distances.begin(), entity_near_distances.end());
    }
    return near_distances;
  }

  void _resolve_solid_collisions(std::vector<collisions::DistanceToObject>& near_distances)
  {
    struct SolidLimits limits;
    for(int i=0; i<near_distances.size(); i++)
    {
      if(near_distances[i].is_solid)
      {
        int entity_id = near_distances[i].entity_id;
        int object_id = near_distances[i].object_id;
        int object_type = near_distances[i].object_type;
        float sensor_center_x = entity::entities[entity_id].sensors[SENSOR_CENTER].x;
        float sensor_center_y = entity::entities[entity_id].sensors[SENSOR_CENTER].y;

        collisions::AABB aabb;
        // extract object type box
        if(object_type == OBJECT_TYPE_ENTITY)
        {
          aabb = entity::entities[object_id].abs[AABB_FULL];
        } else if (object_type == OBJECT_TYPE_MAP)
        {
          aabb = maps2::tiles[object_id].abs[AABB_FULL];
        }


        // loop through entity sensors
        for (auto const& [k, v]: entity::entities[entity_id].sensors)
        {
          // entity on the left
          if((k == SENSOR_TOP_RIGHT || k == SENSOR_RIGHT || k == SENSOR_BOTTOM_RIGHT) && 
            (v.x >=  aabb.min_x) && 
            (v.y >=  aabb.min_y && v.y <= aabb.max_y) &&
            (sensor_center_x < aabb.min_x)
          )
          {
            limits.right_borders.push_back(aabb.min_x);
          }

          // entity on the right
          if((k == SENSOR_TOP_LEFT || k == SENSOR_LEFT || k == SENSOR_BOTTOM_LEFT) && 
            (v.x <=  aabb.max_x) && 
            (v.y >=  aabb.min_y && v.y <= aabb.max_y) &&
            (sensor_center_x > aabb.max_x)
          )
          {
            limits.left_borders.push_back(aabb.max_x);
          }

          // entity on the top
          if((k == SENSOR_BOTTOM_RIGHT || k == SENSOR_BOTTOM || k == SENSOR_BOTTOM_LEFT) && 
            (v.y >=  aabb.min_y) && 
            (v.x >=  aabb.min_x && v.x <= aabb.max_x) &&
            (sensor_center_y < aabb.max_y)
          )
          {
            limits.bottom_borders.push_back(aabb.min_y);
          }

          // entity on the bottom
          if((k == SENSOR_TOP_LEFT || k == SENSOR_TOP || k == SENSOR_TOP_RIGHT) && 
            (v.y <=  aabb.max_y) && 
            (v.x >=  aabb.min_x && v.x <= aabb.max_x) &&
            (sensor_center_y > aabb.min_y)
          )
          {
            limits.top_borders.push_back(aabb.max_y);
          }
        }
      }
    }
    // resolve collisions
    // hero on the left     x |_|
    if (limits.right_borders.size() > 0)
    { 
      float min_x_border = *std::min_element(limits.right_borders.begin(), limits.right_borders.end());
      camera::cam.x = camera::cam.previous_x;
    } 
    // hero on the right   |_| x 
    if (limits.left_borders.size() > 0)
    { 
      float max_x_border = *std::max_element(limits.left_borders.begin(), limits.left_borders.end());
      camera::cam.x = camera::cam.previous_x;
    } 
    // hero on the top 
    if (limits.bottom_borders.size() > 0)
    { 
      float min_y_border = *std::min_element(limits.bottom_borders.begin(), limits.bottom_borders.end());
      camera::cam.y = camera::cam.previous_y;
    } 
    // hero on the bottom 
    if (limits.top_borders.size() > 0)
    { 
      float max_y_border = *std::min_element(limits.top_borders.begin(), limits.top_borders.end());
      camera::cam.y = camera::cam.previous_y;
    } 
  }

  void _set_sensors(int entity_id)
  {
    // entity::entities[entity_id].sensors.clear();
    // entity::entities.at(entity_id).sensors.clear();

    // NOTE: dont know if that really works
    // copied over just to search for it once
    entity::EntityData edd = entity::entities.at(entity_id);
    edd.sensors.clear();

    for(int i=0; i < collisions::SENSOR_COUNT; i++)
    {
      collisions::Sensor s;
      switch(i) {
        case SENSOR_TOP:
            s.x = edd.mid_x;
            s.y = (edd.y - collisions::SENSOR_OFFSET);
            s.id = SENSOR_TOP;
            edd.sensors.insert(std::pair<int, collisions::Sensor>(SENSOR_TOP, s));
        break;
        case SENSOR_TOP_RIGHT:
            s.x = (edd.x + edd.w + collisions::SENSOR_OFFSET);
            s.y = (edd.y  - collisions::SENSOR_OFFSET);
            s.id = SENSOR_TOP_RIGHT;
            edd.sensors.insert(std::pair<int, collisions::Sensor>(SENSOR_TOP_RIGHT, s));
        break;
        case SENSOR_RIGHT:
            s.x = (edd.x + edd.w  + collisions::SENSOR_OFFSET);
            s.y = (edd.y) ;
            s.id = SENSOR_RIGHT;
            edd.sensors.insert(std::pair<int, collisions::Sensor>(SENSOR_RIGHT, s));
        break;
        case SENSOR_BOTTOM_RIGHT:
            s.x = (edd.x + edd.w + collisions::SENSOR_OFFSET);
            s.y = (edd.y + edd.h + collisions::SENSOR_OFFSET);
            s.id = SENSOR_BOTTOM_RIGHT;
            edd.sensors.insert(std::pair<int, collisions::Sensor>(SENSOR_BOTTOM_RIGHT, s));
        break;
        case SENSOR_BOTTOM:
            s.x = edd.mid_x;
            s.y = (edd.y + edd.h + collisions::SENSOR_OFFSET);
            s.id = SENSOR_BOTTOM;
            edd.sensors.insert(std::pair<int, collisions::Sensor>(SENSOR_BOTTOM, s));
        break;
        case SENSOR_BOTTOM_LEFT:
            s.x = edd.x - collisions::SENSOR_OFFSET;
            s.y = (edd.y + edd.h + collisions::SENSOR_OFFSET);
            s.id = SENSOR_BOTTOM_LEFT;
            edd.sensors.insert(std::pair<int, collisions::Sensor>(SENSOR_BOTTOM_LEFT, s));
        break;
        case SENSOR_LEFT:
            s.x = edd.x - collisions::SENSOR_OFFSET ;
            s.y = edd.mid_y;
            s.id = SENSOR_LEFT;
            edd.sensors.insert(std::pair<int, collisions::Sensor>(SENSOR_LEFT, s));
        break;
        case SENSOR_TOP_LEFT:
            s.x = edd.x - collisions::SENSOR_OFFSET;
            s.y = edd.y - collisions::SENSOR_OFFSET;
            s.id = SENSOR_TOP_LEFT;
            edd.sensors.insert(std::pair<int, collisions::Sensor>(SENSOR_TOP_LEFT, s));
        break;
        case SENSOR_CENTER:
            s.x = edd.mid_x;
            s.y = edd.mid_y;
            s.id = SENSOR_CENTER;
            edd.sensors.insert(std::pair<int, collisions::Sensor>(SENSOR_CENTER, s));
        break;
      }  
    }
  }

  void _set_abs_entities(int entity_id)
  {
    entity::EntityData edd = entity::entities.at(entity_id);
    edd.abs.clear();
    for(int a = 0; a < collisions::ABS_COUNT; a++)
    {
      collisions::AABB aabb;
      aabb.min_x = (edd.x + collisions::SENSOR_OFFSET);
      aabb.min_y = (edd.y + collisions::SENSOR_OFFSET);
      aabb.max_y = (edd.y + edd.h - collisions::SENSOR_OFFSET);
      aabb.max_x = (edd.x + edd.w - collisions::SENSOR_OFFSET);
      aabb.id = AABB_FULL;
      edd.abs.insert(std::pair<int, collisions::AABB>(AABB_FULL, aabb));
    }
  }

  void _set_abs_maps(int tile_id)
  {
    maps2::TileData tdd = maps2::tiles.at(tile_id);
    tdd.abs.clear();
    for(int a = 0; a < collisions::ABS_COUNT; a++)
    {
      collisions::AABB aabb;
      aabb.min_x = (tdd.x + collisions::SENSOR_OFFSET);
      aabb.min_y = (tdd.y + collisions::SENSOR_OFFSET);
      aabb.max_y = (tdd.y + tdd.h - collisions::SENSOR_OFFSET);
      aabb.max_x = (tdd.x + tdd.w - collisions::SENSOR_OFFSET);
      aabb.id = AABB_FULL;
      tdd.abs.insert(std::pair<int, collisions::AABB>(AABB_FULL, aabb));
    }    
  }

  void _set_abs(std::vector<collisions::DistanceToObject>& near_distances)
  {
    for(int i = 0; i < near_distances.size(); i++)
    {
      if(near_distances[i].object_type == OBJECT_TYPE_ENTITY)
      {
        collisions::_set_abs_entities(near_distances[i].object_id);
      } else if (near_distances[i].object_type == OBJECT_TYPE_MAP)
      {
        collisions::_set_abs_maps(near_distances[i].object_id);
      }
    }
  }

  void handle_entity_collisions(int entity_id)
  {
    // for hero only currently
    std::vector<collisions::DistanceToObject> near_distances = collisions::find_entity_broad_collisions(entity_id);
    if(near_distances.size() > 0)
    {
      collisions::_set_sensors(entity_id);
      collisions::_set_abs(near_distances);
      collisions::_resolve_solid_collisions(near_distances);
    }
  }


}