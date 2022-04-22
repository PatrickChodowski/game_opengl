
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
bool test_anims__entity_anim_play()
{
    ecs::clear();
    bool passed = true;  

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

    // Testing IDLE animation
    // frame_count: 3
    // update times: [0.0, 200.0, 700.0, 900.0]
    // time_length: 900.0 
    // breakable = true
    // locking = false
    // frames: [0,1,2]
    // anim side id = ANIM_SIDE_FRONT (1)
    // Final frame_id = ANIM_ID*10000 + SIDE_ID*100 + FRAME_ID
    // 10100, 10101, 10102

    if(ecs::models.at(entity_id).frame_id != 10100){
        std::cout << " [ERROR] Anims update failed on 1st frame" << std::endl;
        std::cout << " [ERROR] Current frame is " << ecs::models.at(entity_id).frame_id << " instead of " << 10100 << std::endl;
        return false;
    }

    SDL_Delay(100);
    anims::play(entity_id);
    if(ecs::models.at(entity_id).frame_id != 10100){
        std::cout << " [ERROR] Anims update too early" << std::endl;
        std::cout << " [ERROR] Current frame is " << ecs::models.at(entity_id).frame_id << " instead of " << 10100 << std::endl;
        return false;
    }
    SDL_Delay(101); //201
    anims::play(entity_id);
    if(ecs::models.at(entity_id).frame_id != 10101){
        std::cout << " [ERROR] Anims update failed on 2nd frame" << std::endl;
        std::cout << " [ERROR] Current frame is " << ecs::models.at(entity_id).frame_id << " instead of " << 10101 << std::endl;
        return false;
    }

    SDL_Delay(500); //701
    anims::play(entity_id);
    if(ecs::models.at(entity_id).frame_id != 10102){
        std::cout << " [ERROR] Anims update failed on 3rd frame" << std::endl;
        std::cout << " [ERROR] Current frame is " << ecs::models.at(entity_id).frame_id << " instead of " << 10102 << std::endl;
        return false;
    }

    SDL_Delay(100); //801
    anims::play(entity_id);
    if(ecs::models.at(entity_id).frame_id != 10102){
        std::cout << " [ERROR] Anims update failed on after 3rd frame check" << std::endl;
        std::cout << " [ERROR] Current frame is " << ecs::models.at(entity_id).frame_id << " instead of " << 10102 << std::endl;
        return false;
    }

    SDL_Delay(100); //901
    anims::play(entity_id);
    if(anims::anims_to_stop[0] != entity_id){
        std::cout << " [ERROR] Anims update failed to stop" << std::endl;
        return false;
    }


    return passed;
}



bool test_anims__entity_anim_breakable()
{
    ecs::clear();
    bool passed = true;  

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

    if(ecs::models.at(entity_id).frame_id != 10100){
        std::cout << " [ERROR] Anims update failed on 1st frame" << std::endl;
        std::cout << " [ERROR] Current frame is " << ecs::models.at(entity_id).frame_id << " instead of " << 10100 << std::endl;
        return false;
    }

    SDL_Delay(100);
    anims::start(ANIM_UNARMED_WALK_FORWARD, entity_id);
    if(ecs::models.at(entity_id).frame_id != 20100){
        std::cout << " [ERROR] Anims update failed on 2nd check" << std::endl;
        std::cout << " [ERROR] Current frame is " << ecs::models.at(entity_id).frame_id << " instead of " << 20100 << std::endl;
        return false;
    }

    return passed;
}


