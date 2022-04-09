#include <algorithm>
#include <cmath>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

#include "anims.h"
#include "entity.h"
#include "fonts.h"
#include "hero.h"
#include "items.h"
#include "mobs.h"
#include "models.h"
#include "npcs.h"
#include "quads.h"
#include "utils.h"

#include "../dependencies/parallel_hashmap/phmap.h"
#include "../dictionary.h"

namespace entity
{

  phmap::flat_hash_map<int, EntityData> entities;
  std::vector<int> Index = {};
  phmap::flat_hash_map<int, sig_ptr> menu_entity_type_map;


  void init()
  {
    // entity::menu_entity_type_map[ENTITY_TYPE_HERO] = hero::info;
    // entity::menu_entity_type_map[ENTITY_TYPE_NPC] = npcs::info;
    // entity::menu_entity_type_map[ENTITY_TYPE_MOB] = mobs::info;
    // entity::menu_entity_type_map[ENTITY_TYPE_ITEM] = items::info;
    std::cout << "Entity Initialized" << std::endl;
  };

  template <typename T>
  int create(T data, int entity_type_id, float camera_type, int entity_id)
  { 
    entity::EntityData edd;

    if(entity_id == -1){
      edd.id = utils::generate_id(entity::Index);
    } else {
      if(entity::entities.count(entity_id) > 0){
        std::cout << "\033[1;31mERROR DOUBLED ENTITY_ID " << entity_id << "\033[0m"<< std::endl;
      }
      edd.id = entity_id;
    }

    edd.model_id = data.model_id;
    // edd.frame_id = data.current_frame;
    edd.entity_type_id = entity_type_id;
    edd.x = data.x;
    edd.y = data.y;
    edd.z = 0.5f;
    edd.h = data.h;
    edd.w = data.w;
    edd.prev_x = data.x;
    edd.prev_y = data.y;
    edd.mid_x = edd.x + (edd.w/2);
    edd.mid_y = edd.y + (edd.h/2);
    edd.diag = std::sqrt(std::pow((edd.w/2),2) + std::pow((edd.h/2),2));
    edd.camera_type = camera_type;
    edd.is_solid = true;

    // if(entity_type_id == ENTITY_TYPE_ITEM)
    // {
    //   edd.is_solid = false;
    // }

    edd.is_clicked = false;
    edd.speed = data.speed;

    edd.r = 0.5;
    edd.g = 0.5;
    edd.b = 0.5;
    edd.a = 1.0;

    models::load(edd.model_id);

    entity::entities[edd.id] = edd;
    std::cout << "Created Entity ID: " <<  edd.id << " of type: " << edd.entity_type_id << std::endl; 

    // Trigger idle animation
    anims::start(ANIM_STANDING_IDLE, edd.id);
    return edd.id;
  }

  void render()
  {
    //quads::add_quads(entity::entities, OBJECT_TYPE_ENTITY);
  };

  void clear()
  {
    entity::Index.clear();
    entity::entities.clear();
    std::cout << "Clearing Entities" << std::endl;
  };

  void drop(int entity_id)
  {
    if(entity::entities.count(entity_id) > 0)
    {
      entity::entities.erase(entity_id);
      utils::drop_id(entity::Index, entity_id);
    }
  };

  void hide(int entity_id)
  {
    if(entity::entities.count(entity_id) > 0)
    {
      entity::entities.erase(entity_id);
    }
  };

  
  std::vector<std::string> info(int entity_id)
  {
    // create a string per row
    entity::EntityData edd = entity::entities.at(entity_id);
    std::vector<std::string> infos = entity::menu_entity_type_map[edd.entity_type_id](entity_id);

    std::string label_id = "id: " + utils::str(edd.id);
    std::string label_pos = "pos: " + utils::str(int(edd.x)) + ", " + utils::str(int(edd.y));
    infos.push_back(label_id);
    infos.push_back(label_pos);
    return infos;
  } 


  void update_position(int entity_id, float x, float y)
  {
    entity::entities.at(entity_id).prev_x = entity::entities.at(entity_id).x;
    entity::entities.at(entity_id).prev_y = entity::entities.at(entity_id).y;
    entity::entities.at(entity_id).x = x;
    entity::entities.at(entity_id).y = y;
    entity::entities.at(entity_id).mid_x = x + (entity::entities.at(entity_id).w/2);
    entity::entities.at(entity_id).mid_y = y + (entity::entities.at(entity_id).h/2);
  }


  void print_entity_data()
  {
    for (auto const& [k, v] : entity::entities)
    {
      // std::cout << "Entity ID: " << k 
      //           << " pos: (" << v.pos.x << ',' << v.pos.y 
      //           << ") model_id: " << v.model_id 
      //           << " entity_type_id: " << v.entity_type_id
      //           << std::endl;
    }
  }


  template int entity::create<hero::HeroData>(hero::HeroData, int, float, int);
  template int entity::create<items::GeneratedItemData>(items::GeneratedItemData, int, float, int);
  template int entity::create<mobs::MobData>(mobs::MobData, int, float, int);
  template int entity::create<npcs::NPCData>(npcs::NPCData, int, float, int);

}
