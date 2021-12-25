
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
    entity::entities[hero.entity_id].pos.x = entity::entities[hero.entity_id].prev_x;
  }

  void revert_position_y()
  {
    hero::hero.y = hero::hero.prev_y;
    entity::entities[hero.entity_id].pos.y = entity::entities[hero.entity_id].prev_y;
  }

  void set_position(float x, float y)
  {
    hero::hero.prev_x = hero::hero.x;
    hero::hero.prev_y = hero::hero.y;
    hero::hero.x = x;
    hero::hero.y = y;
    entity::update_position(hero.entity_id, x, y);
  }


  void update_position(float camera_move_x, float camera_move_y)
  { 
    float new_hero_x = hero::hero.x + camera_move_x;
    float new_hero_y = hero::hero.y - camera_move_y;
    hero::hero.prev_x = hero::hero.x;
    hero::hero.prev_y = hero::hero.y;
    hero::hero.x = new_hero_x;
    hero::hero.y = new_hero_y;
    entity::update_position(hero.entity_id, new_hero_x, new_hero_y);
  }

  void refresh()
  {
    hero::HeroData new_hero;
    hero::hero = new_hero;
  }

  std::vector<std::string> info(int entity_id)
  {
    std::vector<std::string> infos = {};
    infos.push_back(hero::hero.name);
    infos.push_back(hero::hero.type);
    return infos;
  }


}
