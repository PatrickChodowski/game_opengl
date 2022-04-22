
#include "../modules/anims.h"
#include "../modules/ecs.h"
#include "../modules/hero.h"
#include "../modules/items.h"
#include "../modules/maps.h"
#include "../dictionary.h"
#include "../dependencies/parallel_hashmap/phmap.h"

// test if animation starts with hero setup
bool test_anims__hero_anim_start()
{
  ecs::clear();
  bool passed = false;

  // Setup: create hero
  hero::create_new("testhero", "1_barbarian", 450, 370);

  // Animations count should be 1
  if(anims::animsplayed.size() == 1){
    passed = true;
  }
  return passed;
}

// Test if animation starts on any visible entity
bool test_anims__entity_anim_start()
{
  ecs::clear();
  bool passed = false;  

  // create animated entity
  ecs::TempEntityData e;
  e.components = {0,1};
  e.entity_type_id = ENTITY_TYPE_LIVE;
  e.x = 400;
  e.y = 400;
  e.z = 0.30;
  e.w = 10;
  e.h = 10;
  e.model_id = 2;
  e.frame_id = 20101;
  e.side_id = ANIM_SIDE_FRONT;
  e.animated = true;
  int entity_id = ecs::create_entity(&e);

  // Given entity should have animation
  if(anims::animsplayed.count(entity_id)){
    passed = true;
  }
  return passed;
}

// Test if animation starts on any visible entity
bool test_anims__entity_anim_not_start()
{
  ecs::clear();
  bool passed = false;  

  // create animated entity
  ecs::TempEntityData e;
  e.components = {0,1};
  e.entity_type_id = ENTITY_TYPE_LIVE;
  e.x = 400;
  e.y = 400;
  e.z = 0.30;
  e.w = 10;
  e.h = 10;
  e.model_id = 2;
  e.frame_id = 20101;
  e.side_id = ANIM_SIDE_FRONT;
  e.animated = false;
  int entity_id = ecs::create_entity(&e);

  // Given entity should have animation
  if(!anims::animsplayed.count(entity_id)){
    passed = true;
  }
  return passed;
}

// Test if animation starts on any visible entity
bool test_anims__entity_anim_start_later()
{
    ecs::clear();
    bool passed = false;  

    // create animated entity
    ecs::TempEntityData e;
    e.components = {0,1};
    e.entity_type_id = ENTITY_TYPE_LIVE;
    e.x = 400;
    e.y = 400;
    e.z = 0.30;
    e.w = 10;
    e.h = 10;
    e.model_id = 2;
    e.frame_id = 20101;
    e.side_id = ANIM_SIDE_FRONT;
    e.animated = false;
    int entity_id = ecs::create_entity(&e);
    anims::start(ANIM_STANDING_IDLE, entity_id);
    // Given entity should have animation
    if(anims::animsplayed.count(entity_id)){
        passed = true;
    }
    return passed;
}


// If animation updates correctly 
bool test_anims__entity_anim_update()
{
    ecs::clear();
    bool passed = false;  

    // create animated entity
    ecs::TempEntityData e;
    e.components = {0,1};
    e.entity_type_id = ENTITY_TYPE_LIVE;
    e.x = 400;
    e.y = 400;
    e.z = 0.30;
    e.w = 10;
    e.h = 10;
    e.model_id = 2;
    e.frame_id = 20101;
    e.side_id = ANIM_SIDE_FRONT;
    e.animated = false;
    int entity_id = ecs::create_entity(&e);
    anims::start(ANIM_STANDING_IDLE, entity_id);
    // Given entity should have animation
    if(anims::animsplayed.count(entity_id)){
        passed = true;
    }
    return passed;
}