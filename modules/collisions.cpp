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
#include "maps.h"
#include "utils.h"
#include "../dictionary.h"

namespace collisions
{
  int SENSOR_COUNT = 9;
  int SENSOR_OFFSET = 1;
  int ABS_COUNT = 1;
  std::vector<collisions::DistanceToObject> distances = {};
  std::vector<int> near_items = {};


  void update()
  {
    // For each entity with sensors
    for (auto const& [entity_id, sensors_data] : ecs::sensors)
    {

    }

  };


  collisions::DistanceToObject _get_entity_to_entity_distance(int entity_id, int target_entity_id)
  {
    ecs::PositionComponent target_pos = ecs::positions.at(target_entity_id);
    ecs::PositionComponent pos = ecs::positions.at(entity_id);

    float dist = utils::get_distance_between_points(target_pos.x, 
                                                    target_pos.y, 
                                                    pos.x, 
                                                    pos.y);
    float dist_limit = ecs::collisions.at(entity_id).diag + ecs::collisions.at(target_entity_id).diag;
    collisions::DistanceToObject dto;
    dto.entity_id = entity_id;
    dto.object_id = target_entity_id;
    dto.object_type = ENTITY_TYPE_LIVE;
    dto.distance = dist;
    dto.limit = dist_limit;
    dto.is_solid = ecs::collisions.at(target_entity_id).is_solid;
    dto.is_near = false;
    if(dist <= dist_limit){
      dto.is_near = true;
    }
    return dto;
  }

  void get_distances(int entity_id)
  {
    collisions::clear();
    for (auto const& [target_entity_id, collision_data]: ecs::collisions)
    {
      if(target_entity_id != entity_id){
        collisions::DistanceToObject dto = _get_entity_to_entity_distance(entity_id, target_entity_id);
        if(dto.is_near){
          collisions::distances.push_back(dto);
        }
      }
    }
  }

  void _resolve_solid_collisions()
  {
    struct SolidLimits limits;
    int entity_id;
    for(int i=0; i<collisions::distances.size(); i++)
    {
      if(collisions::distances[i].object_type == ENTITY_TYPE_LIVE){
        if(ecs::entities.at(collisions::distances[i].object_id).entity_type_id == ENTITY_TYPE_ITEM){
          collisions::near_items.push_back(collisions::distances[i].object_id);
        }
      }

      if(collisions::distances[i].is_solid){

        entity_id = collisions::distances[i].entity_id;
        int object_id = collisions::distances[i].object_id;
        int object_type = collisions::distances[i].object_type;

        float sensor_center_x = ecs::sensors.at(entity_id).sensors[SENSOR_CENTER].x;
        float sensor_center_y = ecs::sensors.at(entity_id).sensors[SENSOR_CENTER].y;

        // extract AABB box
        collisions::AABB aabb = ecs::collisions.at(object_id).abs[AABB_FULL];

        // loop through entity sensors
        for (auto const& [k, v]: ecs::sensors.at(entity_id).sensors)
        {
          // entity on the left
          if((k == SENSOR_TOP_RIGHT || k == SENSOR_RIGHT || k == SENSOR_BOTTOM_RIGHT) && 
            (v.x >=  aabb.min_x) && 
            (v.y >=  aabb.min_y && v.y <= aabb.max_y) &&
            (sensor_center_x < aabb.min_x)){
            limits.right_borders.push_back(aabb.min_x);
          }

          // entity on the right
          if((k == SENSOR_TOP_LEFT || k == SENSOR_LEFT || k == SENSOR_BOTTOM_LEFT) && 
            (v.x <=  aabb.max_x) && 
            (v.y >=  aabb.min_y && v.y <= aabb.max_y) &&
            (sensor_center_x > aabb.max_x)){
            limits.left_borders.push_back(aabb.max_x);
          }

          // entity on the top
          if((k == SENSOR_BOTTOM_RIGHT || k == SENSOR_BOTTOM || k == SENSOR_BOTTOM_LEFT) && 
            (v.y >=  aabb.min_y) && 
            (v.x >=  aabb.min_x && v.x <= aabb.max_x) &&
            (sensor_center_y < aabb.max_y)){
            limits.bottom_borders.push_back(aabb.min_y);
          }

          // entity on the bottom
          if((k == SENSOR_TOP_LEFT || k == SENSOR_TOP || k == SENSOR_TOP_RIGHT) && 
            (v.y <=  aabb.max_y) && 
            (v.x >=  aabb.min_x && v.x <= aabb.max_x) &&
            (sensor_center_y > aabb.min_y)){
            limits.top_borders.push_back(aabb.max_y);
          }
        }
      }
    }
    // resolve collisions
    // entity on the left     x |_|
    if (limits.right_borders.size() > 0){ 
      float min_x_border = *std::min_element(limits.right_borders.begin(), limits.right_borders.end());
      camera::cam.x = camera::cam.previous_x;
      ecs::revert_position_x(entity_id);
    } 
    // entity on the right   |_| x 
    if (limits.left_borders.size() > 0){ 
      float max_x_border = *std::max_element(limits.left_borders.begin(), limits.left_borders.end());
      camera::cam.x = camera::cam.previous_x;
      ecs::revert_position_x(entity_id);
    } 
    // entity on the top 
    if (limits.bottom_borders.size() > 0){ 
      float min_y_border = *std::min_element(limits.bottom_borders.begin(), limits.bottom_borders.end());
      camera::cam.y = camera::cam.previous_y;
      ecs::revert_position_y(entity_id);
    } 
    // entity on the bottom 
    if (limits.top_borders.size() > 0){ 
      float max_y_border = *std::min_element(limits.top_borders.begin(), limits.top_borders.end());
      camera::cam.y = camera::cam.previous_y;
      ecs::revert_position_y(entity_id);
    } 
  }

  void _set_sensors(int entity_id)
  {
    ecs::sensors.at(entity_id).sensors.clear();
    ecs::PositionComponent pos = ecs::positions.at(entity_id);

    for(int i=0; i < collisions::SENSOR_COUNT; i++)
    {
      ecs::PositionComponent pos = ecs::positions.at(entity_id);
      ecs::MoveComponent mov = ecs::moves.at(entity_id);
      collisions::Sensor s;
      switch(i) {
        case SENSOR_TOP:
            s.x = mov.mid_x;
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
            s.x = mov.mid_x;
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
            s.y = mov.mid_y;
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
            s.x = mov.mid_x;
            s.y = mov.mid_y;
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

      // if(game::IS_DEBUG_MODE)
      // {
      //   debug::render_square(entity::entities.at(entity_id).pos.x, entity::entities.at(entity_id).pos.y, entity::entities.at(entity_id).dims.w, entity::entities.at(entity_id).dims.h, 0.6, 0.6, 0.0, 1.0);
      // }
    }
  }

  void init()
  {
    std::cout << "Collisions Initialized" << std::endl;
  }

  void _set_abs()
  {
    for(int i = 0; i < collisions::distances.size(); i++)
    {
      _set_abs_entity(collisions::distances[i].object_id);
    }
  }

  void handle_entity_collisions(int entity_id)
  {
    if(entity_id > -1)
    {
      collisions::get_distances(entity_id);
      if(collisions::distances.size() > 0)
      {
        collisions::_set_sensors(entity_id);
        collisions::_set_abs();
        collisions::_resolve_solid_collisions();
      }
      // if(collisions::door_distances.size()>0)
      // {
      //   collisions::_resolve_doors();
      // }
    }
  }

  void _resolve_doors()
  {
    // for(int i=0; i<collisions::door_distances.size(); i++)
    // {
    //   int dest_scene_id = maps::maps[game::MAP_ID].doors[collisions::door_distances[i].object_id].dest_scene_id;
    //   game::HERO_START_X = maps::maps[game::MAP_ID].doors[collisions::door_distances[i].object_id].player_enter_x;
    //   game::HERO_START_Y = maps::maps[game::MAP_ID].doors[collisions::door_distances[i].object_id].player_enter_y;
    //   std::cout << "Switching level to " << dest_scene_id << std::endl;
    //   game::switch_scene(dest_scene_id, SCENE_LOAD_CHANGE_LEVEL);
    // }
  }

  void clear()
  {
    collisions::distances.clear();
    //collisions::door_distances.clear();
    collisions::near_items.clear();
  }


}
