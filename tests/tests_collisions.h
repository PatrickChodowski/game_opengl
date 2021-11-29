

#include "../modules/collisions.h"
#include "../modules/entity.h"
#include "../modules/hero.h"
#include "../modules/items.h"
#include "../modules/maps.h"
#include "../dictionary.h"


bool test_collisions__set_sensors_on_entity_count()
{
  bool passed = false;

  // preparation: create hero entity
  hero::create_new("testhero", "barbarian");
  hero::set_position(450, 370);

  // set sensors on hero entity
  collisions::_set_sensors(hero::hero.entity_id);

  // entity::entities[hero::hero.entity_id].sensors.size()

  // collisions::SENSOR_COUNT should be 9
  if(entity::entities[hero::hero.entity_id].sensors.size() == collisions::SENSOR_COUNT)
  {
    passed = true;
  }
  return passed;
}

bool test_collisions__set_sensors_on_entity_top()
{
  bool passed = false;

  // preparation: create hero entity
  hero::create_new("testhero", "barbarian");
  hero::set_position(450, 370);

  // set sensors on hero entity
  collisions::_set_sensors(hero::hero.entity_id);
  if(
    (entity::entities[hero::hero.entity_id].sensors[SENSOR_TOP].x == entity::entities[hero::hero.entity_id].mid_x) &
    (entity::entities[hero::hero.entity_id].sensors[SENSOR_TOP].y == (entity::entities[hero::hero.entity_id].y  - collisions::SENSOR_OFFSET))
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
  hero::create_new("testhero", "barbarian");
  hero::set_position(450, 370);

  // set sensors on hero entity
  collisions::_set_sensors(hero::hero.entity_id);
  if(
    (entity::entities[hero::hero.entity_id].sensors[SENSOR_BOTTOM_LEFT].x == (entity::entities[hero::hero.entity_id].x- collisions::SENSOR_OFFSET)) &
    (entity::entities[hero::hero.entity_id].sensors[SENSOR_BOTTOM_LEFT].y == (entity::entities[hero::hero.entity_id].y + + entity::entities[hero::hero.entity_id].h + collisions::SENSOR_OFFSET))
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
  hero::create_new("testhero", "barbarian");
  hero::set_position(450, 370);

  // set sensors on hero entity
  collisions::_set_sensors(hero::hero.entity_id);

  // change position, set sensors again
  hero::set_position(600, 450);
  collisions::_set_sensors(hero::hero.entity_id);

  // collisions::SENSOR_COUNT should be 9
  if(entity::entities[hero::hero.entity_id].sensors.size() == collisions::SENSOR_COUNT)
  {
    passed = true;
  }
  return passed;
}


bool test_collisions__set_sensors_on_entity_top_after_update()
{
  bool passed = false;

  // preparation: create hero entity
  hero::create_new("testhero", "barbarian");
  hero::set_position(450, 370);

  // set sensors on hero entity
  collisions::_set_sensors(hero::hero.entity_id);

  // change position, set sensors again
  hero::set_position(600, 450);
  collisions::_set_sensors(hero::hero.entity_id);
  if(
    (entity::entities[hero::hero.entity_id].sensors[SENSOR_TOP].x == entity::entities[hero::hero.entity_id].mid_x) &
    (entity::entities[hero::hero.entity_id].sensors[SENSOR_TOP].y == (entity::entities[hero::hero.entity_id].y  - collisions::SENSOR_OFFSET))
  )
  {
    passed = true;
  }
  return passed;
}

bool test_collisions__get_entity_to_map_dto_solidpass()
{
  bool passed = false;

  // preparation: create hero entity
  hero::create_new("testhero", "barbarian");
  hero::set_position(450, 370);

  // Create tile
  maps::TileData tile = maps::generate_tile(200, 
                                            200, 
                                            2, //dungeon 
                                            11); //solid frame_id
  maps::tiles[tile.id] = tile;

  // get distance from entity to tile
  collisions::DistanceToObject dto;
  dto.is_solid = false;
  dto = collisions::_get_entity_to_single_tile_distance(hero::hero.entity_id, tile.id);

  if(dto.is_solid)
  {
    passed = true;
  }
  return passed;
}

bool test_collisions__get_entity_to_map_dto_is_near()
{
  bool passed = false;

  // preparation: create hero entity
  hero::create_new("testhero", "barbarian");
  hero::set_position(450, 370);

  // Create tile
  maps::TileData tile = maps::generate_tile(450, 
                                            360, 
                                            2, //dungeon 
                                            11); //solid frame_id
  maps::tiles[tile.id] = tile;

  collisions::DistanceToObject dto = collisions::_get_entity_to_single_tile_distance(hero::hero.entity_id, tile.id);

  if(dto.is_near)
  {
    passed = true;
  }
  return passed;
}



// bool test_collisions__get_entity_to_entity_dto_solidpass()
// {
//   bool passed = false;

//   // preparation: create hero entity
//   hero::create_new("testhero", "barbarian");
//   hero::set_position(450, 370);

//   // add item on ground
//   items::init();
//   // items::put_item_on_ground(0, 450, 360);

//   // // get distance from entity to tile
//   // collisions::DistanceToObject dto;
//   // dto.is_solid = true;
//   // dto = collisions::_get_entity_to_single_entity_distance(hero::hero.entity_id, 2);
//   // // item on ground should not be solid
//   // if(!dto.is_solid)
//   // {
//   //   passed = true;
//   // }
//   return passed;
// }