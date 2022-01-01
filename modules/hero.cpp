
#include <iostream>

#include "debug.h"
#include "entity.h"
#include "game.h"
#include "hero.h"
#include "items.h"
#include "textures.h"

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
    hero::_update_joints();
  }

  void revert_position_y()
  {
    hero::hero.y = hero::hero.prev_y;
    entity::entities[hero.entity_id].pos.y = entity::entities[hero.entity_id].prev_y;
    hero::_update_joints();
  }

  void _update_joints()
  {
    // temporarily just fixed point
    hero::hero.hand_x = hero::hero.x + textures::textures[hero::hero.texture_id].frames[entity::entities[hero::hero.entity_id].frame_id].hand_x;
    hero::hero.hand_y = hero::hero.y + textures::textures[hero::hero.texture_id].frames[entity::entities[hero::hero.entity_id].frame_id].hand_y;

    if(game::IS_DEBUG_MODE)
    {
      debug::render_square(hero::hero.hand_x, hero::hero.hand_y, 10, 10, 0.9, 0.0, 0.0, 1.0);
    }

    if(hero::hero.in_hand_entity_id > -1)
    {
      items::EquippedItems[hero::hero.in_hand_entity_id].x = hero::hero.hand_x;
      items::EquippedItems[hero::hero.in_hand_entity_id].y = hero::hero.hand_y;
      entity::update_position(hero::hero.in_hand_entity_id, hero::hero.hand_x, hero::hero.hand_y);
    }

  };

  void set_position(float x, float y)
  {
    hero::hero.prev_x = hero::hero.x;
    hero::hero.prev_y = hero::hero.y;
    hero::hero.x = x;
    hero::hero.y = y;
    entity::update_position(hero.entity_id, x, y);
    hero::_update_joints();
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
    hero::_update_joints();
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
