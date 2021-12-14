
#include <algorithm>
#include <iostream>
#include <math.h>
#include <vector>

#include "camera.h"
#include "collisions.h"
#include "debug.h"
#include "entity.h"
#include "game.h"
#include "hero.h"
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
  std::map <int,sig_ptr> AABBsHandler = {};
  std::vector<collisions::DistanceToObject> distances = {};
  std::vector<collisions::DistanceToObject> door_distances = {};



  collisions::DistanceToObject _get_entity_to_single_entity_distance(int entity_id, int target_entity_id)
  {
    float dist = utils::get_distance_between_points(entity::entities[target_entity_id].pos.x, 
                                                    entity::entities[target_entity_id].pos.y, 
                                                    entity::entities[entity_id].pos.x, 
                                                    entity::entities[entity_id].pos.y);
    float dist_limit = entity::entities[entity_id].diag + entity::entities[target_entity_id].diag;
    collisions::DistanceToObject dto;
    dto.entity_id = entity_id;
    dto.object_id = target_entity_id;
    dto.object_type = OBJECT_TYPE_ENTITY;
    dto.distance = dist;
    dto.limit = dist_limit;
    dto.is_solid = entity::entities[target_entity_id].is_solid;
    dto.is_near = false;
    if(dist <= dist_limit)
    {
      dto.is_near = true;
    }
    return dto;
  }

  void _get_entity_to_entity_distances(int entity_id)
  {
    for (auto const& [k, v]: entity::entities)
    {
      if(k != entity_id)
      {
        collisions::DistanceToObject dto = _get_entity_to_single_entity_distance(entity_id, k);
        if(dto.is_near)
        {
          collisions::distances.push_back(dto);
        }
      }
    }
  }

  collisions::DistanceToObject _get_entity_to_single_tile_distance(int entity_id, int tile_id)
  {
    float dist = utils::get_distance_between_points(maps::tiles[tile_id].pos.x, 
                                                    maps::tiles[tile_id].pos.y, 
                                                    entity::entities[entity_id].pos.x, 
                                                    entity::entities[entity_id].pos.y);
    float dist_limit = entity::entities[entity_id].diag + maps::tiles[tile_id].diag;
    collisions::DistanceToObject dto;
    dto.entity_id = entity_id;
    dto.object_id = tile_id;
    dto.object_type = OBJECT_TYPE_MAP;
    dto.distance = dist;
    dto.limit = dist_limit;
    dto.is_solid = maps::tiles[tile_id].is_solid;
    dto.is_near = false;
    if(dist <= dist_limit)
    {
      dto.is_near = true;
    }
    return dto;
  }


  void _get_entity_to_door_distances(int entity_id, int map_id)
  {
    for(int d = 0; d < maps::maps[map_id].doors.size(); d++)
    {
      collisions::DistanceToObject dto = _get_entity_to_door_distance(entity_id, map_id, d);
      if(dto.is_near)
      {
        std::cout << "close distance to door " << std::endl;
        collisions::door_distances.push_back(dto);
      }
    }
  };
  
  collisions::DistanceToObject _get_entity_to_door_distance(int entity_id, int map_id, int door_index)
  {
    float dist = utils::get_distance_between_points(maps::maps[map_id].doors[door_index].x,
                                                    maps::maps[map_id].doors[door_index].y,
                                                    entity::entities[entity_id].pos.x, 
                                                    entity::entities[entity_id].pos.y);
    float dist_limit = entity::entities[entity_id].diag;
    collisions::DistanceToObject dto;
    dto.entity_id = entity_id;
    dto.object_id = maps::maps[map_id].doors[door_index].door_id;
    dto.object_type = OBJECT_TYPE_DOOR;
    dto.distance = dist;
    dto.limit = dist_limit;
    dto.is_solid = false;
    dto.is_near = false;
    if(dist <= dist_limit)
    {
      dto.is_near = true;
    }
    return dto;
  }
  


  void _get_entity_to_map_distances(int entity_id)
  {
    std::vector<collisions::DistanceToObject> distances = {};
    for (auto const& [k, v]: maps::tiles)
    {
      if (v.is_solid)
      {
        collisions::DistanceToObject dto = _get_entity_to_single_tile_distance(entity_id, k);
        if(dto.is_near)
        {
          collisions::distances.push_back(dto);
        }
      }
    }
  }

  void _collect_near_distances(int entity_id = 0)
  {
    collisions::clear();
    collisions::_get_entity_to_map_distances(entity_id);
    collisions::_get_entity_to_entity_distances(entity_id);
    collisions::_get_entity_to_door_distances(entity_id, game::MAP_ID);
  }

  void _resolve_solid_collisions()
  {
    struct SolidLimits limits;
    for(int i=0; i<collisions::distances.size(); i++)
    {
      if(collisions::distances[i].is_solid)
      {
        int entity_id = collisions::distances[i].entity_id;
        int object_id = collisions::distances[i].object_id;
        int object_type = collisions::distances[i].object_type;
        float sensor_center_x = entity::entities[entity_id].sensors[SENSOR_CENTER].x;
        float sensor_center_y = entity::entities[entity_id].sensors[SENSOR_CENTER].y;

        collisions::AABB aabb;
        // extract object type box
        if(object_type == OBJECT_TYPE_ENTITY)
        {
          aabb = entity::entities[object_id].abs[AABB_FULL];
        } else if (object_type == OBJECT_TYPE_MAP)
        {
          aabb = maps::tiles[object_id].abs[AABB_FULL];
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
      hero::revert_position_x();
    } 
    // hero on the right   |_| x 
    if (limits.left_borders.size() > 0)
    { 
      float max_x_border = *std::max_element(limits.left_borders.begin(), limits.left_borders.end());
      camera::cam.x = camera::cam.previous_x;
      hero::revert_position_x();
    } 
    // hero on the top 
    if (limits.bottom_borders.size() > 0)
    { 
      float min_y_border = *std::min_element(limits.bottom_borders.begin(), limits.bottom_borders.end());
      camera::cam.y = camera::cam.previous_y;
      hero::revert_position_y();
    } 
    // hero on the bottom 
    if (limits.top_borders.size() > 0)
    { 
      float max_y_border = *std::min_element(limits.top_borders.begin(), limits.top_borders.end());
      camera::cam.y = camera::cam.previous_y;
      hero::revert_position_y();
    } 
  }

  void _set_sensors(int entity_id)
  {
    entity::entities[entity_id].sensors.clear();

    for(int i=0; i < collisions::SENSOR_COUNT; i++)
    {
      collisions::Sensor s;
      switch(i) {
        case SENSOR_TOP:
            s.x = entity::entities[entity_id].mid_x;
            s.y = (entity::entities[entity_id].pos.y - collisions::SENSOR_OFFSET);
            s.id = SENSOR_TOP;
            entity::entities[entity_id].sensors.insert(std::pair<int, collisions::Sensor>(SENSOR_TOP, s));
        break;
        case SENSOR_TOP_RIGHT:
            s.x = (entity::entities[entity_id].pos.x + entity::entities[entity_id].dims.w + collisions::SENSOR_OFFSET);
            s.y = (entity::entities[entity_id].pos.y  - collisions::SENSOR_OFFSET);
            s.id = SENSOR_TOP_RIGHT;
            entity::entities[entity_id].sensors.insert(std::pair<int, collisions::Sensor>(SENSOR_TOP_RIGHT, s));
        break;
        case SENSOR_RIGHT:
            s.x = (entity::entities[entity_id].pos.x + entity::entities[entity_id].dims.w  + collisions::SENSOR_OFFSET);
            s.y = (entity::entities[entity_id].pos.y) ;
            s.id = SENSOR_RIGHT;
            entity::entities[entity_id].sensors.insert(std::pair<int, collisions::Sensor>(SENSOR_RIGHT, s));
        break;
        case SENSOR_BOTTOM_RIGHT:
            s.x = (entity::entities[entity_id].pos.x + entity::entities[entity_id].dims.w + collisions::SENSOR_OFFSET);
            s.y = (entity::entities[entity_id].pos.y + entity::entities[entity_id].dims.h + collisions::SENSOR_OFFSET);
            s.id = SENSOR_BOTTOM_RIGHT;
            entity::entities[entity_id].sensors.insert(std::pair<int, collisions::Sensor>(SENSOR_BOTTOM_RIGHT, s));
        break;
        case SENSOR_BOTTOM:
            s.x = entity::entities[entity_id].mid_x;
            s.y = (entity::entities[entity_id].pos.y + entity::entities[entity_id].dims.h + collisions::SENSOR_OFFSET);
            s.id = SENSOR_BOTTOM;
            entity::entities[entity_id].sensors.insert(std::pair<int, collisions::Sensor>(SENSOR_BOTTOM, s));
        break;
        case SENSOR_BOTTOM_LEFT:
            s.x = entity::entities[entity_id].pos.x - collisions::SENSOR_OFFSET;
            s.y = (entity::entities[entity_id].pos.y + entity::entities[entity_id].dims.h + collisions::SENSOR_OFFSET);
            s.id = SENSOR_BOTTOM_LEFT;
            entity::entities[entity_id].sensors.insert(std::pair<int, collisions::Sensor>(SENSOR_BOTTOM_LEFT, s));
        break;
        case SENSOR_LEFT:
            s.x = entity::entities[entity_id].pos.x - collisions::SENSOR_OFFSET ;
            s.y = entity::entities[entity_id].mid_y;
            s.id = SENSOR_LEFT;
            entity::entities[entity_id].sensors.insert(std::pair<int, collisions::Sensor>(SENSOR_LEFT, s));
        break;
        case SENSOR_TOP_LEFT:
            s.x = entity::entities[entity_id].pos.x - collisions::SENSOR_OFFSET;
            s.y = entity::entities[entity_id].pos.y - collisions::SENSOR_OFFSET;
            s.id = SENSOR_TOP_LEFT;
            entity::entities[entity_id].sensors.insert(std::pair<int, collisions::Sensor>(SENSOR_TOP_LEFT, s));
        break;
        case SENSOR_CENTER:
            s.x = entity::entities[entity_id].mid_x;
            s.y = entity::entities[entity_id].mid_y;
            s.id = SENSOR_CENTER;
            entity::entities[entity_id].sensors.insert(std::pair<int, collisions::Sensor>(SENSOR_CENTER, s));
        break;
      }  
    }
  }

  void _set_abs_entities(int entity_id)
  {
    entity::entities[entity_id].abs.clear();
    for(int a = 0; a < collisions::ABS_COUNT; a++)
    {
      collisions::AABB aabb;
      aabb.min_x = (entity::entities[entity_id].pos.x + collisions::SENSOR_OFFSET);
      aabb.min_y = (entity::entities[entity_id].pos.y + collisions::SENSOR_OFFSET);
      aabb.max_y = (entity::entities[entity_id].pos.y + entity::entities[entity_id].dims.h - collisions::SENSOR_OFFSET);
      aabb.max_x = (entity::entities[entity_id].pos.x + entity::entities[entity_id].dims.w - collisions::SENSOR_OFFSET);
      aabb.id = AABB_FULL;
      entity::entities[entity_id].abs.insert(std::pair<int, collisions::AABB>(AABB_FULL, aabb));

      if(game::IS_DEBUG_MODE)
      {
        debug::render_square(entity::entities[entity_id].pos.x, entity::entities[entity_id].pos.y, entity::entities[entity_id].dims.w, entity::entities[entity_id].dims.h, 0.6, 0.6, 0.0, 1.0);
      }
    }
  }

  void _set_abs_maps(int tile_id)
  {
    maps::tiles[tile_id].abs.clear();
    for(int a = 0; a < collisions::ABS_COUNT; a++)
    {
      collisions::AABB aabb;
      aabb.min_x = (maps::tiles[tile_id].pos.x + collisions::SENSOR_OFFSET);
      aabb.min_y = (maps::tiles[tile_id].pos.y + collisions::SENSOR_OFFSET);
      aabb.max_y = (maps::tiles[tile_id].pos.y + maps::tiles[tile_id].dims.h - collisions::SENSOR_OFFSET);
      aabb.max_x = (maps::tiles[tile_id].pos.x + maps::tiles[tile_id].dims.w - collisions::SENSOR_OFFSET);
      aabb.id = AABB_FULL;
      maps::tiles[tile_id].abs.insert(std::pair<int, collisions::AABB>(AABB_FULL, aabb));

      if(game::IS_DEBUG_MODE)
      {
        debug::render_square(maps::tiles[tile_id].pos.x, maps::tiles[tile_id].pos.y, maps::tiles[tile_id].dims.w, maps::tiles[tile_id].dims.h, 0.6, 0.6, 0.0, 1.0);
      }
    }    
  }

  void init()
  {
    collisions::AABBsHandler[OBJECT_TYPE_ENTITY] = _set_abs_entities;
    collisions::AABBsHandler[OBJECT_TYPE_MAP] = _set_abs_maps;
  }


  void _set_abs()
  {
    for(int i = 0; i < collisions::distances.size(); i++)
    {
      collisions::AABBsHandler[collisions::distances[i].object_type](collisions::distances[i].object_id);
    }
  }

  void handle_entity_collisions(int entity_id)
  {
    collisions::_collect_near_distances(entity_id);
    if(collisions::distances.size() > 0)
    {
      collisions::_set_sensors(entity_id);
      collisions::_set_abs();
      collisions::_resolve_solid_collisions();
    }
    if(collisions::door_distances.size()>0)
    {
      collisions::_resolve_doors();
    }
  }

  void _resolve_doors()
  {
    for(int i=0; i<collisions::door_distances.size(); i++)
    {
      int dest_scene_id = maps::maps[game::MAP_ID].doors[collisions::door_distances[i].object_id].dest_scene_id;
      game::switch_scene(dest_scene_id, false);
    }
  }


  void clear()
  {
    collisions::distances.clear();
    collisions::door_distances.clear();
  }


}