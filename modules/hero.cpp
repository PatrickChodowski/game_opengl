
#include <iostream>

#include "entity.h"
#include "hero.h"
#include "../dependencies/json_struct.h"
#include "../dictionary.h"


namespace hero
{

  hero::HeroData hero;

  void _read_data(std::string char_type)
  {
    std::string file_path = "data/heros/" + char_type + ".json";
    std::string json_data = utils::read_text_file(file_path);
    JS::ParseContext context(json_data);
    context.parseTo(hero::hero);
  }

  void _load_from_save(std::string save_name)
  {
    std::string file_path = "saves/" + save_name + ".json";
    std::string json_data = utils::read_text_file(file_path);
    JS::ParseContext context(json_data);
    context.parseTo(hero::hero);
  };

  void create_new(std::string name, std::string type)
  {
    hero::_read_data(type);
    hero.current_frame = 0;
    hero.current_movement_state = ENTITY_STATE_CALM;
    hero.name = name;
    hero.type = type;
    hero.entity_id = entity::create(hero::hero, ENTITY_TYPE_HERO, CAMERA_DYNAMIC);
  };

  void revert_position_x()
  {
    hero::hero.x = hero::hero.prev_x;
    entity::entities[hero.entity_id].x = entity::entities[hero.entity_id].prev_x;
  }

  void revert_position_y()
  {
    hero::hero.y = hero::hero.prev_y;
    entity::entities[hero.entity_id].y = entity::entities[hero.entity_id].prev_y;
  }

  void set_position(float x, float y)
  {
    hero::hero.prev_x = hero::hero.x;
    hero::hero.prev_y = hero::hero.y;
    hero::hero.x = x;
    hero::hero.y = y;
    entity::entities[hero.entity_id].prev_x = entity::entities[hero.entity_id].x;
    entity::entities[hero.entity_id].prev_y = entity::entities[hero.entity_id].y;
    entity::entities[hero.entity_id].x = x;
    entity::entities[hero.entity_id].y = y;
    entity::entities[hero.entity_id].mid_x = x + (entity::entities[hero.entity_id].w/2);
    entity::entities[hero.entity_id].mid_y = y + (entity::entities[hero.entity_id].h/2);
  }


  void update_position(float camera_move_x, float camera_move_y)
  { 
    float new_hero_x = hero::hero.x + camera_move_x;
    float new_hero_y = hero::hero.y - camera_move_y;

    hero::hero.prev_x = hero::hero.x;
    hero::hero.prev_y = hero::hero.y;

    hero::hero.x = new_hero_x;
    hero::hero.y = new_hero_y;
    
    
    entity::entities[hero.entity_id].prev_x = entity::entities[hero.entity_id].x;
    entity::entities[hero.entity_id].prev_y = entity::entities[hero.entity_id].y;

    entity::entities[hero.entity_id].x = new_hero_x;
    entity::entities[hero.entity_id].y = new_hero_y;
    entity::entities[hero.entity_id].mid_x = new_hero_x + (entity::entities[hero.entity_id].w/2);
    entity::entities[hero.entity_id].mid_y = new_hero_y + (entity::entities[hero.entity_id].h/2);

  }


}
