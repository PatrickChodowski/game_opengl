

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


