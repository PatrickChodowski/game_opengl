#include <algorithm>
#include <iostream>
#include <math.h>
#include <vector>
#include "../dependencies/parallel_hashmap/phmap.h"

#include "camera.h"
#include "collisions.h"
#include "debug.h"
#include "ecs.h"
#include "game.h"
#include "hero.h"
#include "items.h"
#include "maps.h"
#include "utils.h"
#include "../dictionary.h"

namespace collisions
{
  int SENSOR_COUNT = 9;
  int SENSOR_OFFSET = 1;
  int ABS_COUNT = 1;
  std::vector<collisions::DistanceToObject> distances = {};
  phmap::flat_hash_map<int, sig_ptr> resolve;
  struct SolidLimits limits;

  void update()
  {
    collisions::clear();
    for(auto & [entity_id, sensors_data]: ecs::sensors)
    {
      collisions::_set_sensors(entity_id);
    }

    for(auto const& [entity_id, sensors_data]: ecs::sensors)
    {
      for(auto const& [collider_entity_id, collision_data]: ecs::collisions)
      {
        if(entity_id != collider_entity_id){
          collisions::_get_entity_to_entity_distance(entity_id, collider_entity_id);
        }
      }
    }

    for(int c=0; c<collisions::distances.size(); c++)
    {
      int target_type = collisions::distances[c].target_entity_type_id;
      collisions::resolve[target_type](collisions::distances[c]);
    }

    collisions::_use_limits();
  };

  void _get_entity_to_entity_distance(int entity_id, int target_entity_id)
  {
    ecs::CollisionsComponent target_point = ecs::collisions.at(target_entity_id);
    ecs::CollisionsComponent point = ecs::collisions.at(entity_id);
    float distance = utils::get_distance_between_points(target_point.mid_x,  target_point.mid_y, point.mid_x, point.mid_y);
    float distance_limit = point.radius + target_point.radius;
    if(distance <= distance_limit){
      collisions::DistanceToObject dto;
      dto.entity_id = entity_id;
      dto.target_entity_id = target_entity_id;
      dto.target_entity_type_id = ecs::entities.at(target_entity_id).entity_type_id;
      dto.distance = distance;
      dto.limit = distance_limit;
      dto.is_solid = ecs::collisions.at(target_entity_id).is_solid;
      collisions::distances.push_back(dto);
    }
  }

  void _use_limits()
  {
    // entity on the left     x |_|
    if (collisions::limits.right_borders.size() > 0){ 
      float min_x_border = *std::min_element(collisions::limits.right_borders.begin(), collisions::limits.right_borders.end());
      camera::cam.x = camera::cam.previous_x;
      ecs::revert_position_x(hero::HERO_ENTITY_ID);
    } 
    // entity on the right   |_| x 
    if (collisions::limits.left_borders.size() > 0){ 
      float max_x_border = *std::max_element(collisions::limits.left_borders.begin(), collisions::limits.left_borders.end());
      camera::cam.x = camera::cam.previous_x;
      ecs::revert_position_x(hero::HERO_ENTITY_ID);
    } 
    // entity on the top 
    if (collisions::limits.bottom_borders.size() > 0){ 
      float min_y_border = *std::min_element(collisions::limits.bottom_borders.begin(), collisions::limits.bottom_borders.end());
      camera::cam.y = camera::cam.previous_y;
      ecs::revert_position_y(hero::HERO_ENTITY_ID);
    } 
    // entity on the bottom 
    if (collisions::limits.top_borders.size() > 0){ 
      float max_y_border = *std::min_element(collisions::limits.top_borders.begin(), collisions::limits.top_borders.end());
      camera::cam.y = camera::cam.previous_y;
      ecs::revert_position_y(hero::HERO_ENTITY_ID);
    } 
  }

  void _set_sensors(int entity_id)
  {
    ecs::sensors.at(entity_id).sensors.clear();
    ecs::PositionComponent pos = ecs::positions.at(entity_id);
    ecs::CollisionsComponent cols = ecs::collisions.at(entity_id);

    for(int i=0; i < collisions::SENSOR_COUNT; i++)
    {
      collisions::Sensor s;
      switch(i) {
        case SENSOR_TOP:
            s.x = cols.mid_x;
            s.y = (pos.y - collisions::SENSOR_OFFSET);
            s.id = SENSOR_TOP;
            ecs::sensors.at(entity_id).sensors.insert(std::pair<int, collisions::Sensor>(SENSOR_TOP, s));
        break;
        case SENSOR_TOP_RIGHT:
            s.x = (pos.x + pos.w + collisions::SENSOR_OFFSET);
            s.y = (pos.y  - collisions::SENSOR_OFFSET);
            s.id = SENSOR_TOP_RIGHT;
            ecs::sensors.at(entity_id).sensors.insert(std::pair<int, collisions::Sensor>(SENSOR_TOP_RIGHT, s));
        break;
        case SENSOR_RIGHT:
            s.x = (pos.x + pos.w  + collisions::SENSOR_OFFSET);
            s.y = (pos.y) ;
            s.id = SENSOR_RIGHT;
            ecs::sensors.at(entity_id).sensors.insert(std::pair<int, collisions::Sensor>(SENSOR_RIGHT, s));
        break;
        case SENSOR_BOTTOM_RIGHT:
            s.x = (pos.x + pos.w + collisions::SENSOR_OFFSET);
            s.y = (pos.y + pos.h + collisions::SENSOR_OFFSET);
            s.id = SENSOR_BOTTOM_RIGHT;
            ecs::sensors.at(entity_id).sensors.insert(std::pair<int, collisions::Sensor>(SENSOR_BOTTOM_RIGHT, s));
        break;
        case SENSOR_BOTTOM:
            s.x = cols.mid_x;
            s.y = (pos.y + pos.h + collisions::SENSOR_OFFSET);
            s.id = SENSOR_BOTTOM;
            ecs::sensors.at(entity_id).sensors.insert(std::pair<int, collisions::Sensor>(SENSOR_BOTTOM, s));
        break;
        case SENSOR_BOTTOM_LEFT:
            s.x = pos.x - collisions::SENSOR_OFFSET;
            s.y = (pos.y + pos.h + collisions::SENSOR_OFFSET);
            s.id = SENSOR_BOTTOM_LEFT;
            ecs::sensors.at(entity_id).sensors.insert(std::pair<int, collisions::Sensor>(SENSOR_BOTTOM_LEFT, s));
        break;
        case SENSOR_LEFT:
            s.x = pos.x - collisions::SENSOR_OFFSET ;
            s.y = cols.mid_y;
            s.id = SENSOR_LEFT;
            ecs::sensors.at(entity_id).sensors.insert(std::pair<int, collisions::Sensor>(SENSOR_LEFT, s));
        break;
        case SENSOR_TOP_LEFT:
            s.x = pos.x - collisions::SENSOR_OFFSET;
            s.y = pos.y - collisions::SENSOR_OFFSET;
            s.id = SENSOR_TOP_LEFT;
            ecs::sensors.at(entity_id).sensors.insert(std::pair<int, collisions::Sensor>(SENSOR_TOP_LEFT, s));
        break;
        case SENSOR_CENTER:
            s.x = cols.mid_x;
            s.y = cols.mid_y;
            s.id = SENSOR_CENTER;
            ecs::sensors.at(entity_id).sensors.insert(std::pair<int, collisions::Sensor>(SENSOR_CENTER, s));
        break;
      }  
    }
  }

  void _set_abs_entity(int entity_id)
  {
    ecs::collisions.at(entity_id).abs.clear();
    ecs::PositionComponent pos = ecs::positions.at(entity_id);
    for(int a = 0; a < collisions::ABS_COUNT; a++)
    {
      collisions::AABB aabb;
      aabb.min_x = (pos.x + collisions::SENSOR_OFFSET);
      aabb.min_y = (pos.y + collisions::SENSOR_OFFSET);
      aabb.max_y = (pos.y + pos.h - collisions::SENSOR_OFFSET);
      aabb.max_x = (pos.x + pos.w - collisions::SENSOR_OFFSET);
      aabb.id = AABB_FULL;
      ecs::collisions.at(entity_id).abs.insert(std::pair<int, collisions::AABB>(AABB_FULL, aabb));

      if(game::IS_DEBUG_MODE){
        ecs::PositionComponent pos = ecs::positions.at(entity_id);
        debug::render_square(pos.x, pos.y, pos.w, pos.h, 0.6, 0.6, 0.0, 1.0);
      }
    }
  }

  void init()
  {
    collisions::resolve[ENTITY_TYPE_ITEM] = _resolve_items;
    collisions::resolve[ENTITY_TYPE_DOOR] = _resolve_doors;
    collisions::resolve[ENTITY_TYPE_LIVE] = _resolve_solid;
    std::cout << "Collisions Initialized" << std::endl;
  }

  void clear()
  {
    collisions::distances.clear();
    collisions::limits.bottom_borders.clear();
    collisions::limits.top_borders.clear();
    collisions::limits.left_borders.clear();
    collisions::limits.right_borders.clear();
    items::near_items.clear();
  }

  void _resolve_doors(collisions::DistanceToObject &dto)
  {
    std::cout << " [COLLISIONS] Resolving doors" << std::endl;
    // for(int i=0; i<collisions::door_distances.size(); i++)
    // {
    //   int dest_scene_id = maps::maps[game::MAP_ID].doors[collisions::door_distances[i].object_id].dest_scene_id;
    //   game::HERO_START_X = maps::maps[game::MAP_ID].doors[collisions::door_distances[i].object_id].player_enter_x;
    //   game::HERO_START_Y = maps::maps[game::MAP_ID].doors[collisions::door_distances[i].object_id].player_enter_y;
    //   std::cout << "Switching level to " << dest_scene_id << std::endl;
    //   game::switch_scene(dest_scene_id, SCENE_LOAD_CHANGE_LEVEL);
    // }
  };

  void _resolve_items(collisions::DistanceToObject &dto)
  {
    // std::cout << " [COLLISIONS] Resolving items - Adding " << dto.target_entity_id << " to near iteams" << std::endl;
    items::near_items.push_back(dto.target_entity_id);
  };

  void _resolve_solid(collisions::DistanceToObject &dto)
  {
    //std::cout << " [COLLISIONS] Resolving solid" << std::endl;

    collisions::_set_abs_entity(dto.target_entity_id);
    if(dto.is_solid){    
      float sensor_center_x = ecs::sensors.at(dto.entity_id).sensors[SENSOR_CENTER].x;
      float sensor_center_y = ecs::sensors.at(dto.entity_id).sensors[SENSOR_CENTER].y;
      collisions::AABB aabb = ecs::collisions.at(dto.target_entity_id).abs[AABB_FULL];

      for (auto const& [k, v]: ecs::sensors.at(dto.entity_id).sensors)
      {
        // entity on the left
        if((k == SENSOR_TOP_RIGHT || k == SENSOR_RIGHT || k == SENSOR_BOTTOM_RIGHT) && 
          (v.x >=  aabb.min_x) && 
          (v.y >=  aabb.min_y && v.y <= aabb.max_y) &&
          (sensor_center_x < aabb.min_x)){
          collisions::limits.right_borders.push_back(aabb.min_x);
        }

        // entity on the right
        if((k == SENSOR_TOP_LEFT || k == SENSOR_LEFT || k == SENSOR_BOTTOM_LEFT) && 
          (v.x <=  aabb.max_x) && 
          (v.y >=  aabb.min_y && v.y <= aabb.max_y) &&
          (sensor_center_x > aabb.max_x)){
          collisions::limits.left_borders.push_back(aabb.max_x);
        }

        // entity on the top
        if((k == SENSOR_BOTTOM_RIGHT || k == SENSOR_BOTTOM || k == SENSOR_BOTTOM_LEFT) && 
          (v.y >=  aabb.min_y) && 
          (v.x >=  aabb.min_x && v.x <= aabb.max_x) &&
          (sensor_center_y < aabb.max_y)){
          collisions::limits.bottom_borders.push_back(aabb.min_y);
        }

        // entity on the bottom
        if((k == SENSOR_TOP_LEFT || k == SENSOR_TOP || k == SENSOR_TOP_RIGHT) && 
          (v.y <=  aabb.max_y) && 
          (v.x >=  aabb.min_x && v.x <= aabb.max_x) &&
          (sensor_center_y > aabb.min_y)){
          collisions::limits.top_borders.push_back(aabb.max_y);
        }
      }
    }
  }
}
