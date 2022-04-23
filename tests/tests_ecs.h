
#include "../modules/camera.h"
#include "../modules/ecs.h" 
#include "../modules/quads.h"

#include "../dictionary.h"
#include "../dependencies/parallel_hashmap/phmap.h"

bool test_ecs__create_entity_basic()
{
  bool passed = false;

  ecs::TempEntityData e;
  e.name = "test_entity";
  e.components = {};
  e.entity_type_id = ENTITY_TYPE_COLOR;

  int entity_id = ecs::create_entity(&e);

  if(entity_id>0){
    passed = true;
  }
  return passed;
}


bool test_ecs__create_entity_component()
{
    ecs::clear();
    bool passed = true;

    ecs::TempEntityData e;
    e.name = "test_entity";
    e.components = {COMPONENT_POSITION};
    e.entity_type_id = ENTITY_TYPE_COLOR;

    // Position component
    e.x = 100;
    e.y = 100;
    e.z = 0.5;
    e.w = 100;
    e.h = 100;

    int entity_id = ecs::create_entity(&e);
    if(!ecs::positions.count(entity_id)){
        std::cout << " [ERROR] ECS missing component" << std::endl;
        return false;
    }

    if((ecs::positions.at(entity_id).x != 100) || 
        (ecs::positions.at(entity_id).y != 100) || 
        (ecs::positions.at(entity_id).z != 0.5) || 
        (ecs::positions.at(entity_id).w != 100) || 
        (ecs::positions.at(entity_id).h != 100)){
        std::cout << " [ERROR] ECS wrong x,y,z,w,h" << std::endl;
        return false;
    }

    return passed;
}

bool test_ecs__drop_entity()
{
    ecs::clear();
    bool passed = true;

    ecs::TempEntityData e;
    e.name = "test_entity";
    e.components = {COMPONENT_POSITION};
    e.entity_type_id = ENTITY_TYPE_COLOR;

    // Position component
    e.x = 100;
    e.y = 100;
    e.z = 0.5;
    e.w = 100;
    e.h = 100;

    int entity_id = ecs::create_entity(&e);
    if(!ecs::positions.count(entity_id)){
        std::cout << " [ERROR] ECS missing component" << std::endl;
        return false;
    }

    // Dropping the entity
    ecs::drop(entity_id);

    if(ecs::positions.count(entity_id)){
        std::cout << " [ERROR] ECS component not deleted" << std::endl;
        return false;
    }

    if(ecs::entities.count(entity_id)){
        std::cout << " [ERROR] ECS entity not deleted" << std::endl;
        return false;
    }

    return passed;
}


bool test_ecs__set_position()
{
    ecs::clear();
    bool passed = false;

    ecs::TempEntityData e;
    e.name = "test_entity";
    e.components = {COMPONENT_POSITION, COMPONENT_COLLISIONS};
    e.entity_type_id = ENTITY_TYPE_COLOR;

    // Position component
    e.x = 100;
    e.y = 100;
    e.z = 0.5;
    e.w = 100;
    e.h = 100;
    e.radius_x = 30;
    e.radius_y = 30;
    e.is_solid = true;

    int entity_id = ecs::create_entity(&e);
    ecs::set_position(entity_id, 200, 300);
    ecs::PositionComponent pos = ecs::positions.at(entity_id);
    ecs::CollisionsComponent col = ecs::collisions.at(entity_id);

    if(pos.x == 200 & pos.y == 300 & pos.prev_x == 200 & pos.prev_y == 300 & col.mid_x == 250 & col.mid_y == 350){
        passed = true;
    }

    return passed;
}


bool test_ecs__update_position()
{
    ecs::clear();
    bool passed = false;

    ecs::TempEntityData e;
    e.name = "test_entity";
    e.components = {COMPONENT_POSITION, COMPONENT_COLLISIONS};
    e.entity_type_id = ENTITY_TYPE_COLOR;

    // Position component
    e.x = 100;
    e.y = 100;
    e.z = 0.5;
    e.w = 100;
    e.h = 100;
    e.radius_x = 30;
    e.radius_y = 30;
    e.is_solid = true;

    int entity_id = ecs::create_entity(&e);
    ecs::update_position(entity_id, 200, 300);
    ecs::PositionComponent pos = ecs::positions.at(entity_id);
    ecs::CollisionsComponent col = ecs::collisions.at(entity_id);

    if(pos.x == 200 & pos.y == 300 & pos.prev_x == 100 & pos.prev_y == 100 & col.mid_x == 250 & col.mid_y == 350){
        passed = true;
    }

    return passed;
}


bool test_ecs__update_position_diff()
{
    ecs::clear();
    bool passed = false;

    ecs::TempEntityData e;
    e.name = "test_entity";
    e.components = {COMPONENT_POSITION, COMPONENT_COLLISIONS};
    e.entity_type_id = ENTITY_TYPE_COLOR;

    // Position component
    e.x = 100;
    e.y = 100;
    e.z = 0.5;
    e.w = 100;
    e.h = 100;
    e.radius_x = 30;
    e.radius_y = 30;
    e.is_solid = true;

    int entity_id = ecs::create_entity(&e);
    ecs::update_position_diff(entity_id, 10, -10);
    ecs::PositionComponent pos = ecs::positions.at(entity_id);
    ecs::CollisionsComponent col = ecs::collisions.at(entity_id);

    if(pos.x == 110 & pos.y == 110 & pos.prev_x == 100 & pos.prev_y == 100 & col.mid_x == 160 & col.mid_y == 160){
        passed = true;
    }

    return passed;
}


bool test_ecs__revert_position_x()
{
    ecs::clear();
    bool passed = false;

    ecs::TempEntityData e;
    e.name = "test_entity";
    e.components = {COMPONENT_POSITION, COMPONENT_COLLISIONS};
    e.entity_type_id = ENTITY_TYPE_COLOR;

    // Position component
    e.x = 100;
    e.y = 100;
    e.z = 0.5;
    e.w = 100;
    e.h = 100;
    e.radius_x = 30;
    e.radius_y = 30;
    e.is_solid = true;

    int entity_id = ecs::create_entity(&e);
    ecs::update_position(entity_id, 200, 300);
    ecs::revert_position_x(entity_id);

    ecs::PositionComponent pos = ecs::positions.at(entity_id);
    ecs::CollisionsComponent col = ecs::collisions.at(entity_id);

    if(pos.x == 100 & pos.y == 300 & pos.prev_x == 100 & pos.prev_y == 100 & col.mid_x == 150 & col.mid_y == 350){
        passed = true;
    }

    return passed;
}


bool test_ecs__revert_position_y()
{
    ecs::clear();
    bool passed = false;

    ecs::TempEntityData e;
    e.name = "test_entity";
    e.components = {COMPONENT_POSITION, COMPONENT_COLLISIONS};
    e.entity_type_id = ENTITY_TYPE_COLOR;

    // Position component
    e.x = 100;
    e.y = 100;
    e.z = 0.5;
    e.w = 100;
    e.h = 100;
    e.radius_x = 30;
    e.radius_y = 30;
    e.is_solid = true;

    int entity_id = ecs::create_entity(&e);
    ecs::update_position(entity_id, 200, 300);
    ecs::revert_position_y(entity_id);

    ecs::PositionComponent pos = ecs::positions.at(entity_id);
    ecs::CollisionsComponent col = ecs::collisions.at(entity_id);

    if(pos.x == 200 & pos.y == 100 & pos.prev_x == 100 & pos.prev_y == 100 & col.mid_x == 250 & col.mid_y == 150){
        passed = true;
    }

    return passed;
}



bool test_ecs__save_temp()
{
    ecs::clear();
    bool passed = false;

    ecs::TempEntityData e;
    e.name = "test_entity";
    e.components = {COMPONENT_POSITION, COMPONENT_COLLISIONS};
    e.entity_type_id = ENTITY_TYPE_COLOR;

    // Position component
    e.x = 100;
    e.y = 100;
    e.z = 0.5;
    e.w = 100;
    e.h = 100;
    e.radius_x = 30;
    e.radius_y = 30;
    e.is_solid = true;

    int entity_id = ecs::create_entity(&e);
    ecs::save_temp(entity_id);
    ecs::clear();

    if(ecs::entities.size() != 0){
        std::cout << " [ERROR] ECS entities not cleared" << std::endl;
        return false;
    }

    if(ecs::saved_entities.size() != 1){
        std::cout << " [ERROR] ECS entities not saved" << std::endl;
        return false;
    }

    int entity_id2 = ecs::create_entity(&ecs::saved_entities[0]);

    ecs::PositionComponent pos = ecs::positions.at(entity_id2);
    ecs::CollisionsComponent col = ecs::collisions.at(entity_id2);

    std::cout << " position: "

    if(pos.x == 100 & pos.y == 100 & pos.prev_x == 100 & pos.prev_y == 100 
        & col.mid_x == 150 & col.mid_y == 150 
        & col.radius_x == 30 & col.radius_y == 30
        & pos.z == 0.3 & col.is_solid){
        
        passed = true;
    }

    return passed;
}







