
#include "../modules/collisions.h"
#include "../modules/ecs.h"
#include "../modules/hero.h"
#include "../modules/items.h"
#include "../modules/maps.h"
#include "../dictionary.h"
#include "../dependencies/parallel_hashmap/phmap.h"

bool test_collisions__set_sensors_on_entity_count()
{
  bool passed = false;

  // preparation: create hero entity
  hero::create_new("testhero", "1_barbarian", 450, 370);

  // set sensors on hero entity
  collisions::_set_sensors(hero::HERO_ENTITY_ID);

  // collisions::SENSOR_COUNT should be 9
  if(ecs::sensors.at(hero::HERO_ENTITY_ID).sensors.size() == collisions::SENSOR_COUNT){
    passed = true;
  }
  return passed;
}

bool test_collisions__set_sensors_on_entity_top()
{
  bool passed = false;

  // preparation: create hero entity
  hero::create_new("testhero", "1_barbarian", 450, 370);
  // set sensors on hero entity
  collisions::_set_sensors(hero::HERO_ENTITY_ID);
  if(
    (ecs::sensors.at(hero::HERO_ENTITY_ID).sensors[SENSOR_TOP].x == ecs::collisions.at(hero::HERO_ENTITY_ID).mid_x) &
    (ecs::sensors.at(hero::HERO_ENTITY_ID).sensors[SENSOR_TOP].y == (ecs::collisions.at(hero::HERO_ENTITY_ID).mid_y - ecs::collisions.at(hero::HERO_ENTITY_ID).radius_y))
  )
  {
    passed = true;
  }
  return passed;
}

bool test_collisions__set_sensors_on_entity_bottomleft()
{
  bool passed = false;

  // preparation: create hero entity
  hero::create_new("testhero", "1_barbarian", 450, 370);

  // set sensors on hero entity
  collisions::_set_sensors(hero::HERO_ENTITY_ID);
  if(
    (ecs::sensors.at(hero::HERO_ENTITY_ID).sensors[SENSOR_BOTTOM_LEFT].x == (ecs::collisions.at(hero::HERO_ENTITY_ID).mid_x - ecs::collisions.at(hero::HERO_ENTITY_ID).radius_x)) &
    (ecs::sensors.at(hero::HERO_ENTITY_ID).sensors[SENSOR_BOTTOM_LEFT].y == (ecs::collisions.at(hero::HERO_ENTITY_ID).mid_y + ecs::collisions.at(hero::HERO_ENTITY_ID).radius_y))
  )
  {
    passed = true;
  }
  return passed;
}

bool test_collisions__set_sensors_on_entity_count_after_update()
{
  bool passed = false;

  // preparation: create hero entity
  hero::create_new("testhero", "1_barbarian", 450, 370);

  // set sensors on hero entity
  collisions::_set_sensors(hero::HERO_ENTITY_ID);

  // change position, set sensors again
  ecs::set_position(hero::HERO_ENTITY_ID, 600, 450);
  collisions::_set_sensors(hero::HERO_ENTITY_ID);

  // collisions::SENSOR_COUNT should be 9
  if(ecs::sensors.at(hero::HERO_ENTITY_ID).sensors.size() == collisions::SENSOR_COUNT)
  {
    passed = true;
  }
  return passed;
}

bool test_collisions__set_sensors_on_entity_top_after_update()
{
  bool passed = false;

  // preparation: create hero entity
  hero::create_new("testhero", "1_barbarian", 450, 370);

  // set sensors on hero entity
  collisions::_set_sensors(hero::HERO_ENTITY_ID);
  // change position, set sensors again
  ecs::set_position(hero::HERO_ENTITY_ID, 600, 450);
  collisions::_set_sensors(hero::HERO_ENTITY_ID);
  if(
    ((ecs::sensors.at(hero::HERO_ENTITY_ID).sensors[SENSOR_TOP].x == ecs::collisions.at(hero::HERO_ENTITY_ID).mid_x)) &
    (ecs::sensors.at(hero::HERO_ENTITY_ID).sensors[SENSOR_TOP].y == (ecs::collisions.at(hero::HERO_ENTITY_ID).mid_y - 
                                                                     ecs::collisions.at(hero::HERO_ENTITY_ID).radius_y))
  )
  {
    passed = true;
  }
  return passed;
}

bool test_collisions__check_far_distance()
{
  ecs::clear();
  collisions::clear();
  bool passed = false;

  hero::create_new("testhero", "1_barbarian", 300, 300);

  ecs::TempEntityData e;
  e.name = "test_entity";
  e.components = {COMPONENT_POSITION, COMPONENT_COLLISIONS};
  e.entity_type_id = ENTITY_TYPE_COLOR;

  e.x = 100;
  e.y = 100;
  e.z = 0.5;
  e.w = 100;
  e.h = 100;
  e.radius_x = 30;
  e.radius_y = 30;
  e.is_solid = true;

  int entity_id = ecs::create_entity(&e);
  collisions::_get_entity_to_entity_distance(hero::HERO_ENTITY_ID, entity_id);

  if(collisions::distances.size() == 0){
    passed = true;
  }
  return passed;
}

bool test_collisions__check_near_distance()
{
  ecs::clear();
  collisions::clear();
  bool passed = true;

  hero::create_new("testhero", "1_barbarian", 100, 100);

  ecs::TempEntityData e;
  e.name = "test_entity";
  e.components = {COMPONENT_POSITION, COMPONENT_COLLISIONS};
  e.entity_type_id = ENTITY_TYPE_COLOR;

  e.x = 100;
  e.y = 100;
  e.z = 0.5;
  e.w = 100;
  e.h = 100;
  e.radius_x = 30;
  e.radius_y = 30;
  e.is_solid = true;
  // entity mid: 150, 150, hero mid: 200, 200
  // 70.7107

  int entity_id = ecs::create_entity(&e);
  collisions::_get_entity_to_entity_distance(hero::HERO_ENTITY_ID, entity_id);

  if(collisions::distances.size() == 0){
    std::cout << " [ERROR] No distance added " << std::endl;
    return false;
  }

  collisions::DistanceToObject dto = collisions::distances[0];
  if(!(dto.distance >= 70 & dto.distance <= 71)){
    std::cout << " [ERROR] Wrong collision distance: " << dto.distance << std::endl;
    return false;
  }

  if(!dto.is_solid){
    std::cout << " [ERROR] Collision not solid " << std::endl;
    return false;
  }


  return passed;
}

bool test_collisions__check_collision_item()
{
  ecs::clear();
  collisions::clear();
  bool passed = true;

  hero::create_new("testhero", "1_barbarian", 100, 100);

  ecs::TempEntityData e;
  e.name = "test_entity";
  e.components = {COMPONENT_POSITION, COMPONENT_COLLISIONS};
  e.entity_type_id = ENTITY_TYPE_COLOR;

  e.x = 100;
  e.y = 100;
  e.z = 0.5;
  e.w = 100;
  e.h = 100;
  e.radius_x = 30;
  e.radius_y = 30;
  e.is_solid = true;
  e.entity_type_id = ENTITY_TYPE_ITEM;
  // entity mid: 150, 150, hero mid: 200, 200
  // 70.7107

  int entity_id = ecs::create_entity(&e);
  collisions::_get_entity_to_entity_distance(hero::HERO_ENTITY_ID, entity_id);

  if(collisions::distances.size() == 0){
    std::cout << " [ERROR] No distance added " << std::endl;
    return false;
  }

  collisions::DistanceToObject dto = collisions::distances[0];
  int target_type = dto.target_entity_type_id;
  collisions::resolve[target_type](dto);

  if(items::near_items.size() == 0){
    std::cout << " [ERROR] Empty near items " << std::endl;
    return false;
  }

  return passed;
}
