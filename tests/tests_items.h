#include "../modules/collisions.h"
#include "../modules/ecs.h"
#include "../modules/hero.h"
#include "../modules/items.h"
#include "../dictionary.h"
#include "../dependencies/parallel_hashmap/phmap.h"

bool test_items__pickup_item()
{
    ecs::clear();
    bool passed = true;
    int item_entity_id = items::generate_item(1, 200, 200, ITEM_LOCATION_GROUND);
    hero::create_new("testhero", "1_barbarian", 210, 205);
    items::pickup(hero::HERO_ENTITY_ID, item_entity_id);

    if(ecs::equipments.at(hero::HERO_ENTITY_ID).equipment[0] != item_entity_id){
        std::cout << " [ERROR] Item not in equipment" << std::endl;
        return false;
    }

    if(ecs::items.at(item_entity_id).item_location != ITEM_LOCATION_EQ){
        std::cout << " [ERROR] Item location not in EQ" << std::endl;
        return false;
    }

    if(ecs::renderdatas.at(item_entity_id).camera_type != CAMERA_HIDDEN){
        std::cout << " [ERROR] Item camera type not CAMERA_HIDDEN" << std::endl;
        return false;
    }

    return passed;
}

bool test_items__yeet_item()
{
    ecs::clear();
    bool passed = true;
    int item_entity_id = items::generate_item(1, 200, 200, ITEM_LOCATION_GROUND);
    hero::create_new("testhero", "1_barbarian", 210, 205);
    items::pickup(hero::HERO_ENTITY_ID, item_entity_id);
    items::yeet(hero::HERO_ENTITY_ID, item_entity_id, 220, 210);

    if(ecs::equipments.at(hero::HERO_ENTITY_ID).equipment.size() != 0){
        std::cout << " [ERROR] Equipment not empty" << std::endl;
        return false;
    }

    if(ecs::items.at(item_entity_id).item_location != ITEM_LOCATION_GROUND){
        std::cout << " [ERROR] Item location not on ground" << std::endl;
        return false;
    }

    if(ecs::renderdatas.at(item_entity_id).camera_type != CAMERA_DYNAMIC){
        std::cout << " [ERROR] Item camera type not CAMERA_DYNAMIC" << std::endl;
        return false;
    }

    if(ecs::positions.at(item_entity_id).x != 220 & ecs::positions.at(item_entity_id).y != 210){
        std::cout << " [ERROR] Item wrong position" << std::endl;
        return false;
    }

    return passed;

}


